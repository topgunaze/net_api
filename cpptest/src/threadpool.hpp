#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <iostream>
#include <deque>
#include <string>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>

typedef void* (*p)(void*);

// ʹ��C++98 ���Թ淶ʵ�ֵ��̳߳أ� �������������ÿһ��job����Task�̳���Ķ���
namespace lz
{
    class Task
    {
	    public:
	        Task(void* arg = NULL, const std::string taskName = "")
	            : arg_(arg), 
	              taskName_(taskName)
	        {
	        
	        }
			
	        virtual ~Task()
	        {
	        
	        }
			
	        void setArg(void* arg)
	        {
	            arg_ = arg;
	        }

	        virtual int run() = 0;

	    protected:
	        void*       arg_;
	        std::string taskName_;
    };


    class ThreadPool
    {
	    public:
	        ThreadPool(int threadNum = 10);
	        ~ThreadPool();

	        size_t addTask(Task *task);
	        int    size();
	        void   start();
	        void   stop();

	    private:
	        ThreadPool& operator=(const ThreadPool&);
	        ThreadPool(const ThreadPool&);
	        int createThreads();
	        static void* threadFunc(void * threadData);
	        //void* threadFunc(void);
			Task* take();

	    private:
	        volatile  bool              isRunning_;
	        int                         threadsNum_;
			
	        pthread_t*                  p_threads_;
	        std::deque<Task*>           taskQueue_;
			
	        pthread_mutex_t             mutex_;
	        pthread_cond_t              condition_;
    };

	class Mytask : public  lz::Task
	{
		public:
		    //Mytask(){}
			
		    virtual int run()
			{
		        printf("thread[%lu] : %s\n", pthread_self(),(char*)this->arg_);
		        sleep(1);
				
		        return 0;
		    }
	};	
}

#endif // THREADPOOL_H_INCLUDED

