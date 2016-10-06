#include <iostream>
#include <math.h>

using namespace std;

const int bordRows = 29;
const int ileRows = 5;
const int nCols = 2;
const int nodeObstacle = 20;
int visibility = 0;

long double external_boundariesX[bordRows];
long double external_boundariesY[bordRows];
long double external_boundaries2X[ileRows];
long double external_boundaries2Y[ileRows];

long double boundaries[bordRows + ileRows][nCols];
long double boundariesX[bordRows + ileRows];
long double boundariesY[bordRows + ileRows];
long double initial_combined_nodes[2 + bordRows + ileRows][2];
long double observerState[1][nCols];
long double currentTargetNode[1][nCols];
long double **way_Coordinates;

double start_point[1][nCols] = { 45.4175, -72.1 };
double end_point[1][nCols] = { 45.414, -72.1 };

long double external_boundaries[bordRows][nCols] = {
	{ 45.40046798639914, -72.10604701096973 },
	{ 45.39856886474337, -72.10890306092946 },
	{ 45.39710554076825, -72.10993786224121 },
	{ 45.39725464647795, -72.1107417384402 },
	{ 45.39760829005974, -72.11158357693286 },
	{ 45.39818790553468, -72.11216479769061 },
	{ 45.39916161019566, -72.11245332252739 },
	{ 45.39967936801582, -72.11189216152513 },
	{ 45.39963890474237, -72.11063083012138 },
	{ 45.39966277033507, -72.10927143136972 },
	{ 45.40026782363386, -72.10839983660577 },
	{ 45.40134371819854, -72.10811733986587 },
	{ 45.40177007410129, -72.10771491831483 },
	{ 45.4017851487011, -72.10770901760276 },
	{ 45.40196094111802, -72.10673112086268 },
	{ 45.41199489061331, -72.10510825614155 },
	{ 45.4164241298658, -72.10474232881299 },
	{ 45.41809250293986, -72.10020026895782 },
	{ 45.41730005453744, -72.09889508081221 },
	{ 45.41748252470161, -72.09636973579238 },
	{ 45.41886691401324, -72.09547335478099 },
	{ 45.41877571690554, -72.09265808117013 },
	{ 45.41862314505141, -72.09198474352155 },
	{ 45.41084117289119, -72.09208178695351 },
	{ 45.40800770113289, -72.09386209628548 },
	{ 45.39738812760901, -72.09500501379034 },
	{ 45.39737957251242, -72.09785588773259 },
	{ 45.3991270131628, -72.0996214370313 },
	{ 45.40052279539043, -72.1028439821494 }
};  // bord

long double external_boundaries2[ileRows][nCols] = {
	{ 45.41713306383333, -72.09818256240924 },
	{ 45.41444637424612, -72.09872774230134 },
	{ 45.41444427330208, -72.10223184215823 },
	{ 45.4167747208955, -72.10249310273306 },
	{ 45.41713306383333, -72.09818256240924 },
};	// lac

