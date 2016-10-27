#include "lineOfSight.h"

LoS::LoS()
{
	point_1[1][nCols] = { 0 }, point_2[1][nCols] = { 0 };
	k = 0, i = 0;
	dot_product = 0;
	intersection_check = 0;
	visibility = 0;
	p_calculation_numerator = 0, p_calculation_denominator = 0, p = 0, q = 0, smallest = 0, x_midpoint = 0, y_midpoint = 0, distance_array[bordRows + ileRows] = { 0 }, empty_array[bordRows + ileRows] = { 0 };

	boundariesX[bordRows + ileRows] = { 0 };
	boundariesY[bordRows + ileRows] = { 0 };
	locatedInPolygon = false;
}

bool LoS::isOnPoly(int npol, const double *xp, const double *yp, double x, double y)
{
	int i;
	bool locatedOnPolygon = false;

	for (i = 0; i < npol; i++)
	{
		if ((yp[i] == y) && (xp[i] == x))
			locatedOnPolygon = true;
	}
	return locatedOnPolygon;
}

bool LoS::isInPoly(int npol, const double *xp, const double *yp, double x, double y)
{
	locatedInPolygon = false;
	for (int i = 0, j = npol - 1; i < npol; j = i++)
	{
		if ((((yp[i] <= y) && (y < yp[j])) || ((yp[j] <= y) && (y < yp[i]))) && (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
			locatedInPolygon = !locatedInPolygon;
	}
	return locatedInPolygon;
}

int LoS::lineOfSight(double observerState[][2], double currentTargetNode[][2], double boundaries[bordRows + ileRows][2])
{
	i = 0;
	for (int k = 0; k < bordRows + ileRows; k++)
	{
		
		if (k < bordRows + ileRows - 1)
		{
			// Assign two adjacent points as wall ends(x_1; y_1) and(x_2; y_2)
			point_1[0][0] = boundaries[k][0];
			point_1[0][1] = boundaries[k][1];
			point_2[0][0] = boundaries[k + 1][0];
			point_2[0][1] = boundaries[k + 1][1];
		}
		else if ((k == bordRows + ileRows - 1))
		{
			// Assign the last point as (x_1;y_1) and the first point in the list as (x_2;y_2)
			point_1[0][0] = boundaries[k][0];
			point_1[0][1] = boundaries[k][1];
			point_2[0][0] = boundaries[0][0];
			point_2[0][1] = boundaries[0][1];
		}

		// Calculate beam and wall direction vectors
		beam_direction_vector[0][0] = currentTargetNode[0][0] - observerState[0][0]; 
		beam_direction_vector[0][1] = currentTargetNode[0][1] - observerState[0][1];
		wall_direction_vector[0][0] = point_2[0][0] - point_1[0][0];
		wall_direction_vector[0][1] = point_2[0][1] - point_1[0][1];

		// Check for cosine of angle between the lines
		dot_product = (beam_direction_vector[0][0] * wall_direction_vector[0][0]) + (beam_direction_vector[0][1] * wall_direction_vector[0][1]);
		intersection_check = dot_product / (sqrt(pow(beam_direction_vector[0][0], 2) + pow(beam_direction_vector[0][1], 2))*sqrt(pow(wall_direction_vector[0][0], 2) + pow(wall_direction_vector[0][1], 2)));
		
		// Make sure that the lines are not parallel(and therefore will intersect)
		if (intersection_check != 1 && intersection_check != -1)
		{
			// Find p(distance to wall expressed as a multiplier to the direct distance between the observer and curent target node)
			p_calculation_numerator = (point_2[0][0] - point_1[0][0])*(point_1[0][1] - observerState[0][1]) - (point_2[0][1] - point_1[0][1])*(point_1[0][0] - observerState[0][0]);
			p_calculation_denominator = (point_2[0][0] - point_1[0][0])*(currentTargetNode[0][1] - observerState[0][1]) - (point_2[0][1] - point_1[0][1])*(currentTargetNode[0][0] - observerState[0][0]);
			p = p_calculation_numerator / p_calculation_denominator;
		}

		// Check if particle is actually facing wall(true if p >= 0)
		if (p >= 0)
		{
			if (point_2[0][1] - point_1[0][1] == 0) // Check if wall is horizontal

				// find q(intersection position along wall vector)
				q = (observerState[0][0] - point_1[0][0] + p*(currentTargetNode[0][0] - observerState[0][0])) / (point_2[0][0] - point_1[0][0]);

			else if (point_2[0][0] - point_1[0][0] == 0) // Check if wall is vertical

				// find q
				q = (observerState[0][1] - point_1[0][1] + p*(currentTargetNode[0][1] - observerState[0][1])) / (point_2[0][1] - point_1[0][1]);

			else //If wall is neither vertical nor horizontal

				// find q
				q = (observerState[0][1] - point_1[0][1] + p*(currentTargetNode[0][1] - observerState[0][1])) / (point_2[0][1] - point_1[0][1]); // Can use any of the two previous equations for calculating q

			// Check if intersection happened within the wall ends
			if (q >= 0 && q <= 1)
			{
				// Store the p - value
				i = i + 1;
				distance_array[i - 1] = p;
			}
		}
	}

	// Get the minimum value only from the first i entries; the other entries may be zero or residual from previous iterations
	if (i == 0)
		smallest = NAN;
	else
	{
		smallest = distance_array[0];
		for (int temp = 1; temp < i; temp++)
		{
			if (distance_array[temp] < smallest)
				smallest = distance_array[temp];
		}
	}
	if (smallest == 0) // If observer coordinates coincide with a wall
	{
		smallest = distance_array[0];
		for (int temp = 1; temp < i; temp++)
		{
			//Only consider nonzero entries i.e.walls that are not coincident with the observer
			if (distance_array[temp] < smallest && distance_array[temp] != 0 || smallest == 0)
				smallest = distance_array[temp];
		}
	}
	
	if (smallest < 1 && !isinf(smallest)) // If wall encountered between observer and current target node
		visibility = 0; // does not have a direct line of sight to the current target node
	else
	{
		// Establish whether the path from the current to the target node passes beyond the boundaries

		// Calculate the midpoint between the current and target nodes
		x_midpoint = 0.5*(observerState[0][0] + currentTargetNode[0][0]);
		y_midpoint = 0.5*(observerState[0][1] + currentTargetNode[0][1]);

		// Determine if the midpoint is within the boundaries
		for (int i = 0; i < bordRows + ileRows; i++)
		{
			boundariesX[i] = boundaries[i][0];
			boundariesY[i] = boundaries[i][1];
		}

		IN = isInPoly(bordRows + ileRows, boundariesX, boundariesY, x_midpoint, y_midpoint);
		ON = isOnPoly(bordRows + ileRows, boundariesX, boundariesY, x_midpoint, y_midpoint);

		// If the path between the current and target nodes lie within the boundaries
		if (IN == 1 || ON == 1)
			visibility = 1; // has a direct line of sight to the target node
		else
			visibility = 0; // does not have a direct line of sight to the current target node
	}
	return visibility;
}