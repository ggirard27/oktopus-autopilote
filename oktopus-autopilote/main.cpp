#include "trajectory.h"

double start_point[1][2] = { 45.4175, -72.1 };
double end_point[1][2] = { 45.401, -72.106 };

int main()
{
	trajectory findPath;

	findPath.traj(start_point, end_point);

	return 0;
}