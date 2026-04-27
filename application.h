#pragma once

#include <string>
#include <functional>
#include <csignal>

#include "result.h"
#include "mon.h"
#include "log_manager.h"
#include "../json/param_list_file.h"



using namespace std;


/* Calback lyambda for Application::lock method */
typedef function <bool ()> LockTerminated;



class Payload;


struct PayloadItem
{
    /* "./processor.so" */
    std::string libraryPath;
    /* dlclose() */
    void* libraryHandle;
    /* payload pointer */
    Payload* instance;
};



class Application : public Result
{
    private:
        bool            terminated          = false;

        ParamList*      cli                 = NULL;
        ParamListFile*  config              = NULL;
        Log*            log                 = NULL;
        LogManager*     logManager          = NULL;
        Mon*            mon                 = NULL;
        bool            configUpdated       = false;
        long int        lastConfigUpdate    = 0;

        /* Массив зарегистрированных сигналов */
        std::vector<int> registered_signals;

        /* Name of payload from config */
        std::map<string, PayloadItem> payloads;
    protected:

        /* Регистрация сигнала для обработки */
        void registerSignal
        (
            int /* Код сигнала */
        );

    public:
        /* Статический указательн на приложение */
        static Application* application;

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
            Prepare configuration for application running
        */
        Application* prepareConfiguration();



        /*
            Run application
        */
        Application* run();



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



        Application* setupSignalHandlers();



        /*
            On signal event handler for overriding in clients
        */
        virtual bool onSignal
        (
            /* Id signal */
            int /* aSignal */ )
        {
            return false;
        }



        inline Application* terminate()
        {
            terminated = true;
            return this;
        }



        /*
            Return get last config update
        */
        inline long int getLastConfigUpdate()
        {
            return lastConfigUpdate;
        }




        /**********************************************************************
            Application events
        */

        /*
            Generate event after config updated
        */
        virtual Application* onConfigUpdated()
        {
            return this;
        }
};
