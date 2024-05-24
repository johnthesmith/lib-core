#pragma once

static const double EPSILON_D = 1e-9;


/*
    Value normalization
*/
double norm
(
    double,         /* Value */
    double,         /* Minimam value will be 0.0 */
    double,         /* Maximum value will be 1.0 */
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
