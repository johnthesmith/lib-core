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

    return result;
}







/*
    Setters and getters
*/



/*
    Return maximum value
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
    int aScreenSize
)
{
    double totalSumm = 0.0;
    long long totalCount = 0;
    double currentMinY = minY;
    double currentMaxY = maxY;

    if( autoMinY )
    {
        currentMinY = + INFINITY;
        currentMaxY = - INFINITY;
    }

    /* Define result stream */
    stringstream result;

    if( aScreenSize > 0 )
    {
        auto dataCount = getCount();

        /* Move data in to indexed buffer */
        vector<double> data( dataCount );
        loopFront
        (
            [
                this,
                &data,
                &totalSumm,
                &totalCount,
                &currentMinY,
                &currentMaxY
            ]
            ( ChainItem * item )
            {
                double value = (( ChainItemD1* )item ) -> getValue();
                totalSumm += value;
                data[ totalCount ] = value;
                totalCount++;

                /* Calculate min and max */
                if( autoMinY )
                {
                    currentMinY = std::min( currentMinY, value );
                    currentMaxY = std::max( currentMaxY, value );
                }

                return false;
            }
        );

        /* Get step */
        double step = (double)dataCount / aScreenSize;

        /* Define the screen array */
        auto screen = vector <double> ( aScreenSize );

        /* Loop for screen elements */
        for (int i = 0; i < aScreenSize; ++i)
        {
            double fromPoint = i * step;
            double toPoint = (i + 1) * step;
            size_t fromDataIndex = (size_t) fromPoint;
            size_t toDataIndex = std::min((size_t) toPoint, data.size());

            double sum = 0.0;
            double totalWeight = 0.0;

            for (size_t j = fromDataIndex; j <= toDataIndex; ++j)
            {
                double segmentStart = std::max(fromPoint, (double) j);
                double segmentEnd = std::min(toPoint, (double)(j + 1));
                double weight = segmentEnd - segmentStart;

                if( j < dataCount )
                {
                    sum += weight * data[j];
                    totalWeight += weight;
                }
            }

            /* Calculate average */
            double average = ( totalWeight > 0.0 ) ? ( sum / totalWeight ) : 0.0;

            /* Store avg into array */
            screen[i] = average;
        }

        /* Build the chart */
        for( int pos = 0; pos < aScreenSize; pos ++ )
        {
            switch
            (
                (int) norm
                (
                    screen[ pos ],
                    currentMinY,
                    currentMaxY,
                    0.0,
                    8.0,
                    true
                )
            )
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
        << " lst:"
        << ( last == NULL ? INFINITY : last -> getValue() )
        << " min:"
        << currentMinY
        << " max:"
        << currentMaxY
        << " avg:"
        << totalSumm / totalCount
        << " cnt:"
        << totalCount
        ;
    }

    return result.str();
}

