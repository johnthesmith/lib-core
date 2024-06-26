#include <string>
#include <cstring>
#include "str.h"



using namespace std;



bool isNum
(
    const string& a
)
{
    auto ch = a.c_str();
    for( int i=0, n = strlen( ch ); i < n; i++ )
    {
        if( ch[ i ] < '0' || ch[ i ] > '9') return false;
    }
    return true;
}


/*
    строку AValue выровненную до длилны ALength символами AChar по правилу ATextAlign
*/

string strAlign
(
    string aValue,
    TextAlign aAlign,
    int aLength
)
{
    int delta = aValue.size() - aLength;
    if( delta > 0 )
    {
        int b;
        int e = aLength;
        switch (aAlign)
        {
            default:
            case ALIGN_LEFT     : b = 0; break;
            case ALIGN_RIGHT    : b = delta; break;
            case ALIGN_CENTER   : b = delta / 2; break;
        }
        aValue = aValue.substr( b, e );
    }
    else
    {
        string s = aValue;
        switch ( aAlign )
        {
            default:
            case ALIGN_LEFT:
                aValue.resize( aLength, ' ');
            break;
            case ALIGN_RIGHT:
                aValue.assign( -delta, ' ').append( s );
            break;
            case ALIGN_CENTER:
                aValue.resize( -delta/2, ' ' );
                aValue.append( s ).resize( aLength, ' ' );
            break;
        }
    }
    return aValue;
}
