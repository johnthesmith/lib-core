#include "thread_manager.h"



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
// cout << "thread_start\n";
            /* Thread code */
            while( !owner -> isTerminating() )
            {
// cout << "task_run\n";
                if( handler != nullptr )
                {
                    if( !owner -> isPaused() )
                    {
                        /* Task execute */
// cout << "task_work\n";
                        handler();
                    }
                    /* Drop task */
                    handler = nullptr;
                }
// cout << "task_completete\n";

                /* Set pause */
                owner -> taskComplete();
            }
// cout << "thread_stop\n";
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



/*
    Return true if task handler defined
*/
bool ThreadManagerTask::isHandler()
{
    return handler != nullptr;
}



/*
    Wait end of process
*/
ThreadManagerTask* ThreadManagerTask::join()
{
    if( worker.joinable())
    {
// cout << "task_join_begin\n";
        worker.join();
// cout << "task_join_end\n";
    }
    return this;
}



/*
    Set task by index
*/
ThreadManagerTask* ThreadManagerTask::setHandler
(
    /* callback lambda */
    const ThreadManagerHandler aHandler
)
{
// cout << "set_handler\n";
    handler = aHandler;
    return this;
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
    if( !terminating || terminated )
    {
        {
            unique_lock <mutex> lck( mtx );
            /* Set terminate to false */
            terminating = false;
            terminated = false;
        }

        /* Check thread count */
        for( size_t i = tasks.size(); i < aCount; i ++ )
        {
            /* Add task to tasks */
            tasks.push_back( ThreadManagerTask::create( this ));
        }
        wait();

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
    if( paused )
    {
        /* Reset pause */
        paused = false;

        /* Drop all counters */
        paused_threads = 0;

        /* Send signal for all threads */
        notifyTasks();
    }
    return this;
}



/*
    Method to stop a thread by its index
*/
ThreadManager* ThreadManager::terminate()
{
    // cout << "terminate_begin\n";

    {
        unique_lock <mutex> lck( mtx );
        terminating = true;
    }

    /* Send signal for manager */
    notifyTasks();

    for (auto& item : tasks)
    {
// cout << "terminate_join_begin\n";
        item -> join();
// cout << "terminate_join_end\n";
    }

    {
        unique_lock <mutex> lck( mtx );

        /* Wait stop threads */
        for (auto& item : tasks)
        {
            item -> destroy();
        }

        /* Delete all threads items */
        tasks.clear();
    }

    terminated = true;

    /* Send signal for all threads */
    notifyManager();

    // cout << "terminate_end\n";

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
// cout << "manager_wait_begin\n";
    cv_manager.wait
    (
        lck,
        [ this ]()
        {
            return !(!terminating && paused_threads < tasks.size());
        }
    );
// cout << "manager_wait_end\n";

    paused = true;

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
ThreadManager* ThreadManager::taskComplete()
{
    {
        unique_lock <mutex> lck( mtx );
        paused_threads++;

        notifyManager();

        if( !terminating )
        {
// cout << "task_complete_wait_begin\n";
            cv.wait
            (
                lck
            );
// cout << "task_complete_wait_end\n";
        }
    }

    return this;
}



/*
    Return true for terminated
*/
bool ThreadManager::isTerminating()
{
    return terminating;
}



/*
    Return true for terminated
*/
bool ThreadManager::isTerminated()
{
    return terminated;
}



/*
    Return true for all threads on paused
*/
bool ThreadManager::isPaused()
{
    return paused;
}




/*
    Set task by index
*/
ThreadManager* ThreadManager::setHandler
(
    /* Handlers index */
    size_t aIndex,
    /* callback lambda */
    const ThreadManagerHandler aHandler
)
{
    {
        unique_lock <mutex> lck( mtx );
        if( aIndex < tasks.size() )
        {
            tasks[ aIndex ] -> setHandler( aHandler );
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

