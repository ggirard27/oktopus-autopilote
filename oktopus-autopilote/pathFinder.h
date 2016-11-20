#include "lineOfSight.h"

class pathFinder
{
	LoS isLineOfSight;

	public:
		pathFinder();
    ~pathFinder();
		double **pathFinding(double [][nCols], double[][nCols] , double[bordRows][nCols], double[ileRows][nCols]);

	private:
		double **way_Coordinates;
};
