/*
    Monitoring
    2024.03.01
*/

#pragma once

#include "result.h"
#include "../json/param_list.h"



using namespace std;



class Mon : public Result
{
    private:

        /* File path and name for the monitoring dump */
        string file         = "";
        /* Monitoring list */
        ParamList*  list    = NULL;

    public:

        /*
            Constructor
        */
        Mon
        (
            string  /* File name */
        );



        /*
            Destructor
        */
        ~Mon();



        /*
            Construct monitor
        */
        static Mon* create
        (
            string  /* File name */
        );



        /*
            Destroy the monitor
        */
        void destroy();



        /**************************************************************************
            Directives
        */

        /*
            Set value
        */
        Mon* setString
        (
            Path,           /* Path of value */
            string,         /* Value */
            bool = false    /* Once set value */
        );



        /*
            Set value
        */
        Mon* setInt
        (
            Path,           /* Path of value */
            long long int,  /* Value */
            bool = false    /* Once set value */
        );



        /*
            Set value
        */
        Mon* setDouble
        (
            Path,           /* Path of value */
            double,         /* Value */
            bool = false    /* Once set value */
        );



        /*
            Set value
        */
        Mon* setBool
        (
            Path,           /* Path of value */
            bool,           /* Value */
            bool = false    /* Once set value */
        );



        /*
            Value increment
        */
        Mon* addInt
        (
            Path,
            long long int = 1
        );



        /*
            Value increment
        */
        Mon* addDouble
        (
            Path,
            double = 1.0
        );



        /**************************************************************************
            Services method
        */


        /*
            Write monitor data in to file
        */
        Mon* flush();



        /*
            Remove Удаляет файл статистики
        */
        Mon* drop();



        /**************************************************************************
            Setters and geters
        */

        /*
            Set file path and name
        */
        Mon* setFile
        (
            string  /* File name */
        );



        /*
            Return file path and name
        */
        string getFile();
};
