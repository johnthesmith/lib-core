#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "console.h"

using namespace std;


/*
    Enumeration defining different parameter types
*/
enum ParamType
{
    KT_UNKNOWN,   /* 0 Unknown data type */
    KT_BOOL,      /* 1 Boolean data type */
    KT_STRING,    /* 2 String data type */
    KT_DATA,      /* 3 Data type for representing binary data */
    KT_INT,       /* 4 Integer data type */
    KT_DOUBLE,    /* 5 Double data type */
    KT_OBJECT,    /* 6 Object data type (structures, classes, etc.) */
    KT_ARRAY,     /* 7 Array data type */
    KT_NULL       /* 8 Null data type (no value) */
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
    /* Delimiter */
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
    Convert double to string
*/
string toString
(
    /* Value for converting */
    long long int,
    /* Color */
    bool = false,
    /* Minimum width with leading zeros (0 = no padding) */
    int = 0
);



/*
    Convert double to string
*/
string toString
(
    /* Value for converting */
    double,
    /* precision */
    unsigned short int = 6,
    /* fixed Zero chars after point */
    DoubleFormat = DF_FIXED,
    /* color */
    bool = false
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


