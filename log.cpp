#include <string>
#include <stack>
#include <iostream>
#include <sstream>

#include "log.h"
#include "str.h"
#include "utils.h"


#include <thread>

using namespace std; // для того что бы std:: не приходилось каждый раз писать



Log::Log()
{
    beginStack = new stack <long long>;
}


/*
    Деструктор
*/
Log::~Log()
{
    lineEnd();

    if( isOpen() )
    {
        close();
    }
    delete beginStack;
}



/*
    Construct
*/
Log* Log::create()
{
    return new Log();
}



/*
    Destroy the log
*/
void Log::destroy()
{
    delete this;
}



/*
    проверка открыт ли Лог файл
*/
bool Log::isOpen()
{
    return fileHandle != NULL;
}



/*
    Открыть логфайл
*/
Log* Log::open()
{
    if( !isOpen() && fileName != "" )
    {
        if( checkPath( getPath( fileName )))
        {
            fileHandle = fopen( fileName.c_str(), "a" );
            if( !isOpen() )
            {
                cout << "Log file open error for " << fileName << endl;
            }
        }
        else
        {
            cout << "Log file path check error for " << fileName << endl;
        }
    }
    return this;
}



/*
    Закрытие лога
*/
Log* Log::close()
{
    if( isOpen() )
    {
        fclose( fileHandle );
        fileHandle = NULL;
    }
    return this;
}



/*
    Получить статус активности вывода в логфайл
*/
bool Log::getEnabled()
{
    return enabled;
}



/*
    Установить активность вывода в логфайл
*/
Log* Log::setEnabled
(
    bool a
)
{
    enabled = a;
    return this;
}



Log* Log::write
(
    string a  /* String for log */
)
{
    if( enabled )
    {
        buffer.append( a );
    }
    return this;
}



/*
    Вывод символа завершения стоки
*/
Log* Log::eol()
{
    write( string( 1, cEOL ));
    return this;
}



/*
    Вывод пробела
*/
Log* Log::space()
{
    write(" ");
    return this;
}



/*
    Вывод текста в лог
*/
Log* Log::text
(
    string a    /* Значение */
)
{
    if( width > 0 )
    {
        a = strAlign( a, align, width );
    }

    write( a );

    return this;
}



/*
    Установка ширины вывода текста
*/
Log* Log::setWidth
(
    int a
)
{
    width = a;
    return this;
}



/*
    Вывод значений
*/

/*
    Вывод логического значения в лог
*/
Log* Log::value
(
    bool a
)
{
    if( a )
    {
        text( trueValue );
    }
    else
    {
        text( falseValue );
    }
    return this;
}



/*
    Вывод строки в лог
*/
Log* Log::value
(
    char* a /* Выводимое значение */
)
{
    text( string( a ) );
    return this;
}



/*
    Вывод целочисленного занчения в лог
*/
Log* Log::value
(
    int a
)
{
    text( to_string( a ) );
    return this;
}




/*
    Вывод целочисленного занчения в лог
*/
Log* Log::value
(
    unsigned int a
)
{
    text( to_string( a ) );
    return this;
}



Log* Log::value
(
    long long a
)
{
    text( to_string( a ) );
    return this;
}



Log* Log::value
(
    float a
)
{
    text( to_string( a ) );
    return this;
}



Log* Log::value
(
    double a
)
{
    pushColor();
    if( a > 0 )
    {
        setColor( colorValuePos );
    }
    else if( a < 0 )
    {
        setColor( colorValueNeg );
    }
    else
    {
        setColor( colorValueZero );
    }

    text( toString( a, 10 ) );
    popColor();
    return this;
}



Log* Log::value
(
    string a
)
{
    text( a );
    return this;
}



