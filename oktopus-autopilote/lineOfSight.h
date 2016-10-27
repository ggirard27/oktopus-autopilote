#include <math.h>

#define bordRows 30
#define ileRows 6
#define nCols 2
#define nodeObstacle 3

class LoS
{
	friend class trajectory;

	public:
		int lineOfSight(double [][nCols], double [][nCols], double [][nCols]);
		LoS();

	private:
		double point_1[1][nCols], point_2[1][nCols], beam_direction_vector[1][nCols], wall_direction_vector[1][nCols];
		int k, i;
		double dot_product;
		double intersection_check;
		double p_calculation_numerator, p_calculation_denominator, p, q, smallest, x_midpoint, y_midpoint, distance_array[bordRows + ileRows], empty_array[bordRows + ileRows];
		bool IN, ON;
		int visibility;
		
		double boundariesX[bordRows + ileRows];
		double boundariesY[bordRows + ileRows];
		bool isOnPoly(int, const double *, const double *, double, double);
		bool isInPoly(int, const double *, const double *, double, double);
		bool locatedInPolygon;
};