// Check if points are inside polygon
bool isInPoly(int npol, const long double *xp, const long double *yp, double x, double y)
{
	int i, j;
	bool locatedInPolygon = false;

	for (i = 0, j = npol - 1; i < npol; j = i++) 
	{
		if ((((yp[i] <= y) && (y < yp[j])) ||((yp[j] <= y) && (y < yp[i]))) && (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
			locatedInPolygon = !locatedInPolygon;
	}
	return locatedInPolygon;
}

bool isOnPoly(int npol, const long double *xp, const long double *yp, double x, double y)
{
	int i;
	bool locatedOnPolygon = false;

	for (i = 0; i < npol; i++)
	{
		if ((yp[i] == y) && ( xp[i] == x))
			locatedOnPolygon = true;
	}
	return locatedOnPolygon;
}

// Determine whether the current target node is visible from the current observer position
int lineOfSight(long double observerState[1][nCols], long double currentTargetNode[1][nCols])
{
	long double point_1[1][nCols], point_2[1][nCols], beam_direction_vector[1][nCols], wall_direction_vector[1][nCols];
	int k = 0, i = 0;
	double dot_product = 0;
	double intersection_check = 0;
	double p_calculation_numerator = 0, p_calculation_denominator = 0, p = 0, q = 0, smallest = 0, x_midpoint = 0, y_midpoint = 0, distance_array[bordRows + ileRows] = { 0 }, empty_array[bordRows + ileRows] = { 0 };
	bool IN, ON;
	
	for (int k = 0; k < (sizeof(boundaries) / sizeof(long double) / 2); k++)
	{
		if (k < (sizeof(boundaries) / sizeof(long double) / 2)-1)
			{
				// Assign two adjacent points as wall ends(x_1; y_1) and(x_2; y_2)
				point_1[0][0] = boundaries[k][0];
				point_1[0][1] = boundaries[k][1];
				point_2[0][0] = boundaries[k+1][0];
				point_2[0][1] = boundaries[k+1][1];				
			}
		else if ((k==sizeof(boundaries) / sizeof(long double) / 2)-1)
			{
				// Assign the last point as (x_1;y_1) and the first point in the list as (x_2;y_2)
				point_1[0][0] = boundaries[k][0];
				point_1[0][1] = boundaries[k][1];
				point_2[0][0] = boundaries[0][0];
				point_2[0][1] = boundaries[0][1];
			}

		// Calculate beam and wall direction vectors
		beam_direction_vector[0][0] = currentTargetNode[0][0] - observerState[0][0]; // double check why variables are different
		beam_direction_vector[0][1] = currentTargetNode[0][1] - observerState[0][1];
		wall_direction_vector[0][0] = point_2[0][0] - point_1[0][0];
		wall_direction_vector[0][1] = point_2[0][1] - point_1[0][1];
		
		// Check for cosine of angle between the lines
		dot_product = (beam_direction_vector[0][0] * wall_direction_vector[0][0]) + (beam_direction_vector[0][1] * wall_direction_vector[0][1]);
		intersection_check = dot_product / (sqrt(pow(beam_direction_vector[0][0],2) + pow(beam_direction_vector[0][1],2))*sqrt(pow(wall_direction_vector[0][0],2) + pow(wall_direction_vector[0][1],2)));
		//cout << dot_product << endl;
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
			if (distance_array[temp] < smallest) // i!=2 problem 
				smallest = distance_array[temp];
		}
	}
	if (smallest == 0) // If observer coordinates coincide with a wall
	{
		smallest = distance_array[0];
		for (int temp = 1; temp < i; temp++)
		{
			//Only consider nonzero entries i.e.walls that are not coincident with the observer
			if (distance_array[temp] < smallest && distance_array[temp]!=0 || smallest == 0)
				smallest = distance_array[temp];
		}
	}
	//cout << isinf(smallest);
	if (smallest < 1 && !isinf(smallest) ) // If wall encountered between observer and current target node
		visibility = 0; // does not have a direct line of sight to the current target node
	else
	{
		// Establish whether the path from the current to the target node passes beyond the boundaries

		// Calculate the midpoint between the current and target nodes
		x_midpoint = 0.5*(observerState[0][0] + currentTargetNode[0][0]);
		y_midpoint = 0.5*(observerState[0][1] + currentTargetNode[0][1]);

		// Determine if the midpoint is within the boundaries
		for (int i = 0; i < bordRows+ileRows; i++)
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

long double* pathFinder(double start_point[1][nCols], double end_point[1][nCols], long double external_boundaries[bordRows][nCols], long double external_boundaries2[ileRows][nCols])
{
	long double combined_nodes[2 + bordRows + ileRows] = { 0 }, visible_neighbours_library[2 + bordRows + ileRows][2 + bordRows + ileRows] = { 0 }, shortest_path_array[nCols][2 + bordRows + ileRows] = { 0 }, min_distance;
	int current_node_ID_index = 0, current_node_ID = 0, visitedNodes = 0, posCounter = 0, targetNodeID = 0, path_index = 0, path[2 + bordRows + ileRows] = { 0 };
	int *unvisited_nodes, *pathToTake;
	float cumulative_distance_to_current_node = 0, new_cumulative_distance = 0, previous_cumulative_distance_to_target_node = 0, distance_from_current_to_target_node = 0;

	// external boundaries
	for (int i = 0; i < nodeObstacle; i++) 
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries[i][j];
		}
	}

	for (int i = nodeObstacle; i < (nodeObstacle+ileRows); i++) 
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries2[i - nodeObstacle][j];
		}
	}

	for (int i = (nodeObstacle + ileRows); i < (bordRows + ileRows); i++) 
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries[i - ileRows][j];
		}
	}

	// Create initial unvisited nodes array (missing 3rd row = infinity)
	initial_combined_nodes[0][0] = start_point[0][0];
	initial_combined_nodes[0][1] = start_point[0][1];

	for (int i = 0; i < (1 + bordRows + ileRows); i++) 
	{
		for (int j = 0; j < nCols; j++)
		{
			initial_combined_nodes[i+1][j] = boundaries[i][j];
		}
	}
	initial_combined_nodes[1 + bordRows + ileRows][0] = end_point[0][0];
	initial_combined_nodes[1 + bordRows + ileRows][1] = end_point[0][1];

	// Initialize vector infinity
	for (int i = 0; i < (bordRows + ileRows + 2); i++)
	{
		for (int j = 0; j < (bordRows + ileRows + 2); j++)
		{
			visible_neighbours_library[i][j] = NAN; 
		}
		combined_nodes[i] = NAN; 
	}

	// Create a library listing the visible neighbours of all of the nodes and their distances with respect to the reference nodes 
	for (int referenceNodeID = 0; referenceNodeID < (2 + bordRows + ileRows); referenceNodeID++) 
	{
		for (int targetID = 0; targetID < (2 + bordRows + ileRows); targetID++)
		{
			//Assign observer and target nodes
			observerState[0][0] = initial_combined_nodes[referenceNodeID][0];
			currentTargetNode[0][0] = initial_combined_nodes[targetID][0];
			observerState[0][1] = initial_combined_nodes[referenceNodeID][1];
			currentTargetNode[0][1] = initial_combined_nodes[targetID][1];

			//Check visibility of target node from observer
			visibility = lineOfSight(observerState, currentTargetNode);

			if (visibility == 1) // If target is visible
			{
				//Overwrite recorded distance
				combined_nodes[targetID] = sqrt(pow((currentTargetNode[0][0] - observerState[0][0]),2) + pow((currentTargetNode[0][1] - observerState[0][1]),2)); //double check variable init
				
				// Create a three dimensional array representing a library of the visible neighbours with respect to the reference node
				// The reference node ID is represented by the 'page' number
				visible_neighbours_library[referenceNodeID][targetID] = combined_nodes[targetID];
				combined_nodes[targetID] = NAN;
			}			
		}
	}

	// Generate a list of all available nodes and assign them to the unvisited nodes set
	unvisited_nodes = new int[2 + bordRows + ileRows - visitedNodes];
	for (int index = 0; index < (2 + bordRows + ileRows); index++)
	{
		unvisited_nodes[index] = index;
	}
	
	// Copy the first 'page' of the visible_neighbours_library as the initial shortest path array
	for (int i = 0; i < (2 + bordRows + ileRows); i++)
	{
		shortest_path_array[0][i] = visible_neighbours_library[0][i];
		// Initialize all of the precursor nodes to 1 (the starting point)
		shortest_path_array[1][i] = 0;
	}
	// Take out the starting point(node 1) from the unvisited nodes set
	current_node_ID = 0;
	int *temp = new int[2 + bordRows + ileRows - visitedNodes];

	for (int i = 0; i< (2 + bordRows + ileRows - visitedNodes); i++)
	{
		if (unvisited_nodes[i] != current_node_ID)
		{
			temp[posCounter] = unvisited_nodes[i];
			posCounter = posCounter + 1;
		}
	}
	visitedNodes = 1;
	for (int i = 0; i< posCounter; i++)
	{
		unvisited_nodes[i] = temp[i];
	}
	delete[] temp;
	temp = NULL;

	while (2 + bordRows + ileRows - visitedNodes > 0)
	{
		min_distance = shortest_path_array[0][0];
		long double *cumulative_distances = new long double[2 + bordRows + ileRows - visitedNodes];

		// Find the node with the smallest nonzero cumulative distance to be assigned as the next current node
		for (int i = 1; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			cumulative_distances[i] = shortest_path_array[0][unvisited_nodes[i]];
			if ((min_distance > cumulative_distances[i] || min_distance == 0) && !isnan(cumulative_distances[i]) && cumulative_distances[i] != 0)
			{
				min_distance = cumulative_distances[i];
				current_node_ID_index = i;
			}
		}

		delete[] cumulative_distances;
		cumulative_distances = NULL;

		// Extract the current node
		current_node_ID = unvisited_nodes[current_node_ID_index];

		// Take out the current node from the set of unvisited nodes
		//unvisited_nodes = setdiff(unvisited_nodes, current_node_ID);
		int *temp = new int[2 + bordRows + ileRows - visitedNodes];
		posCounter = 0;
		for (int i = 0; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			if (unvisited_nodes[i] != current_node_ID)
			{
				temp[posCounter] = unvisited_nodes[i];
				posCounter = posCounter + 1;
			}
		}
		visitedNodes = visitedNodes + 1;
		for (int i = 0; i < posCounter; i++)
		{
			unvisited_nodes[i] = temp[i];
			//cout << unvisited_nodes[i] << " ";
		}
		delete[] temp;
		//cout << endl;

		// Refer to the library to find the distance to visible(neighboring) and UNVISITED nodes
		for (int i = 0; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			// Assign one of the unvisited nodes as the target node
			targetNodeID = unvisited_nodes[i];

			// Visibility is implied if the distance recorded between the current and target nodes(in the library) is less than infinity
			if (!isnan(visible_neighbours_library[current_node_ID][targetNodeID]))
			{
				// Calculate the(tentative) cumulative distance for the target node
				cumulative_distance_to_current_node = shortest_path_array[0][current_node_ID];
				distance_from_current_to_target_node = visible_neighbours_library[current_node_ID][targetNodeID];
				new_cumulative_distance = cumulative_distance_to_current_node + distance_from_current_to_target_node;

				// Find the PREVIOUS cumulative distance to the target node
				previous_cumulative_distance_to_target_node = shortest_path_array[0][targetNodeID];

				// If a smaller cumulative distance from the starting node to the target node, passing through the current node was obtained
				if (new_cumulative_distance < previous_cumulative_distance_to_target_node || isnan(previous_cumulative_distance_to_target_node))
				{
					// Overwrite the previous cumulative distance to the smaller value
					shortest_path_array[0][targetNodeID] = new_cumulative_distance;

					// Replace the last precursor node with current_node_ID
					shortest_path_array[1][targetNodeID] = current_node_ID;
				}
			}
		}
	}

	delete[] unvisited_nodes;
	unvisited_nodes = NULL;

	// Extract the shortest path
	//Assign the end point ID as the first entry on the path array
	path[path_index] = (sizeof(shortest_path_array)/sizeof(long double)/2)-1;
	
	while (path[path_index] > 0) // Repeat until the starting node is reached
	{
		path_index = path_index + 1;
		path[path_index] = shortest_path_array[1][path[path_index - 1]];
	}

	pathToTake = new int[path_index];
	way_Coordinates = new long double*[path_index+1];
	for (int i = 0; i <= path_index+1; ++i)
	{
		way_Coordinates[i] = new long double[nCols];
	}
	for (int i = 0; i <= path_index; i++)
	{
		pathToTake[i+1] = path[path_index-i];
		way_Coordinates[i+1][0] = initial_combined_nodes[pathToTake[i+1]][0];
		way_Coordinates[i+1][1] = initial_combined_nodes[pathToTake[i+1]][1];
	}

	// store sizeof(dynamic array) in first array 
	way_Coordinates[0][0] = path_index;

	return *way_Coordinates;
}


