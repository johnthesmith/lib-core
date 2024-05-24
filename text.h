#pragma once

#include <sstream>
#include <string>



using namespace std;



class Text :public stringstream
{
    private:

        unsigned short int tabSize = 4;
        unsigned short int depth = 0;
        string lineDelimiter = "\n";
        char tabChar = ' ';
        bool compact = false;

    public:

        /*
            Create new Text object
        */
        static Text* create();



        /*
            Destroy Text object
        */
        void destroy();



        /*
            Add string to text object
        */
        Text* add
        (
            string,         /* Value */
            string = "",    /* Prefix */
            string = ""     /* Suffix */
        );



        /*
            Begin of section with value
        */
        Text* begin
        (
            string = "" /* text */
        );



        /*
            End of section with value
        */
        Text* end
        (
            string = "" /* text */
        );



        /*
            Add new line
        */
        Text* lineBegin();



        /*
            Add line end
        */
        Text* lineEnd();



        /*
            Return string
        */
        string getString();



        /*
            Set line delimiter string
        */
        Text* setLineDelimiter
        (
            string  /* Delimiter string */
        );



        /*
            Set tab character
        */
        Text* setTabChar
        (
            char  /* Tab character */
        );



        /*
            Set tcompact mode
        */
        Text* setCompact
        (
            bool
        );
};
