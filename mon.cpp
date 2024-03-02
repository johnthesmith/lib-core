#include "mon.h"



/*
    Constructor
*/
Mon::Mon
(
    string aFile
)
{
    file = aFile;
    list = ParamList::create();
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
    if( !aOnce || !list -> exists( aPath ))
    {
        list -> setString( aPath, aValue );
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
    if( !aOnce || !list -> exists( aPath ) )
    {
        list -> setInt( aPath, aValue );
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
    if( !aOnce || !list -> exists( aPath ))
    {
        list -> setDouble( aPath, aValue );
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
    if( !aOnce || !list -> exists( aPath ))
    {
        list -> setBool( aPath, aValue );
    }
    return this;
}



/*
    Value increment
*/
Mon* Mon::addInt
(
    Path aPath,
    long long int aValue
)
{
    list -> setInt
    (
        aPath,
        list -> getInt( aPath, 0 ) + aValue
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
    list -> setDouble
    (
        aPath,
        list -> getDouble( aPath, 0.0 ) + aValue
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
    auto handle = fopen( file.c_str(), "w" );
    if( handle == NULL )
    {
        setResult( "mon_file_open_error" ) -> getDetails() -> setString( "file", file );
    }
    else
    {
        /* Write to file */
        auto buff = list -> toString();
        fwrite( buff.c_str(), buff.length(), 1, handle );
        fclose( handle );
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
