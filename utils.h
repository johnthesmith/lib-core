#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

using namespace std;


/*
    Enumeration defining different parameter types
*/
enum ParamType
{
    KT_UNKNOWN,   /* Unknown data type */
    KT_BOOL,      /* Boolean data type */
    KT_STRING,    /* String data type */
    KT_DATA,      /* Data type for representing binary data */
    KT_INT,       /* Integer data type */
    KT_DOUBLE,    /* Double data type */
    KT_OBJECT,    /* Object data type (structures, classes, etc.) */
    KT_ARRAY,     /* Array data type */
    KT_NULL       /* Null data type (no value) */
};



/*
    Return now value in microseconds
*/
long long now();



const vector<string> explode
(
    const string&,
    const string&,
    const bool = true,
    const long unsigned int = 0
);




/*
    Implode the vector string
*/
string implode
(
    const vector<string>,
    const string&
);



bool mkDir
(
    string,         /* Path */
    int = S_IRWXU   /* Right */
);



bool fileExists
(
    string
);



long int fileLastUpdate
(
    string
);



/*
    Check file update
*/
bool checkFileUpdate
(
    const string,   /* File name */
    long int&       /* Last moment update. Will be updated if file was chenged */
);



long int fileSize
(
    string
);



bool checkPath
(
    const string
);



string getPath
(
    const string
);



vector <string> getFileList
(
    const string
);



/*
    Return file extention
*/
string getFileExt
(
    const string
);



/*
    Return type by string
*/
ParamType getType
(
    string
);



/*
    Convert hex string to integer
*/
unsigned int hexToInt
(
    string a
);



/*
    Decode byte to utf length
*/
unsigned char utfLength
(
    const unsigned char
);



/*
    Return UTF symbol from string as string symbol
    funcation may return one or more bytes
*/
string utfSimbol
(
    const string,           /* string Utf8*/
    const unsigned int,     /* index in string */
    const unsigned char = 0 /* length from utfLength */
);



/*
    Return count of UTF symbols
*/
unsigned int utfCount
(
    const string a
);



/*
    Convert string to lower
*/
void toLowerCase
(
    string&
);




void b
(
    string = "break"
);



void b
(
    double = 0
);



/******************************************************************************
    Convertors
*/

enum DoubleFormat
{
    DF_MIXED,   /* 0, 0.1, 0-10 etc */
    DF_SCIENT,  /* -0.0-e0  */
    DF_FIXED    /* -0.000000 */
};



/*
    Convert char memory to string
*/
string toString
(
    /* Buffer */
    char*,
    /* Size of buffer */
    unsigned long long int
);



/*
    Counvert double to string
*/
string toString
(
    /* Value for converting */
    double,                     /* value */
    unsigned short int = 6,     /* precision */
    DoubleFormat = DF_FIXED     /* fixed Zero chars after point */
);



bool toBool
(
    string
);



long long int toInt
(
    string
);



double toDouble
(
    string
);


