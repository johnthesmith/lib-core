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
    /* Default value */
    long long int aValue
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
Moment& Moment::setNow()
{
    set( now() );
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



/*
    Return true for leap year
*/
bool Moment::isLeapYear()
{
    auto year = getYear();
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}



/*
    Return number of year
    TODO
*/
int Moment::getYear()
{
    // int days = value / DAY;
    return 0;//1970 + (int) ( (double)days / 365.2425);
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



///*
//
//*/
//Moment operator=
//(
//    const Moment& a,     /* First operand */
//    const Moment& b      /* First operand */
//)
//{
//    a.value = b.value;
//    return a;
//}
