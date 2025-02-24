#include <cmath>
#include <iostream>
#include "chain_d1.h"




/*
    Create new item
*/
ChainD1* ChainD1::create()
{
    return new ChainD1();
}





/*
    Create new double value before argument
    A B  *D* E
        C
*/
ChainItemD1* ChainD1::createBefore
(
    double aValue,
    ChainItemD1* aBefore
)
{
    ChainItemD1* result = NULL;
    if( aBefore != NULL )
    {
        result = ChainItemD1::create() -> setValue( aValue );
        insertBefore( ( ChainItem* ) result, ( ChainItem* ) aBefore );
    }
    else
    {
        setCode( "before_is_null" );
    }
    return result;
}



/*
    Create new double value after the argument
    A B  *D* E
        C
*/
ChainItemD1* ChainD1::createAfter
(
    double aValue,
    ChainItemD1* aAfter
)
{
    ChainItemD1* result = NULL;
    if( aAfter != NULL )
    {
        result = ChainItemD1::create() -> setValue( aValue );
        insertAfter( ( ChainItem* ) result, ( ChainItem* ) aAfter );
    }
    else
    {
        setCode( "after_is_null" );
    }
    return result;
}



/*
    Create new last element
*/
ChainItemD1* ChainD1::createLast
(
    double aValue
)
{
    auto result = ChainItemD1::create() -> setValue( aValue );
    insertLast( ( ChainItem* ) result );
    return result;
}




/*
    Create new first element
*/
ChainItemD1* ChainD1::createFirst
(
    double aValue
)
{
    auto result = ChainItemD1::create() -> setValue( aValue );
    insertFirst( ( ChainItem* ) result );
    return result;
}



/*
    Put data in to double buffer
*/
ChainD1* ChainD1::toBuffer
(
    BufferD1* aBuffer
)
{
    aBuffer -> setCount( getCount() );
    auto i = 0;
    loopFront
    (
        [ &i, &aBuffer ]
        ( ChainItemD1* aItem )
        {
            aBuffer -> setValue( i, aItem -> getValue() );
            i++;
            return false;
        }
    );
    return this;
}




/*
    Put data from double buffer in to chain
*/
ChainD1* ChainD1::fromBuffer
(
    BufferD1* aBuffer
)
{
    /* Clear current */
    clear();
    auto c = aBuffer -> getCount();

    /* Fill the chain */
    for( unsigned long long int i = 0 ; i < c; i++ )
    {
        createLast( aBuffer -> getValue( i ));
    }

    return this;
}




/*
    Put data from memory buffer in to chain
*/
ChainD1* ChainD1::fromBuffer
(
    char* aBuffer,  /* Memory buffer */
    size_t aSize    /* Size of buffer in bytes */
)
{
    auto buffer = BufferD1::create() -> setMem( aBuffer, aSize );
    fromBuffer( buffer);
    buffer -> destroy();

    return this;
}



/*
    Return avg( Ni - Ni+1)
    TODO Отладить и проверить для графика тиков
*/
double ChainD1::avgDelta()
{
    double result = 0.0;
    auto count = getCount();

    if( count > 1 )
    {
        double summ = 0.0;
        double last = (( ChainItemD1* )getFirst()) -> getValue();
        loopFront
        (
            [ &summ, &last ]
            ( ChainItemD1* aItem )
            {
                auto current = aItem -> getValue();
                summ += last - current;
                last = current;
                return false;
            },
            getFirst() -> getNext()
        );
        result = summ;// / ( count - 1 );
    }
    return result;
}



/*
    Return avg
*/
double ChainD1::avg()
{
    double result = 0.0;
    auto count = getCount();
    if( count > 1 )
    {
        double summ = 0.0;
        loopFront
        (
            [ &summ ]
            ( ChainItemD1* aItem )
            {
                summ += aItem -> getValue();
                return false;
            }
        );
        result = summ / count;
    }
    return result;
}



/*
    Smoth algorithm on glider average
*/
ChainD1* ChainD1::smoth
(
    double aSmoth,
    ChainD1* aDest
)
{
    auto c = getCount();
    auto smothCount = ( unsigned int )( aSmoth * c );

    /* Check smoth count */
    if( smothCount == 0 )
    {
        smothCount = 1;
    }

    /* Buffer */
    auto buffer = BufferD1::create( c );
    toBuffer( buffer );

    for( unsigned int i = smothCount; i < c; i ++ )
    {
        double sum = 0;
        unsigned int jn = 0;

        for( unsigned int j = 0; j < smothCount; j ++ )
        {
            sum += buffer -> getValue( i - j );
            jn ++;
        }

        /* Calc average and write last value  */
        if( jn == smothCount )
        {
            aDest -> createLast( sum / jn );
        }
    }

    buffer -> destroy();

    return this;
}




/*
    Calculate and return current minimum and maximum value
*/
ChainD1* ChainD1::calcMinMaxY
(
    double&aMin,
    double&aMax
)
{
    aMin = + INFINITY;
    aMax = - INFINITY;

    loopFront
    (
        [
            &aMin,
            &aMax
        ]
        (
            ChainItem* item
        )
        {
            auto itemD1 = (ChainItemD1*)item;
            auto value = itemD1 -> getValue();
            if( value < aMin ) aMin = value;
            if( value > aMax ) aMax = value;
            return false;
        }
    );

    return this;
}



/*
    Calculate sum
*/
double ChainD1::calcSumY()
{
    double result = 0.0;

    loopFront
    (
        [ &result ]
        ( ChainItem* item )
        {
            auto itemD1 = (ChainItemD1*)item;
            result += itemD1 -> getValue();
            return false;
        }
    );

    return result;
}



/*
    Calculate avg
*/
double ChainD1::calcAvgY()
{
    auto c = getCount();
    return c > 0 ? calcSumY() / (double)c : 0.0;
}



/*
    Calculate delta between max and min values
*/
double ChainD1::deltaMinMax()
{
    double minVal;
    double maxVal;
    calcMinMaxY( minVal, maxVal );
    return maxVal - minVal;
}
