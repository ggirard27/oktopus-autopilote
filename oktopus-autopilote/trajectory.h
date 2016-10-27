#include "pathFinder.h"
using namespace std;

class trajectory
{
	pathFinder pathToTake;
	LoS inSight;

	public:
		trajectory();
		void traj(double[][nCols] , double[][nCols]);
		
	private:
		static double external_boundaries[bordRows][nCols];
		static double external_boundaries2[ileRows][nCols];
		double external_boundariesX[bordRows];
		double external_boundariesY[bordRows];
		double external_boundaries2X[ileRows];
		double external_boundaries2Y[ileRows];
};