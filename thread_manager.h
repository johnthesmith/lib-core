#pragma once

#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <tuple>
#include <iostream>
#include <condition_variable>

#include "log_manager.h"
#include "result.h"


using namespace std;



/*
    Handler function
*/
typedef function<void()> ThreadManagerHandler;

/* Predeclaration */
class ThreadManager;


/*
    Thread managers task
*/
class ThreadManagerTask :public Result
{
    private:

        /* Thread manager */
        ThreadManager*          owner;
        /* Thread worker */
        thread                  worker;
        /* Callback methor */
        ThreadManagerHandler    handler;

    public:

        /* Constructor */
        ThreadManagerTask
        (
            ThreadManager*
        );



        /*
            Destructor
        */
        ~ThreadManagerTask();



        /*
            Create new task
        */
        static ThreadManagerTask* create
        (
            ThreadManager*
        );



        /*
            Destroy task
        */
        void destroy();



        /*
            Return true if task handler defined
        */
        bool isHandler();



        /* Wait end of process */
        ThreadManagerTask* join();




        /*
            Set task handler
        */
        ThreadManagerTask* setHandler
        (
            /* callback lambda */
            ThreadManagerHandler
        );
};




/*
    Thread manageger
*/
class ThreadManager :public Result
{
    private:
        /* Log manager object */
        LogManager* logManager;

        /* List of workers with handlers */
        vector<ThreadManagerTask*> tasks;

        /* Mutex of this manager */
        mutex                   mtx;
        /* Child thread waiting controller */
        condition_variable cv;
        /* Manager waiting controller */
        condition_variable cv_manager;
        /* true when all children threads on pause */
        bool                    paused = true;
        /* count of threads on pause */
        int                     paused_threads = 0;
        /* Terminating begin */
        bool                    terminating = false;
        /* Terminateing finished */
        bool                    terminated = false;

        /*
            Send wakeup signal for waiting manager
        */
        ThreadManager* notifyManager();


        /*
            Send wakeup signal for waiting tasks
        */
        ThreadManager* notifyTasks();



        /*
            Add new thread for thread manager
            It will be paused.
        */
        ThreadManagerTask* addThread();


    public:

        /*
            Constructor
        */
        ThreadManager
        (
            /* LogManager object */
            LogManager*
        );



        /*
            Destructor
        */
        ~ThreadManager();


        /*
            Static method to create object
        */
        static ThreadManager* create
        (
            LogManager*
        );



        /*
            Static method for shared_ptr
        */
        static shared_ptr<ThreadManager> shared
        (
            /* LogManager object */
            LogManager*
        );



        /*
            Self-destructor
        */
        void destroy();



        /*
            Check threads count and add new thread
            Threads will not deleted
        */
        bool prepare
        (
            size_t
        );



        /*
            Method starts threads
        */
        ThreadManager* run();



        /*
            Method to stop a thread by its index
        */
        ThreadManager* terminate();




        /*
            Method to wait for all threads to finish
        */
        ThreadManager* wait();



        /*
            Return thrue for manager terminating finish
        */
        bool isTerminated();



        /*
            Return thrue for manager terminating begin
        */
        bool isTerminating();



        /*
            Return thrue for manager paused
        */
        bool isPaused();



        /*
            Don't call it
        */
        ThreadManager* taskComplete();



        /*
            Set task by index
        */
        ThreadManager* setHandler
        (
            /* Handlers index */
            size_t,
            /* callback lambda */
            const ThreadManagerHandler
        );


        /*
            Return count of threads
        */
        size_t getCount();



        bool isEmpty();
};
