#include "pathFinder.h"
using namespace std;

class Trajectory
{
	pathFinder pathToTake;
	LoS inSight;

	public:
		Trajectory();
    ~Trajectory();
		void getData(double[][nCols] , double[][nCols], double listPointsX[10],double listPointsY[10]);
		
	private:
		static double external_boundaries[bordRows][nCols];
		static double external_boundaries2[ileRows][nCols];
		/*double external_boundariesX[bordRows];
		double external_boundariesY[bordRows];
		double external_boundaries2X[ileRows];
		double external_boundaries2Y[ileRows];*/
		double **value;
};
