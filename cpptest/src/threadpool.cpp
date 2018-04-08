#include "threadpool.hpp"
#include <stdio.h>
#include <assert.h>

namespace lz
{
    ThreadPool::ThreadPool(int threadNum)
	{
		threadsNum_ = threadNum;
		//isRunning_ = true;    
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
                printf("thread %lu %d will exit\n", tid, pid);
                break;
            }

			printf("thread %lu %d will run\n", tid, pid);

            assert(task);
            task->run();
        }
		
        return 0;
    }

	int ThreadPool::createThreads()
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&condition_, NULL);
		
        p_threads_ = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum_);
		
        for (int i = 0; i < threadsNum_; ++i)
        {
            pthread_create(&p_threads_[i], NULL, threadFunc, (void*)this);
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

        free(p_threads_);
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
	
    //bool ThreadPool::getisRunning_(){return isRunning_;}
}

