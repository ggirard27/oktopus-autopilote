#include <iostream>
#include "trajectory.h"

trajectory::trajectory()
{
	external_boundariesX[bordRows] = { 0 };
	external_boundariesY[bordRows] = { 0 };
	external_boundaries2X[ileRows] = { 0 };
	external_boundaries2Y[ileRows] = { 0 };
}

double trajectory::external_boundaries[bordRows][nCols] = {
	{ 45.418814, -72.100747 },
	{ 45.417669, -72.101300 },
	{ 45.416628, -72.102644 },
	{ 45.415214, -72.103114 },
	{ 45.413444, -72.102986 },
	{ 45.411619, -72.102425 },
	{ 45.409828, -72.102036 },
	{ 45.408408, -72.103186 },
	{ 45.407272, -72.105069 },
	{ 45.406472, -72.106439 },
	{ 45.405014, -72.105222 },
	{ 45.403192, -72.104944 },
	{ 45.401664, -72.105522 },
	{ 45.400106, -72.106772 },
	{ 45.400339, -72.104017 },
	{ 45.400442, -72.100619 },
	{ 45.400986, -72.097942 },
	{ 45.402278, -72.096039 },
	{ 45.403925, -72.095931 },
	{ 45.405897, -72.096264 },
	{ 45.407517, -72.096058 },
	{ 45.409072, -72.095567 },
	{ 45.410617, -72.094892 },
	{ 45.412131, -72.094017 },
	{ 45.413783, -72.093672 },
	{ 45.415533, -72.094169 },
	{ 45.416494, -72.095783 },
	{ 45.417164, -72.097425 },
	{ 45.417847, -72.099331 },
	{ 45.418814, -72.100747 },
};

double trajectory::external_boundaries2[ileRows][nCols] = {
	{ 45.416835, -72.102203 },
	{ 45.416835, -72.098791 },
	{ 45.414749, -72.098791 },
	{ 45.410394, -72.101037 },
	{ 45.414749, -72.102203 },
	{ 45.416835, -72.102203 },
};

void trajectory::traj(double start_point[][nCols], double end_point[][nCols])
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
	
	if ((inSight.isInPoly(bordRows, external_boundariesX, external_boundariesY, start_point[0][0], start_point[0][1]) == 1) &&
		(inSight.isInPoly(bordRows, external_boundariesX, external_boundariesY, end_point[0][0], end_point[0][1]) == 1) &&
		(inSight.isInPoly(ileRows, external_boundaries2X, external_boundaries2Y, start_point[0][0], start_point[0][1]) == 0) &&
		(inSight.isInPoly(ileRows, external_boundaries2X, external_boundaries2Y, end_point[0][0], end_point[0][1]) == 0))
	{
		double **value = pathToTake.pathFinding(start_point, end_point, external_boundaries, external_boundaries2);
		for (int i = 1; i <= value[0][0] + 1; i++)
		{
			cout << value[i][0] << ", " << value[i][1] << endl;
		}

	}
	else
		cout << "Point de debut ou d'arrive est a l'exterieur du cadre";

	cin.get();
}