int main()
{
	for (int i = 0; i < bordRows; i++)
	{
		external_boundariesX[i] = external_boundaries[i][0];
		external_boundariesY[i] = external_boundaries[i][1];
	}

	for (int i = 0; i < ileRows; i++)
	{
		external_boundaries2X[i] = external_boundaries2[i][0];
		external_boundaries2Y[i] = external_boundaries2[i][1];
	}

	if ((isInPoly(bordRows, external_boundariesX, external_boundariesY, start_point[0][0], start_point[0][1]) == 1) &&
		(isInPoly(bordRows, external_boundariesX, external_boundariesY, end_point[0][0], end_point[0][1]) == 1) &&
		(isInPoly(ileRows, external_boundaries2X, external_boundaries2Y, start_point[0][0], start_point[0][1]) == 0) &&
		(isInPoly(ileRows, external_boundaries2X, external_boundaries2Y, end_point[0][0], end_point[0][1]) == 0))
	{
		long double *value = pathFinder(start_point, end_point, external_boundaries, external_boundaries2);
		for (int i = 1; i <= value[0]+1; i++)
		{
			cout << way_Coordinates[i][0] << ", " << way_Coordinates[i][1] << endl;
		}
		
	}
	else
		cout << "Starting point or destination point is outside of reach";

	cin.get();
	return 0;
}
