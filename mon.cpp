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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setString( aPath, aValue );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif
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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( !aOnce || !list -> getParamList() -> exists( aPath ) )
    {
        list -> getParamList() -> setInt( aPath, aValue );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setDouble( aPath, aValue );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( !aOnce || !list -> getParamList() -> exists( aPath ))
    {
        list -> getParamList() -> setBool( aPath, aValue );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

    return this;
}


/*
    Copy object in to path of mon
*/
Mon* Mon::copyObject
(
    Path aPath,         /* Path of object on mon structure */
    ParamList* aSource  /* Source objet */
)
{
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list
    -> getParamList()
    -> setObject
    (
        aPath,
        ParamList::create() -> copyFrom( aSource )
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setInt
    (
        aPath,
        list  -> getParamList() -> getInt( aPath, 0 ) + aValue
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list  -> getParamList() -> setDouble
    (
        aPath,
        list -> getParamList() -> getDouble( aPath, 0.0 ) + aValue
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

    return this;
}



/*
    Set now in to parameter
*/
Mon* Mon::div
(
    Path aPathDest,
    Path aPath1,
    Path aPath2
)
{
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list  -> getParamList() -> setDouble
    (
        aPathDest,
        list -> getParamList() -> getDouble( aPath1 ) /
        list -> getParamList() -> getDouble( aPath2 )
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

    return this;
}



/*
    Set now in to parameter
*/
Mon* Mon::now
(
    Path aPath,
    bool aInt
)
{
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( aInt )
    {
        list  -> getParamList() -> setInt( aPath, Moment().setNow().get() );
    }
    else
    {
        list  -> getParamList() -> setString( aPath, Moment().setNow().toString() );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

    return this;
}



/*
    Interval trace
*/
Mon* Mon::trace
(
    Path aPath
)
{
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    auto p = list -> getObject( aPath );
    long long last = 0;
    if( p != NULL )
    {
        p -> loop
        (
            [ &last ]
            ( Param* param )
            {
                auto current = param -> getInt();
                if( last != 0 )
                {
                    param -> setInt( current - last );
                }
                last = current;
                return false;
            }
        );
    }

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list  -> getParamList() -> setString
    (
        aPathDest,
        Moment( list  -> getParamList() -> getInt( aPath1))
        .add( - list  -> getParamList() -> getInt( aPath2 ))
        .intervalToString()
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

    return this;
}




/*
    Set now in to parameter
*/
Mon* Mon::intervalScale
(
    Path aPathDest,
    Path aPath1,
    Path aPath2,
    long long aScale
)
{
    list  -> getParamList() -> setDouble
    (
        aPathDest,
        Moment( list  -> getParamList() -> getInt( aPath1 ))
        .add( - list  -> getParamList() -> getInt( aPath2 ))
        .scale( aScale )
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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setInt( aPath, Moment().setNow().get() );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setInt
    (
        aPath,
        Moment().setNow().get() - list -> getInt( aPath, 0 )
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    if( aDest.empty() )
    {
        aDest = aSource;
    }

    list -> getParamList() -> setString
    (
        aDest,
        Moment( list -> getInt( aSource )).intervalToString()
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setPath( aPath )
    -> setString( "code", aResult -> getCode() )
    -> setString( "message", aResult -> getMessage() );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setInt
    (
        aDestPath,
        min
        (
            list -> getParamList() -> getInt( aDestPath, LLONG_MAX ),
            list -> getParamList() -> getInt( aSourcePath, LLONG_MAX )
        )
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

    list -> getParamList() -> setInt
    (
        aDestPath,
        max
        (
            list -> getParamList() -> getInt( aDestPath, LLONG_MIN ),
            list -> getParamList() -> getInt( aSourcePath, LLONG_MIN )
        )
    );

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
    #ifdef THREAD_PROTECTED
        lock();
    #endif

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

    #ifdef THREAD_PROTECTED
        unlock();
    #endif

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
