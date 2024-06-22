#include <cmath>
#include <iostream>

#include "buffer_d1.h"

using namespace std;



/*
    Constructor
*/
BufferD1::BufferD1
(
    long long int aCount
)
{
    setCount( aCount );
}



/*
    Destructor
*/
BufferD1::~BufferD1()
{
    if( !foreign )
    {
        setCount( 0 );
    }
}



/*
    Create new buffer
*/
BufferD1* BufferD1::create
(
    long long int aCount
)
{
    return new BufferD1( aCount );
}



/*
    Delete exists buffer
*/
void BufferD1::destroy()
{
    delete this;
}



/*
    Create new last element
*/
BufferD1* BufferD1::setValue
(
    long long int aIndex,
    double aValue
)
{
    if( aIndex >= 0 && aIndex < count )
    {
        buffer[ aIndex ] = aValue;
    }
    else
    {
        setCode( "IndexOutOfRange" );
    }
    return this;
}



/*
    Get value by index
*/
double BufferD1::getValue
(
    long long int aIndex
)
{
    return
    aIndex >= 0 && aIndex < count
    ? buffer[ aIndex ]
    : 0.0;
}



/*
    Return buffer address
*/
double* BufferD1::getBuffer()
{
    return buffer;
}



/*
    Return count of items
*/
BufferD1* BufferD1::setCount
(
    long long int aCount
)
{
    if( aCount != count && !foreign )
    {
        /* Delete old buffer if exists */
        if( buffer != NULL )
        {
            delete [] buffer;
            buffer = NULL;
        }

        /* Reallocate new buffer for count more 0 */
        if( aCount > 0 )
        {
            buffer = new double[ aCount ];
        }
        count = aCount;
    }

    return this;
}



/*
    Return count of items
*/
long long int BufferD1::getCount()
{
    return count;
}



/*
*/
BufferD1* BufferD1::setMem
(
    char* aMemBuffer,
    size_t aMemSize
)
{
    if( count == 0 )
    {
        buffer = ( double* ) aMemBuffer;
        count = aMemSize / sizeof( double );
        foreign = true;
    }
    return this;
}



/*
    Return memory buffer
*/
char* BufferD1::getMemBuffer()
{
    return ( char* ) buffer;
}



/*
    Return size of bufer
*/
size_t BufferD1::getMemSize()
{
    return count * sizeof( double );
}



/*
    Dump to strout
*/
BufferD1* BufferD1::dump
(
    unsigned long long int aFrom,
    unsigned long long int aCount
)
{
    auto f = fmax( aFrom, 0 );
    auto c = fmin( count, aCount == 0 ? count : ( f + aCount ));
    for( auto i = f; i < c; i++ )
    {
        cout << i << ":" << getValue( i ) << "\n";
    }
    return this;
}
