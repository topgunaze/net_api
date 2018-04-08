#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <deque>
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/syscall.h>

// 使用C++98 语言规范实现的线程池： 面向对象做法，每一个job都是Task继承类的对象
namespace thread_pool
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
			bool   getisRunning();

	    private:
	        ThreadPool& operator=(const ThreadPool&);
	        ThreadPool(const ThreadPool&);
	        int createThreads();
	        static void* threadFunc(void * threadData);
<<<<<<< HEAD:cpptest/src/threadpool.hpp
	        //void* threadFunc(void);
=======
>>>>>>> 3f647d1f0ae1437f097f4794d723f17ec5f74db6:application/test/cpptest/include/threadpool.hpp
			Task* take();

	    private:
	        volatile  bool              isRunning_;
	        int                         threadsNum_;
			
	        pthread_t*                  p_threads_;
	        std::deque<Task*>           taskQueue_;
			
	        pthread_mutex_t             mutex_;
	        pthread_cond_t              condition_;
    };
}

#endif // THREADPOOL_H_INCLUDED

