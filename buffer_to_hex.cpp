#include <sstream>
#include <iomanip>
#include "buffer_to_hex.h"
#include "console.h"


string bufferToHex
(
    const void*       aBuffer,
    const size_t      aSize,
    const string      aHexDelimiter,
    const string      aLineDelimiter,
    const string      aGroupDelimiter,
    const short int   aCountInLine,
    const short int   aCountInGroup
)
{
    string result = "";
    if( aSize != 0 )
    {
        stringstream stream;
        for( size_t i = 0; i < aSize; i++ )
        {
            stream
            << hex
            << setw(2)
            << setfill( '0' )
            << (int) (( unsigned char* ) aBuffer )[ i ]
            << INK_GREEN
            <<
            (
                (( unsigned char* ) aBuffer )[ i ] > 31 &&
                (( unsigned char* ) aBuffer )[ i ] < 128
                ? (( char* ) aBuffer )[ i ]
                : ' '
            )
            << INK_DEFAULT
            <<
            (
                ( ( i + 1 ) % aCountInGroup == 0 )
                ? aGroupDelimiter
                :
                (
                    ( ( i + 1 ) % aCountInLine == 0 )
                    ? aLineDelimiter
                    : aHexDelimiter
                )
            );
        }

        if( aSize % aCountInLine != 0 )
        {
            stream <<  aLineDelimiter;
        }

        result = stream.str();
    }
    return result;
}
