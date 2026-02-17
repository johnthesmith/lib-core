#pragma once

#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <tuple>
#include <iostream>
#include <condition_variable>

#include "log_manager.h"
#include "result.h"


using namespace std;

/*

  add -> run                                         stop
--*--------------------------------------------------*-------> t
   \                                                 |
    A-----------------------                         |
     B----------                                     |
      C----------------------                        |
       \                     D------------           |
        \                     E-----                 |
         \                     F---------            |
          \                     G--------------------*
           \--------------
                          I------------------
                           J--------------
*/




enum TaskState
{
    TASK_CREATING,
    TASK_WAITING,
    TASK_RUNNING,
    TASK_TERMINATED
};



/*
    Handler function
*/
using ThreadManagerHandler = void (*)
(
    /* data structuer pointer */
    void*
);




/* Predeclaration */
class ThreadManager;




/*
    Thread managers task
*/
class ThreadManagerTask :public Result
{
    private:
        /* Thread id */
        std::string             id;
        /* Thread manager */
        ThreadManager*          owner;
        /* Thread worker */
        thread                  worker;
        /* Callback methor */
        ThreadManagerHandler    handler;
        /* Handler structure pointer */
        void*                   data;
        size_t                  dataSize;
        /* Task mutex */
        mutex                   task_mutex;
        /* Sleeper and wakeup */
        condition_variable      task_cv;
        /* Task state */
        std::atomic<TaskState>  state{ TASK_CREATING};

    public:

        /*
            Constructor
        */
        ThreadManagerTask
        (
            const std::string,
            ThreadManager*
        );



        /*
            Destructor
        */
        ~ThreadManagerTask();



        /*
            Create new task
            Doesn't call it directly
        */
        inline static ThreadManagerTask* create
        (
            const std::string aId,
            ThreadManager* aManager
        )
        {
            return new ThreadManagerTask( aId, aManager );
        }



        /*
            Selfdestructor
        */
        inline void destroy()
        {
            delete this;
        }



        /*
            Return true if task handler defined
        */
        inline bool isHandler()
        {
            return handler != nullptr;
        }



        /*
            Run task
        */
        ThreadManagerTask* run
        (
            ThreadManagerHandler,
            void* = nullptr,
            size_t = 0
        );




        /*
            Wait end of process
        */
        inline ThreadManagerTask* join()
        {
            if( worker.joinable())
            {
                worker.join();
            }
            return this;
        }



        /*
            Wake up the task
        */
        inline void notify()
        {
            task_cv.notify_one();
        }




        /*
            Clear memory buffer
        */
        inline ThreadManagerTask* freeData()
        {
            if( data != nullptr )
            {
                ::operator delete( data );
                dataSize = 0;
                data = nullptr;
            }
            return this;
        }
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
        std::unordered_map<string, ThreadManagerTask*> tasks;

        /* Mutex of this manager */
        mutex                   mtx;
        /* Manager waiting controller */
        condition_variable cv_manager;
        /* Terminating begin */
        bool                    terminating = false;
        /* Terminateing finished */
        bool                    terminated = false;
        /* Running task counter, add when run, dec when stop */
        std::atomic<size_t>     runningCount{0};


        /*
            Send wakeup signal for waiting manager
        */
        inline ThreadManager* notify()
        {
            cv_manager.notify_all();
            return this;
        }

        /*
            Send wakeup signal for waiting tasks
        */
        ThreadManager* notifyTasks()
        {
            unique_lock <mutex> lck( mtx );
            for( auto& [id, task] : tasks )
            {
                task -> notify();
            }
            return this;
        }


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
        inline static ThreadManager* create
        (
            LogManager* aLogManager
        )
        {
            return new ThreadManager( aLogManager );
        }



        /*
            Self-destructor
        */
        inline void destroy()
        {
            delete this;
        }



        /*
            Static method for shared_ptr
        */
        shared_ptr<ThreadManager> shared
        (
            /* Log manager */
            LogManager* aLogManager
        )
        {
            return make_shared <ThreadManager>( aLogManager );
        }



        /*
            Check threads count and add new thread
            Threads will not deleted
        */
        ThreadManagerTask* add
        (
            const std::string&
        );



        /*
            Method to stop a thread by its index
        */
        ThreadManager* terminate();



        /*
            Wait for compleet all threads
        */
        inline ThreadManager* wait()
        {
            /* Lock mutex */
            unique_lock <mutex> lck( mtx );
            /* Wait pause for all threads */
            cv_manager.wait
            (
                lck,
                [ this ]()
                {
                    /* This is terminateing || isPause() */
                    return terminating || runningCount == 0;
                }
            );

            return this;
        }



        /*
            Return true for terminated
        */
        bool isTerminated()
        {
            return terminated;
        }



        /*
            Return thrue for manager terminating begin
        */
        bool isTerminating()
        {
            return terminating;
        }



        /*
            Dont't use it method from application
            Only for internal use
        */
        inline ThreadManager* taskComplete()
        {
            runningCount--;
            {
                unique_lock <mutex> lck( mtx );
                notify();
            }
            return this;
        }



        /*
            Dont't use it method from application
            Only for internal use
        */
        ThreadManager* taskRun();



        /*
            Return count of threads
        */
        inline size_t getCount() const
        {
            return tasks.size();
        }



        /*
            Return task by id or null
        */
        inline ThreadManagerTask* byId
        (
            /*Id of task */
            const string& aId
        )
        {
            unique_lock <mutex> lck( mtx );
            auto it = tasks.find( aId );
            return it != tasks.end() ? it -> second : nullptr;
        }
};
