#include <cmath>
#include <sstream>
#include <iostream>
#include "chart_data.h"
#include "math.h"



using namespace std;



/*
    Create new item
*/
ChartData* ChartData::create()
{
    return new ChartData();
}



/*
    Destroy the object
*/
void  ChartData::destroy()
{
    clear();
    delete this;
}




/*
    Push new element
*/
ChainItemD1* ChartData::createLast
(
    double a
)
{
    auto result = ChainD1::createLast( a );

    if( getCount() > maxCount && maxCount != 0 )
    {
        deleteFirst();
    }

    /* automaticaly rescale */
    if( autoMinY && a < minY )
    {
        minY = a;
    }

    if( autoMaxY && a > maxY )
    {
        maxY = a;
    }

    return result;
}







/*
    Setters and getters
*/



/*
    Return first element
*/
double ChartData::getMaxY()
{
    return maxY;
}



/*
    Return first element
*/
ChartData* ChartData::setMaxY
(
    double a
)
{
    maxY = a;
    return this;
}



/*
    Return first element
*/
double ChartData::getMinY()
{
    return minY;
}



/*
    Return first element
*/
ChartData* ChartData::setMinY
(
    double a
)
{
    minY = a;
    return this;
}



/*
    Return first element
*/
long long int ChartData::getMaxCount()
{
    return maxCount;
}



/*
    Return first element
*/
ChartData* ChartData::setMaxCount
(
    long long int a
)
{
    maxCount = a;
    return this;
}




/*
    Return chart like string
    > 0123456789 <
     / \
    /   \
   |*****|*****|*****|*****|***...***|
      0     1     2     3      ...9
*/
string ChartData::toString
(
    int aSize
)
{
    stringstream result;
    if( aSize > 0 )
    {
        auto current = (ChainItemD1*) getFirst();
        auto shift = ( double )getCount() / ( double ) aSize;
        double accum = 0;

        for( unsigned long long int pos = 0; pos < aSize; pos ++ )
        {
            double average = 0;
            accum += shift;

            if( current != NULL )
            {
                if( accum < 1 )
                {
                    average = current -> getValue();
                }
                else
                {
                    auto countItems = floor( accum );
                    for( unsigned long long i = 0; i < countItems; i ++ )
                    {
                        average = max( average, current -> getValue());
                        current = (ChainItemD1*) current -> getNext();
                    }
                    accum = accum - countItems;
                }
            }

            switch( (int) norm( average, minY, maxY, 0.0, 8.0, true ) )
            {
                case 0: result << " "; break;
                case 1: result << "▁"; break;
                case 2: result << "▂"; break;
                case 3: result << "▃"; break;
                case 4: result << "▄"; break;
                case 5: result << "▅"; break;
                case 6: result << "▆"; break;
                case 7: result << "▇"; break;
                case 8: result << "█"; break;
                default: result << "*"; break;
            }
        }

        auto last = ( ChainItemD1* ) getLast();
        result
        << " last:"
        << ( last == NULL ? 0.0 : last -> getValue() )
        << " min:"
        << minY
        << " max:"
        << maxY
        << " count:"
        << getCount()
        ;
    }

    return result.str();
}

