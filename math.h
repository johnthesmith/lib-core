#pragma once

#include <string>

/* Pi constant */
static const double PI = 3.14159265358979323846;

/* Minimal value  */
static const double EPSILON_D = 1e-9;



/*
    Value normalization
*/
double norm
(
    double,         /* Value */
    double = 0.0,   /* Minimam value will be 0.0 */
    double = 1.0,   /* Maximum value will be 1.0 */
    bool = false    /* Cut reusult form 0 to 1 for true */
);



double norm
(
    double, /* Value */
    double, /* Minimam value will be minimal limit */
    double, /* Maximum value will be maximal limit */
    double, /* Minimal limit */
    double,  /* Maximal limit */
    bool = false    /* Cut reusult form 0 to 1 for true */
);



std::string valueToColor
(
    double aValue
);



std::string valueToChar
(
    /* Value */
    double,
    /* Colored */
    bool = true
);



/*
class Math
{
    static double catmull
    (
        double,
        double,
        double,
        double,
        double
    );
}
*/
