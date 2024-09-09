#include <iostream>


#include "console.h"
#include "math.h"


/*
    Value normalization
*/
double norm
(
    double  aValue, /* Value */
    double  aMin,   /* Minimal value will be 0.0 */
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
    double aMin,    /* Minimal value will be minimal limit */
    double aMax,    /* Maximum value will be maximal limit */
    double aLimMin, /* Minimal limit */
    double aLimMax, /* Maximal limit */
    bool   aCut     /* Cut reusult form 0 to 1 for true */
)
{
    return aLimMin + norm( aValue, aMin, aMax, aCut ) * ( aLimMax - aLimMin );
}



std::string valueToColor
(
    double aValue
)
{
    if( aValue > EPSILON_D ) return INK_GREEN;
    else if( aValue < - EPSILON_D ) return INK_RED;
    else return INK_GREY;
}



std::string valueToChar
(
    double aValue
)
{
    string result = valueToColor( aValue );
    auto absValue = abs( aValue );

    if( absValue < EPSILON_D ) result += "0";
    else if( absValue < 1e-9 ) result += "n";
    else if( absValue < 1e-6 ) result += "µ";
    else if( absValue < 1e-3 ) result += "m";
    else if( absValue < 0.01 ) result += "c";
    else if( absValue < 0.1 )  result += "d";
    else if( absValue < 0.2 )  result += "1";
    else if( absValue < 0.3 )  result += "2";
    else if( absValue < 0.4 )  result += "3";
    else if( absValue < 0.5 )  result += "4";
    else if( absValue < 0.6 )  result += "5";
    else if( absValue < 0.7 )  result += "6";
    else if( absValue < 0.8 )  result += "7";
    else if( absValue < 0.9 )  result += "8";
    else if( absValue < 1.0 )  result += "9";
    else if( absValue < 10 )   result += "I";
    else if( absValue < 100 )  result += "D";
    else if( absValue < 1000 ) result += "K";
    else if( absValue < 1000000 ) result += "M";
    else if( absValue < 1000000000 ) result += "B";
    else result += "!";

    return result + INK_DEFAULT;
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
