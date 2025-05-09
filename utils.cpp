#include <time.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <regex>
#include <dirent.h>
#include <sys/stat.h>

#include "utils.h"
#include "math.h"


using namespace std;



/*
    Return now value in microseconds
*/
long long now()
{
    struct timespec Current;
    return
    clock_gettime( CLOCK_REALTIME, &Current ) == 0
    ? Current.tv_sec * 1000000 + Current.tv_nsec / 1000
    : 0;
}



/*
    String utils
*/

const vector<string> explode
(
    const string& aSource,
    const string& aDelimeter,
    const bool aReturnEmpty,
    const long unsigned int aCount
)
{
	string buff{ "" };
	vector<string> v;

    int lDelimeter  = aDelimeter.size();
    int lSource     = aSource.size();

    if( lDelimeter == 0 )
    {
        v.push_back( aSource );
    }
    else
    {
        int equals = 0;
        int b = 0;

    	for
        (
            int i = 0;
            i < lSource && ( aCount == 0 || aCount != v.size() );
            i ++
        )
    	{
    		if( aSource[ i ] == aDelimeter[ equals ] )
            {
                equals++;
                if( equals == lDelimeter )
                {
                    if( aReturnEmpty || i - b - equals > 0 )
                    {
                        v.push_back( aSource.substr( b, i - b - equals + 1 ));
                    }
                    b = i + 1;
                    equals = 0;
                }
            }
            else
            {
                equals = 0;
            }
        }
        if( aReturnEmpty || lSource - b > 0 )
        {
            v.push_back( aSource.substr( b, lSource - b ));
        }
    }

	return v;
}



/*
    Implde the vector string
    TODO need make the one peace of memory.
*/
string implode
(
    const vector<string> aList,
    const string& aDelimeter
)
{
    string result = "";
    int c = aList.size();
    for( int i = 0; i < c; i++ )
    {
        result += aList[ i ];
        if( i != c - 1)
        {
            result += aDelimeter;
        }
    }
    return result;
}



/*
    file utils
*/



/*
    Create directories
*/
bool mkDir
(
    string aPath,
    int aRight
)
{
    auto list = explode( aPath, "/" );
    string path = "";
    for( auto dir:list )
    {
        path += dir + "/";
        mkdir( path.c_str(), aRight );
    }
    return true;
}



/*
    return true if file exists
*/
bool fileExists
(
    string a
)
{
    struct stat result;
    return stat( a.c_str(), &result ) == 0 ;
}



/*
    Check file update
*/
long int fileLastUpdate
(
    string a
)
{
    struct stat result;
    long int r = 0;
    if( stat( a.c_str(), &result ) == 0 )
    {
        r = result.st_mtime;
    }
    return r;
}



/*
    Check file update
*/
bool checkFileUpdate
(
    const string aFile,
    long int &aLast
)
{
    bool result = false;
    if( fileExists( aFile ) )
    {
        auto last = fileLastUpdate( aFile );
        result = last != aLast;
        if( result )
        {
            aLast = last;
        }
    }
    return result;
}



/*
    Return file size
*/
long int fileSize
(
    string a
)
{
    struct stat result;
    long int r = -1;
    if( stat( a.c_str(), &result ) == 0 )
    {
        r = result.st_size;
    }
    return r;
}




bool checkPath
(
    const string aPath
)
{
    bool result = true;
    if( aPath != "" && !fileExists( aPath ))
    {
        mkDir( aPath );
        result = fileExists( aPath );
    }
    return result;
}



string getPath
(
    const string a
)
{
    auto path = explode( a, "/" );
    path.pop_back();
    return implode( path, "/" );
}



/*
    return list of files
*/
vector <string> getFileList
(
    const string aPath
)
{
    vector <string> result;
    DIR *dir;
    if(( dir = opendir( aPath.c_str() )) != NULL)
    {
        struct dirent *f_cur;

        while (( f_cur = readdir( dir )) != NULL)
        {
            result.push_back( f_cur -> d_name );
        }

        closedir( dir );
    }
    return result;
}



/*
    Return file extention
*/
string getFileExt
(
    const string a
)
{
  return a.substr( a.find_last_of( "." ) + 1 );
}



