#include <time.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "moment.h"
#include "utils.h"


/**********************************************************************
    Construction
*/


/*
    Constructor
*/
Moment::Moment
(
    long long aValue
)
{
    set( aValue );
}



/**********************************************************************
    Methods
*/



/*
    Set now in to value
*/
Moment& Moment::now()
{
    /* TODO use the utils::now */

    struct timespec Current;

    set
    (
        clock_gettime( CLOCK_REALTIME, &Current ) == 0
        ? Current.tv_sec * 1000000 + Current.tv_nsec / 1000
        : 0
    );

    return *this;
}



/*
    Return moment to string
*/
string Moment::toString()
{
    time_t time = (long long ) get() / SECOND;
    auto mcs = get() % SECOND;

    char timeString[ size( "yyyy-mm-dd hh:mm:ss.uuuuuu" )];
    strftime
    (
        data( timeString ),
        size( timeString ),
        "%F %T",
        gmtime( &time )
    );

    /* Return result */
    stringstream ss;
    ss << timeString << '.' << std::setw( 6 ) << setfill( '0' ) << mcs;
    return ss.str();
}



/*
    Return delta between this moment and arguments
*/
long long Moment::delta
(
    Moment& aMoment
)
{
    return aMoment.get() - get();
}



/*
    Return string delta between this moment and arguments
*/
string Moment::intervalToString()
{
    auto d = get();

    auto mcs = d % MILLISECOND;
    auto mls = ( d / MILLISECOND ) % 1000;
    auto sec = ( d / SECOND ) % 60;
    auto min = ( d / MINUTE ) % 60;
    auto hor = ( d / HOUR ) % 24;
    auto day = ( d / DAY ) % 30;
    auto mon = ( d / MONTH ) % 365;
    auto yer = d / YEAR;

    stringstream result;

    if( yer != 0 ) result << yer << "year ";
    if( mon != 0 ) result << mon << "month ";
    if( day != 0 ) result << day << "day ";
    if( hor != 0 ) result << hor << "hour ";
    if( min != 0 ) result << min << "min ";
    if( sec != 0 ) result << sec << "sec ";
    if( mls != 0 ) result << mls << "mls ";
    if( mcs != 0 ) result << mcs << "mcs ";

    return result.str();
}



/*
    Add value
*/
Moment& Moment::add
(
    long long aValue
)
{
    set( get() + aValue );
    return *this;
}



/**********************************************************************
    Seters and getters
*/

/*
    Get value of moment
*/
long long Moment::get()
{
    return value;
}



/*
    Return value of moment
*/
Moment& Moment::set
(
    long long aValue  /* Value */
)
{
    value = aValue;
    return *this;
}



/******************************************************************************
*/

/*
    Plus Operator r = a + b
*/
Moment operator+
(
    const Moment& a,    /* First operand */
    const Moment& b     /* Second operand */
)
{
    return Moment
    (
        a.value + b.value
    );
}



/*
    Plus Operator r = a - b
*/
Moment operator-
(
    const Moment& a,    /* First operand */
    const Moment& b     /* Second operand */
)
{
    return Moment
    (
        a.value - b.value
    );
}
