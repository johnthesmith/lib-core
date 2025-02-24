#pragma once

#include <string>
#include <functional>

#include "result.h"
#include "log.h"
#include "mon.h"
#include "log_manager.h"
#include "../json/param_list_file.h"
#include "../json/json.h"



using namespace std;


/* Calback lyambda for Application::lock method */
typedef function <bool ()> LockTerminated;


class Application : public Result
{
    private:

        ParamList*      cli                 = NULL;
        ParamListFile*  config              = NULL;
        Log*            log                 = NULL;
        LogManager*     logManager          = NULL;
        Mon*            mon                 = NULL;
        bool            configUpdated       = false;
        long int        lastConfigUpdate    = 0;

    public:

        /*
            Constructor
        */
        Application
        (
            int,        /* cli argumends count */
            char**      /* cli arguments */
        );




        /*
            Destructor
        */
        ~Application();



        /*
            Creator
        */
        static Application* create
        (
            int,        /* cli argumends count */
            char**      /* cli arguments */
        );



        /*
            Destroy
        */
        void destroy();



        /*
            Return config object
        */
        ParamList* getCli();



        /*
            Return config object
        */
        ParamListFile* getConfig();



        /*
            Check update moment of the config file.
            If file was updated, then the config object is rebuilding.
        */
        Application* checkConfigUpdate();


        /**********************************************************************
            Log operations
        */



        /*
            Return Log manager
        */
        LogManager* getLogManager();



        /*
            Create new log
        */
        Log* createThreadLog
        (
            string  /* Log id */
        );



        /*
            Destroy log
        */
        Application* destroyThreadLog();



        /*
            End of thread
        */
        virtual Application* onThreadAfter();



        /*
            Return true if config wile was updated
        */
        bool getConfigUpdated();



        /*
            Return log object
        */
        Log* getLog();



        /*
            Return mon object
        */
        Mon* getMon();



        /*
            Lock process and waiting resolve locker file from user
            https://github.com/johnthesmith/scraps/blob/main/ru/process_locking_for_debug.md
        */
        string lock
        (
            /* Lock file */
            string,
            /* Params */
            ParamList*,
            /* Callback terminated */
            LockTerminated = NULL
        );



        /*
            Return the name of configuraion file
        */
        string getConfigFileName();

};
