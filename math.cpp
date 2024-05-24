#include "math.h"



/*
    Value normalization
*/
double norm
(
    double  aValue, /* Value */
    double  aMin,   /* Minimam value will be 0.0 */
    double  aMax,   /* Maximum value will be 1.0 */
    bool    aCut    /* Cut reusult form 0 to 1 for true */
)
{
    auto delta = aMax - aMin;
    auto result = delta > EPSILON_D ? ( aValue - aMin ) / delta : 0;
    return aCut
    ? ( result < 0.0 ? 0.0 : ( result > 1.0 ? 1.0 : result ) )
    : result;
}



double norm
(
    double aValue,  /* Value */
    double aMin,    /* Minimam value will be minimal limit */
    double aMax,    /* Maximum value will be maximal limit */
    double aLimMin, /* Minimal limit */
    double aLimMax, /* Maximal limit */
    bool   aCut     /* Cut reusult form 0 to 1 for true */
)
{
    return aLimMin + norm( aValue, aMin, aMax, aCut ) * ( aLimMax - aLimMin );
}






/*
double Math::catmull
(
    double p0,
    double p1,
    double p2,
    double p3,
    double t
)
{
	double v0 = ( p2 - p0 ) * 0.5;
	double v1 = ( p3 - p1 ) * 0.5;
	double t2 = t * t;
	double t3 = t * t2;
	return
    ( 2 * p1 - 2 * p2 + v0 + v1 ) *
    t3 + ( - 3 * p1 + 3 * p2 - 2 * v0 - v1 ) *
    t2 + v0 * t + p1;
}
*/