/*
    Вывод целочисленного занчения в лог формате [ int title:value ]
*/
Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    string aValue   /* Value */
)
{
    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "string " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value ( aValue );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    int aValue      /* Value */
)
{
    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "int " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value ( aValue );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    unsigned int aValue      /* Value */
)
{
    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "int " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value ( aValue );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    long long aValue      /* Value */
)
{
    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "long long " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value ( aValue );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    void* aValue    /* Value */
)
{
    stringstream ss;
    ss << aValue;

    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "adr " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value ( ss.str() );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::prm
(
    string aTitle,  /* Title for parameter */
    double aValue      /* Value */
)
{
    pushColor();
    setColor( colorLabel );
    text( "[" );
    text( "double " );
    setColor( colorInfo );
    text( aTitle );
    space();
    setColor( colorValue );
    value( aValue );
    setColor( colorLabel );
    text( "]" );
    popColor();

    return this;
}



Log* Log::print
(
    string a    /* Message */
)
{
    begin();
    lineEnd();
    pushColor();
    setColor( colorInfo );
    write( a );
    popColor();
    end();
    return this;
}



/******************************************************************************
    Line contrl
*/

Log* Log::lineBegin
(
    LogRecord type
)
{
    lineEnd();

    /* Collect the line begin moment */
    auto momentCurrent = now();
    long long delta = momentLineBegin > 0 ?  momentCurrent - momentLineBegin : 0;
    momentLineBegin = momentCurrent;

    string Tab = "";
    Tab.resize( depth * tabSize, tabChar );

    typeLine = type;
    string charLine;

    switch( type )
    {
        case LOG_DUMP:
        case LOG_NONE:
            colorLine = INK_DEFAULT;
            charLine = "";
        break;
        case LOG_BEGIN:
            colorLine = colorJob;
            charLine = ">";
        break;
        case LOG_END:
            colorLine = colorJob;
            charLine = "<";
        break;
        case LOG_WARNING:
            colorLine = colorWarning;
            charLine = "!";
        break;
        case LOG_TRACE:
            colorLine = colorTrace;
            charLine = "~";
        break;
        case LOG_DEBUG:
            colorLine = colorDebug;
            charLine = "#";
        break;
        case LOG_INFO:
            colorLine = colorInfo;
            charLine = "i";
        break;
        case LOG_ERROR:
            colorLine = colorError;
            charLine = "X";
        break;
    }

    setAlign( ALIGN_RIGHT );
    setWidth( 8 );
    setColor( colorLabel );
    value( delta );
    space();
    setAlign();
    setWidth( 0 );
    setColor( colorLine );
    write( charLine );
    space();
    setColor( tabColor );
    write( Tab );
    setColor( colorLine );

    return this;
}



/*
    Безусловено завершение линии
    Если линия была открыта то буфер выгрушается в направление
*/
Log* Log::lineEnd()
{
    /* Вывод строки */
    if( enabled && typeLine != LOG_NONE )
    {
        /* Close current line */
        typeLine = LOG_NONE;

        /* Draw enter */
        eol();

        if( trapBuffers.empty() || trapBuffers.top().empty() )
        {
            flush( buffer );
        }
        else
        {
            auto &currentTrap = trapBuffers.top();
            currentTrap.push_back( buffer );
        }
        buffer = "";
    }
    return this;
}



/*
    Flush buffer to out
*/
Log* Log::flush
(
    string aBuffer
)
{
    if( fileName == "" )
    {
        /* Write to console */
        cout << aBuffer;
    }
    else
    {
        /* Write to file */
        if( !isOpen() ) open();
        fwrite( aBuffer.c_str(), aBuffer.length(), 1, fileHandle );
        close();
    }
    return this;
}






/******************************************************************************
    Log lines
*/

Log* Log::begin()
{
    return begin( "" );
}



Log* Log::begin
(
    string message
)
{
    lineBegin( LOG_BEGIN );

    beginStack -> push( momentLineBegin );

    write( message );

    depth++;

    return this;
}



Log* Log::end()
{
    return end( "" );
}



Log* Log::end
(
    string aMessage
)
{
    depth--;

    if( depth < 0 )
    {
        depth=0;
    }

    lineBegin( LOG_END );
    write( aMessage );

    if( beginStack -> size() > 0 )
    {
        long long momentBegin = beginStack -> top();
        beginStack -> pop();
        long long delta = momentLineBegin - momentBegin;
        pushColor();
        setColor( colorLabel );
        value( delta );
        popColor();
    }
    else
    {
        pushColor();
        setColor( colorError );
        text( "Log heracly error" );
        eol();
        popColor();
    }

    return this;
}



Log* Log::debug()
{
    return debug( "" );
}



Log* Log::debug
(
    string a    /* Message */
)
{
    lineBegin( LOG_DEBUG );
    write( a );
    return this;
}



Log* Log::trace()
{
    return trace( "" );
}



Log* Log::trace
(
    string a    /* Message */
)
{
    lineBegin( LOG_TRACE );
    write( a );
    return this;
}



Log* Log::info()
{
    return info( "" );
}


Log* Log::info
(
    string a    /* Message */
)
{
    trapDump();
    lineBegin( LOG_INFO );
    write( a );
    return this;
}



Log* Log::warning()
{
    return warning( "" );
}



Log* Log::warning
(
    string a    /* Message */
)
{
    trapDump();
    lineBegin( LOG_WARNING );
    write( a );
    return this;
}



Log* Log::error()
{
    return error( "" );
}



Log* Log::error
(
    string a    /* Message */
)
{
    trapDump();
    lineBegin( LOG_ERROR );
    write( a );
    return this;
}



/******************************************************************************
*/


// TLog &Param(string AName, int AValue)
// {
//  Write(AName+"="+to_string(AValue));
//  return **this;
// }
//
//
//
// TLog &Param(string AName, unsigned AValue)
// {
//  Write(AName+"="+to_string(AValue));
//  return **this;
// }
//
//
//
// TLog &Param(string AName, long long AValue)
// {
//  Write(AName+"="+to_string(AValue));
//  return **this;
// }
//
//
// TLog &Param(string AName, unsigned long long AValue)
// {
//  Write(AName+"="+to_string(AValue));
//  return **this;
// }
//
//
//
// TLog &Param(string AName, double AValue)
// {
//  Write(AName+"="+to_string(AValue));
//  return **this;
// }
//
//
//
// // Вывод параметра в формате AName=AValue
// TLog &Param(string AName, string AValue)
// {
//  Write(AName+"="+AValue);
//  return **this;
// }



/******************************************************************************
    Service
*/


/*
    Установка умолчального выравнивания по левому краю
*/
Log* Log::setAlign()
{
    return setAlign( ALIGN_LEFT );
}



/*
    Установка умолчального выравнивания по левому краю
*/
Log* Log::setAlign( TextAlign a )
{
    align = a;
    return this;
}



/*
    Возвращает значение выравнивания
*/
TextAlign Log::getAlign()
{
    return align;
}



/*
    Установка умолчального цвета
*/
Log* Log::setColor()
{
    setColor( INK_DEFAULT );
    return this;
}



/*
    Установка специфического цвета
*/
Log* Log::setColor
(
    string a    /* Цветовая константа */
)
{
    color = a;  /* Сохраняем цвет */
    if( getColored() )
    {
        write( a );         /* Изменяем цвет на консоли */
    }
    return this;
}



bool Log::getColored()
{
    return colored;
}



Log* Log::setColored( bool value )
{
    colored = value;
    return this;
}



Log* Log::pushColor()
{
    colorStack.push( color );
    return this;
}



Log* Log::popColor()
{
    setColor( colorStack.top());
    colorStack.pop();
    return this;
}



Log* Log::pushAlign()
{
    alignStack.push( color );
    return this;
}



Log* Log::popAlign()
{
    setColor( colorStack.top());
    alignStack.pop();
    return this;
}



/*
    Set log file name
*/
Log* Log::setFileName
(
    string aFileName
)
{
    fileName = aFileName;
    if( isOpen() )
    {
        close();
    }
    open();
    return this;
}



/*
    Return log file name
*/
string Log::getFileName()
{
    return fileName;
}




/*
    Clone log settings
*/
Log* Log::clone
(
    Log* aSource
)
{
    setEnabled( aSource -> getEnabled());
    setColored( aSource -> getColored());
    setFileName( aSource -> getFileName());
    return this;
}




/*
    Dump param list to log
*/
Log* Log::dump
(
    ParamList* aParamList,
    string  aSection
)
{
    return dumpInternal( aParamList, aSection, 0 );
}




Log* Log::dumpInternal
(
    ParamList*  aParamList,
    string      aSection,
    int         depth
)
{
    int c = aParamList -> getCount();
    begin( aSection );
    for( int i = 0; i < c; i++ )
    {
        auto p = aParamList -> getByIndex( i );

        switch( p -> getType() )
        {
            case KT_OBJECT:
                if( p -> getObject() != NULL )
                {
                    dumpInternal
                    (
                        p -> getObject(),
                        p -> getName( to_string( i ) ),
                        depth + 1
                    );
                }
            break;
            case KT_DATA:
            {
                auto s  = p -> getSize();
                trace();
                pushColor();
                setColor( colorLabel );
                text( p -> getNameOfType() );
                setColor( colorInfo );
                space();
                text( p -> getName() );
                setColor( colorLabel );
                text( " size " );
                setColor( colorValue );
                value( ( long long int ) s );
                popColor();
            }
            break;
            default:
                auto s  = p -> getSize();
                trace();
                pushColor();
                setColor( colorLabel );
                text( p -> getNameOfType() );
                setColor( colorInfo );
                space();
                text( p -> getName() );
                setColor( colorLabel );
                text( " = " );
                setColor( colorValue );
                value( aParamList -> getString( i ) );
                setColor( colorLabel );
                text( " size " );
                setColor( colorValue );
                value( ( long long int ) s );
                popColor();
            break;
        }
    };
    end();

    return this;
}



/*
    Trap on
*/
Log* Log::trapOn()
{
    if( trapEnabled )
    {
        lineEnd();
        trapBuffers.push( vector <string> { "" } );
    }
    return this;
}



/*
    Trap off
*/
Log* Log::trapOff()
{
    if( !trapBuffers.empty() )
    {
        lineEnd();
        trapBuffers.pop();
    }
    return this;
}



/*
    Trap dump
*/
Log* Log::trapDump()
{
    if( !trapBuffers.empty() )
    {
        auto &trapBuffer = trapBuffers.top();
        flush( implode( trapBuffer, "" ));
        trapBuffer.clear();
    }
    return this;
}



/*
    Set log trap
*/
Log* Log::setTrapEnabled
(
    bool a
)
{
    trapEnabled = a;
    return this;
}



/*
    Return trap enabled flag
*/
bool Log::getTrapEnabled()
{
    return trapEnabled;
}



LogRecord Log::logRecordFromString
(
    string a
)
{
    if( a == "NONE" )       return LOG_NONE;
    if( a == "BEGIN" )      return LOG_BEGIN;
    if( a == "END" )        return LOG_END;
    if( a == "TRACE" )      return LOG_TRACE;
    if( a == "DEBUG" )      return LOG_DEBUG;
    if( a == "INFO" )       return LOG_INFO;
    if( a == "ERROR" )      return LOG_ERROR;
    if( a == "WARNING" )    return LOG_WARNING;
    if( a == "DUMP" )       return LOG_DUMP;
    return LOG_NONE;
}



Log* Log::record
(
    LogRecord aRecord,
    string aMessage
)
{
    switch( aRecord )
    {
        default:
        case LOG_NONE       : break;
        case LOG_DUMP       :break;
        case LOG_BEGIN      : begin( aMessage ); break;
        case LOG_END        : end( aMessage ); break;
        case LOG_TRACE      : trace( aMessage ); break;
        case LOG_DEBUG      : debug( aMessage ); break;
        case LOG_INFO       : info( aMessage ); break;
        case LOG_ERROR      : error( aMessage ); break;
        case LOG_WARNING    : warning( aMessage ); break;
    }
    return this;
}
