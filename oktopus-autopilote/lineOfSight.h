#include <math.h>

#define bordRows 6//30 //6 test campus
#define ileRows 5//6 //5  test campus
#define nCols 2
#define nodeObstacle 4//3 //4 test sur campus
 
class LoS
{
	friend class Trajectory;

	public:
		int lineOfSight(double [][nCols], double [][nCols], double [][nCols]);
		LoS();

	private:
		bool isOnPoly(int, const double *, const double *, double, double);
		bool isInPoly(int, const double *, const double *, double, double);
		//bool locatedInPolygon;
};
