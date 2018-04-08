#include <iostream>
#include <pthread.h>
#include <sys/syscall.h>
#include <assert.h>

#include "threadpool.hpp"


namespace thread_pool
{
    ThreadPool::ThreadPool(int threadNum)
	{
		threadsNum_ = threadNum;
	}
 
	void* ThreadPool::threadFunc(void* arg)
    {
    	//tid & pid
        pthread_t tid = pthread_self();
		pid_t     pid = syscall(SYS_gettid);
		
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
		
        while (pool->isRunning_)
        {
        	//×èÈû½Ó¿Ú
            Task* task = pool->take();

            if (!task)
            {
                std::cout<<"thread tid "<<tid<<" pid "<<pid<<" will exit\r\n"<<std::endl;
                break;
            }

            assert(task);
			
			std::cout<<"thread tid "<<tid<<" pid "<<pid<<" will exe\r\n"<<std::endl;
            task->run();
        }
		
        return 0;
    }

	int ThreadPool::createThreads()
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&condition_, NULL);
		
        //p_threads_ = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum_);
        p_threads_ = new pthread_t[threadsNum_];

        for (int i = 0; i < threadsNum_; ++i)
        {
            pthread_create(&p_threads_[i], NULL, ThreadPool::threadFunc, (void*)this);
			//error 
        }
		
        return 0;
    }

    void ThreadPool::start()
	{
	    isRunning_ = true;
        createThreads();

    }

    ThreadPool::~ThreadPool()
    {
        stop();
		
        for(std::deque<Task*>::iterator it = taskQueue_.begin(); it != taskQueue_.end(); ++it)
        {
            delete *it;
        }
		
        taskQueue_.clear();
    }

    size_t ThreadPool::addTask(Task *task)
    {
        pthread_mutex_lock(&mutex_);
		
        taskQueue_.push_back(task);
        int size = taskQueue_.size();
		
        pthread_mutex_unlock(&mutex_);
        pthread_cond_signal(&condition_);
		
        return size;
    }

    void ThreadPool::stop()
    {
        if (!isRunning_)
        {
            return;
        }

        isRunning_ = false;
        pthread_cond_broadcast(&condition_);

        for (int i = 0; i < threadsNum_; i++)
        {
            pthread_join(p_threads_[i], NULL);
        }

        //free(p_threads_);
		delete []p_threads_;
        p_threads_ = NULL;

        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&condition_);
    }

    int ThreadPool::size()
    {
        pthread_mutex_lock(&mutex_);
		
        int size = taskQueue_.size();
		
        pthread_mutex_unlock(&mutex_);
		
        return size;
    }

    Task* ThreadPool::take()
    {
        Task* task = NULL;
		
        while (!task)
        {
            pthread_mutex_lock(&mutex_);
			
            while (taskQueue_.empty() && isRunning_)
            {
                pthread_cond_wait(&condition_, &mutex_);
            }

            if (!isRunning_)
            {
                pthread_mutex_unlock(&mutex_);

                break;
            }
#if 0
            else if (taskQueue_.empty())
            {
                pthread_mutex_unlock(&mutex_);
                continue;
            }
#endif

            assert(!taskQueue_.empty());
            task = taskQueue_.front();
            taskQueue_.pop_front();
			
            pthread_mutex_unlock(&mutex_);
        }
		
        return task;
    }
	
    bool ThreadPool::getisRunning()
	{
		return isRunning_;
	}
}

