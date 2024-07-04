#include <climits>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/file.h>

#include "mon.h"
#include "moment.h"



using namespace std;

/*
    Constructor
*/
Mon::Mon
(
    string aFile
)
{
    file = aFile;
    list = Json::create();
}



/*
    Destructor
*/
Mon::~Mon()
{
    list -> destroy();
}



/*
    Construct monitor
*/
Mon* Mon::create
(
    string aFile
)
{
    return new Mon( aFile );
}



/*
    Destroy the monitor
*/
void Mon::destroy()
{
    delete this;
}



/**************************************************************************
    Directives
*/

/*
    Set value
*/
Mon* Mon::setString
(
    Path aPath,     /* Path of value */
    string aValue,  /* Value */
    bool aOnce      /* Once set value */
)
{
    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setString( aPath, aValue );
    }
    return this;
}



/*
    Set value
*/
Mon* Mon::setInt
(
    Path aPath,             /* Path of value */
    long long int aValue,   /* Value */
    bool aOnce              /* Once set value */
)
{
    if( !aOnce || !list -> getParamList() -> exists( aPath ) )
    {
        list -> getParamList() -> setInt( aPath, aValue );
    }
    return this;
}



/*
    Set value
*/
Mon* Mon::setDouble
(
    Path aPath,     /* Path of value */
    double aValue,  /* Value */
    bool aOnce      /* Once set value */
)
{
    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setDouble( aPath, aValue );
    }
    return this;
}



/*
    Set value
*/
Mon* Mon::setBool
(
    Path aPath,     /* Path of value */
    bool aValue,    /* Value */
    bool aOnce      /* Once set value */
)
{
    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setBool( aPath, aValue );
    }
    return this;
}



/*
    Value increment
*/
Mon* Mon::addInt
(
    Path aPath,
    long long aValue
)
{
    list -> getParamList() -> setInt
    (
        aPath,
        list  -> getParamList() -> getInt( aPath, 0 ) + aValue
    );
    return this;
}



/*
    Value increment
*/
Mon* Mon::addDouble
(
    Path aPath,
    double aValue
)
{
    list  -> getParamList() -> setDouble
    (
        aPath,
        list -> getParamList() -> getDouble( aPath, 0.0 ) + aValue
    );
    return this;
}



/*
    Set now in to parameter
*/
Mon* Mon::now
(
    Path aPath
)
{
    list  -> getParamList() -> setString( aPath, Moment().setNow().toString() );
    return this;
}




/*
    Set now in to parameter
*/
Mon* Mon::interval
(
    Path aPathDest,
    Path aPath1,
    Path aPath2
)
{
    list  -> getParamList() -> setString
    (
        aPathDest,
        Moment( list  -> getParamList() -> getInt( aPath1))
        .add( - list  -> getParamList() -> getInt( aPath2 ))
        .intervalToString()
    );
    return this;
}



/*
    Timer start
*/
Mon* Mon::startTimer
(
    Path aPath
)
{
    list -> getParamList() -> setInt( aPath, Moment().setNow().get() );
    return this;
}



/*
    Stop timer
*/
Mon* Mon::stopTimer
(
    Path aPath
)
{
    list -> getParamList() -> setInt
    (
        aPath,
        Moment().setNow().get() - list -> getInt( aPath, 0 )
    );
    return this;
}



/*
    Conver timer to string
*/
Mon* Mon::timerToString
(
    Path aSource,   /* Source path */
    Path aDest      /* Destination path */
)
{
    if( aDest.empty() )
    {
        aDest = aSource;
    }

    list -> getParamList() -> setString
    (
        aDest,
        Moment( list -> getInt( aSource )).intervalToString()
    );
    return this;
}



/*
    Dump result in to mon
*/
Mon* Mon::dumpResult
(
    Path aPath,
    Result* aResult
)
{
    list -> getParamList() -> setPath( aPath )
    -> setString( "code", aResult -> getCode() )
    -> setString( "message", aResult -> getMessage() );
    return this;
}



/*
    Dump result in to mon
*/
Mon* Mon::minInt
(
    Path aDestPath,
    Path aSourcePath
)
{

    list -> getParamList() -> setInt
    (
        aDestPath,
        min
        (
            list -> getParamList() -> getInt( aDestPath, LLONG_MAX ),
            list -> getParamList() -> getInt( aSourcePath, LLONG_MAX )
        )
    );
    return this;
}



/*
    Dump result in to mon
*/
Mon* Mon::maxInt
(
    Path aDestPath,
    Path aSourcePath
)
{

    list -> getParamList() -> setInt
    (
        aDestPath,
        max
        (
            list -> getParamList() -> getInt( aDestPath, LLONG_MIN ),
            list -> getParamList() -> getInt( aSourcePath, LLONG_MIN )
        )
    );
    return this;
}



/**************************************************************************
    Services method
*/


/*
    Write monitor data in to file
*/
Mon* Mon::flush()
{

    if( !checkPath( getPath( file )))
    {
        setResult( "mon_path_check_error" )
        -> getDetails()
        -> setString( "file", file );
    }
    else
    {
        auto buff = list -> toString();

        auto handle = fopen( file.c_str(), "r+" );
        if( handle == NULL )
        {
            handle = fopen( file.c_str(), "w+" );
        }

        if( handle == NULL )
        {
            setResult( "mon_file_open_error" )
            -> getDetails()
            -> setString( "file", file );
        }
        else
        {
            auto handleno = fileno( handle );
            auto buffsize = buff.length();

            /* Write to file */
            flock( handleno, LOCK_EX );
            ftruncate( handleno, buffsize );
            fseek( handle, 0, SEEK_SET );
            fwrite( buff.c_str(), buffsize, 1, handle );
            flock( handleno, LOCK_UN );
            fclose( handle );
        }
    }
    return this;
}



/*
    Remove Удаляет файл статистики
    TODO
*/
Mon* Mon::drop()
{
    return this;
}



/**************************************************************************
    Setters and geters
*/

/*
    Set file path and name
*/
Mon* Mon::setFile
(
    string aValue /* File name */
)
{
    file = aValue;
    return this;
}



/*
    Return file path and name
*/
string Mon::getFile()
{
    return file;
}
