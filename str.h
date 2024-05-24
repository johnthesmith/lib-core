#pragma once

#include <string>
#include "text_align.h"



using namespace std;


//
///*
//    https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
//*/
//template<typename ... Args>
//std::string strFormat
//(
//    const std::string& format,  /* format template */
//    Args ... args               /* list of arguments */
//)
//{
//    string result;
//    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
//    if( size >= 0 )
//    {
//        std::unique_ptr<char[]> buf( new char[ size ] );
//        snprintf( buf.get(), size, format.c_str(), args ... );
//        result = std::string( buf.get(), buf.get() + size - 1 );
//    }
//    else
//    {
//        result = "";
//    }
//    return result;
//}
//




bool isNum
(
    const string&
);



/*
    строку AValue выровненную до длилны ALength символами AChar по правилу ATextAlign
*/

string strAlign
(
    string,
    TextAlign,
    int /* Length */
);