/*
    Return type by string
*/
ParamType getType
(
    string a
)
{
    ParamType result = KT_STRING;

    if( a == "true" || a == "false" || a == "TRUE" || a == "FALSE" )
    {
        result = KT_BOOL;
    }
    else
    {
        if( a == "null" || a == "NULL" )
        {
            result = KT_NULL;
        }
        else
        {
            if( regex_match( a, regex( "[-+]?([0-9]+)" ) ))
            {
                result = KT_INT;
            }
            else
            {
                if( regex_match( a, regex( "[-+]?([0-9]*[\\.\\,][0-9]+)" ) ))
                {
                    result = KT_DOUBLE;
                }
            }
        }
    }
    return result;
}




unsigned int hexToInt
(
    string a
)
{
    int result;
    sscanf( a.c_str(), "%x", &result );
    return result;
}



/*
    Decode byte to utf length
*/
unsigned char utfLength
(
    const unsigned char a
)
{
    if( ( a & 0b10000000 ) == 0b00000000 ) return 1;
    if( ( a & 0b11100000 ) == 0b11000000 ) return 2;
    if( ( a & 0b11110000 ) == 0b11100000 ) return 3;
    if( ( a & 0b11111000 ) == 0b11110000 ) return 4;
    return 1;
}



/*
    Return UFT-8 symbol from string
*/
string utfSimbol
(
    const string        aString,    /* string Utf8*/
    const unsigned int  aIndex,     /* index in string */
    const unsigned char aLength     /* length from utfLength */
)
{
    return aString.substr( aIndex, aLength == 0 ? utfLength( aString[ aIndex ]) : aLength );
}



/*
    Return count of UTF symbols
*/
unsigned int utfCount
(
    const string a
)
{
    unsigned int result = 0;
    unsigned int i = 0;
    unsigned int c = a.size();
    while( i < c )
    {
        i += utfLength( a[ i ]);
        result++;
    }
    return result;
}



/*
    Convert string to lower
*/
void toLowerCase
(
    string& a
)
{
    auto c = a.length();
    for( size_t i = 0; i < c; i ++ )
    {
        a[ i ] = tolower( a[ i ] );
    }
}




/*
*/
void b
(
    string a
)
{
    cout << a << "\n";
    if( a == "break" )
    {
        exit( 0 );
    }
}



void b
(
    double a
)
{
    cout << a << "\n";
    if( a == 0 )
    {
        exit( 0 );
    }
}



/******************************************************************************
    Convertors
*/



/*
    Convert char memory to string
*/
string toString
(
    /* Buffer */
    char* aBuffer,
    /* Size of buffer */
    unsigned long long int aSize
)
{
    return string( aBuffer, aSize );
}



/*
    Convert double to string
*/
string toString
(
    /* Value for converting */
    double              aValue,
    unsigned short int  aPrecision,
    DoubleFormat        aFormat,
    bool                aColor
)
{
    stringstream s;

    if( aColor )
    {
        if( aValue > EPSILON_D ) s << INK_GREEN;
        else if( aValue < - EPSILON_D ) s << INK_RED;
        else s << INK_GREY;
    }

    switch( aFormat )
    {
        default:
        case DF_FIXED   : s << fixed << setprecision( aPrecision )
                            << setfill( '0' ); break;
        case DF_MIXED   : /* default */ break;
        case DF_SCIENT  : s << scientific; break;
    }

    s << setprecision( aPrecision );

    if( aColor )
    {
        s << abs( aValue );
        s << INK_DEFAULT;
    }
    else
    {
        s << aValue;
    }

    return s.str();
}





/*
    Convert integer to string
*/
string toString
(
    /* Value for converting */
    long long int       aValue,
    /* Color out true or false */
    bool                aColor,
    /* Minimum width with leading zeros (0 = no padding) */
    int                 aWidth
)
{
    stringstream s;

    if( aColor )
    {
        if( aValue > 0 ) s << INK_GREEN;
        else if( aValue < 0 ) s << INK_RED;
        else s << INK_GREY;
    }

    if( aWidth > 0 )
    {
        s << std::setw(aWidth) << std::setfill('0');
    }

    if( aColor )
    {
        s << abs(aValue);
        s << INK_DEFAULT;
    }
    else
    {
        s << aValue;
    }

    return s.str();
}



bool toBool
(
    string a
)
{
    return a == "true" || a == "TRUE";
}



long long int toInt
(
    string a
)
{
    return atol( a.c_str() );
}



double toDouble
(
    string a
)
{
    return atof( a.c_str() );
}
