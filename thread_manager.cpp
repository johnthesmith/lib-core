#include "thread_manager.h"
#include <unistd.h> /* usleep */
#include <x86intrin.h>
#include <iomanip>  // для std::hex и std::setw

ThreadManagerTask::ThreadManagerTask
(
    ThreadManager* aManager
)
{
    /* Let manager */
    owner = aManager;

    /* Reset handler */
    handler = nullptr;

    /* Create thread with body */
    worker = thread
    (
        [ this ]
        ()
        {
            /* Thread code */
            while( !owner -> isTerminating() )
            {
                if( handler != nullptr )
                {
                    if( !owner -> isPaused() )
                    {
                        /* Task execute */
                        handler( data );
                    }
                    /* Drop task */
                    handler = nullptr;
                }
                /* Wait */
                owner -> taskComplete( this );
            }
        }
    );
}



/*
    Destructor
*/
ThreadManagerTask::~ThreadManagerTask()
{
    join();
}



/*
    Create new task
*/
ThreadManagerTask* ThreadManagerTask::create
(
    ThreadManager* aManager
)
{
    return new ThreadManagerTask( aManager );
}



/*
    Selfdestructor
*/
void ThreadManagerTask::destroy()
{
    delete this;
}



/******************************************************************************
    ThreadManager
*/



/*
    Constructor
*/
ThreadManager::ThreadManager
(
    LogManager* aLogManager
)
{
    logManager = aLogManager;
}



/*
    Destructor
*/
ThreadManager::~ThreadManager()
{
    terminate();
}



/*
    Static method to create object
*/
ThreadManager* ThreadManager::create
(
    LogManager* aLogManager
)
{
    return new ThreadManager( aLogManager );
}



/*
    Static method for shared_ptr
*/
shared_ptr<ThreadManager> ThreadManager::shared
(
    LogManager* aLogManager
)
{
    return make_shared <ThreadManager>( aLogManager );
}



/*
    Self-destructor
*/
void ThreadManager::destroy()
{
    delete this;
}


/*
    Check threads count and add new thread
    Threads will not deleted
*/
bool ThreadManager::prepare
(
    size_t aCount
)
{
    unique_lock <mutex> lck( mtx );

    if( !terminating || terminated )
    {
        /* Set terminate to false */
        terminating = false;
        terminated = false;

        /* Check thread count */
        for( size_t i = tasks.size(); i < aCount; i ++ )
        {
            /* Add task to tasks */
            tasks.push_back( ThreadManagerTask::create( this ));
        }
        /* Wait for compleet all threads */
        cv_manager.wait
        (
            lck,
            [ this, aCount ]()
            {
                /* This is terminateing || isPause() */
                return terminating || pausedThreads == tasks.size();
            }
        );

        return true;
    }
    else
    {
        return false;
    }
}



/*
    Method starts a threads
*/
ThreadManager* ThreadManager::run()
{
    /* Wait current jobs finish */
    wait();

    /* Send signal for all threads */
    unique_lock <mutex> lck( mtx );
    pausedThreads = 0;
    notifyTasks();

    return this;
}



/*
    Method to stop a thread by its index
*/
ThreadManager* ThreadManager::terminate()
{
    {
        unique_lock <mutex> lck( mtx );
        terminating = true;
    }

    /* Send signal for manager */
    notifyTasks();

    for( auto& item : tasks )
    {
        item -> join();
    }

    {
        unique_lock <mutex> lck( mtx );

        /* Wait stop threads */
        for (auto& item : tasks)
        {
            item -> destroy();
        }

        /* Drop cout of tasks */
        pausedThreads = 0;

        /* Delete all threads items */
        tasks.clear();
    }

    terminated = true;

    /* Send signal for all threads */
    notifyManager();

    return this;
}



/*
    Wait for compleet all threads
*/
ThreadManager* ThreadManager::wait()
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
            return terminating || pausedThreads == tasks.size();
        }
    );

    return this;
}



/*
    Send wakeup signal for waiting manager
*/
ThreadManager* ThreadManager::notifyManager()
{
    cv_manager.notify_all();
    return this;
}



/*
    Send wakeup signal for waiting tasks
*/
ThreadManager* ThreadManager::notifyTasks()
{
    cv.notify_all();
    return this;
}



/*
    Dont't use it method from application
    Only for internal use
*/
ThreadManager* ThreadManager::taskComplete( ThreadManagerTask* a )
{
    uint64_t tsc = __rdtsc();
    string t = std::to_string(tsc);
    {
        unique_lock <mutex> lck( mtx );
        pausedThreads++;
        notifyManager();
        if( !terminating )
        {
            cv.wait( lck );
        }
    }

    return this;
}



/*
    Set task by index
*/
ThreadManager* ThreadManager::setHandler
(
    /* Handlers index */
    size_t aIndex,
    /* Data structure ptr for handler */
    void* aData,
    /* callback lambda */
    const ThreadManagerHandler aHandler
)
{
    {
        unique_lock <mutex> lck( mtx );
        if( aIndex < tasks.size() )
        {
            tasks[ aIndex ]
            -> setHandler( aHandler )
            -> setData( aData );
        }
    }
    return this;
}




size_t ThreadManager::getCount()
{
    return tasks.size();
}



bool ThreadManager::isEmpty()
{
    return tasks.size() == 0;
}

