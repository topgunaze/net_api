#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpp_test.hpp"
#include "threadpool.hpp"
#include "containers.hpp"
#include "funcobject.hpp"
#include "overload.hpp"
#include "struct.hpp"
#include "suanfa.hpp"
//#include "redis_api.hpp"
#include "designmode.hpp"


#if 0
Swap(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}


int Partition (int data[], int length, int start, int end)
{
    if(data = NULL || length <= 0 || start < 0 || end >= length)
		return -1;

    int index = Randominrange(start, end);
	
    Swap(&data[index], &data[end]);
    int small= start - 1;
    
    for(index = start; index < end; ++index)
    {
        if(data[index] < data[end])
        {
            ++small;
            if (small != index)
            Swap(&data[index], &data [small]);
        }
    }
    
    ++ small
    Swap(&data[small], &data[end]);
    return small
}

void Quicksort(int data[], int length, int start, int end)
{
    if(start == end)
        return;

    int index = Partition(data, length, start, end);
    
    if(index > start)
        Quicksort(data, length, start, index - 1);
        
    if( index < end)
        Quicksort(data, length, index + 1, end);
}
#endif

Stu& Stu::growup(void)
{
	++age;
	return *this;
}

void Stu::display(void)
{
	cout<<name<<":"<<age;	
}

myclock::myclock()
{
    time_t t = time(NULL);
    tm *p_local_tm = localtime(&t);
    hour = p_local_tm->tm_hour;
    min  = p_local_tm->tm_min;
    sec  = p_local_tm->tm_sec;
}

void myclock::run(void)
{
	while(1)
	{
	    tick();
        show();
    }
}

void myclock::tick()
{
    sleep(1);
    ++sec;
	if (sec == 60)
	{
	    sec = 0;
        ++min;
        if (min == 60)
        {
            ++hour;
            min = 0;
            if (hour == 24)
            {
                hour = 0;
            }
        }
	}
}

void myclock::show(void)
{
    //system("clear");
    cout<<hour<<":"<<min<<":"<<sec<<endl;
}

class Mytask : public thread_pool::Task
{
	public:
		
		virtual int run()
		{
			cout<<"thread tid "<<pthread_self()<<" arg "<<(char*)this->arg_<<endl;
			sleep(1);
			
			return 0;
		}
};	

int main()
{
 
	//doTest();

#if 1
	//simple_factory_test();
	//simple_delegate_test();

#endif

	
	//quicksort_test();


#if 0
	//deque_test();
	//vector_test();
	//list_test();
	//string_test();
	//set_test();
	//map_test();
	//stack_test();
	//queue_test();
#endif

#if 0
	//funcobject_test();
	//signal_funcobj_test();
	//dual_funcobj_test();

	//bool_test();
	type_test();
#endif


#if 0
	mylist_test();
#endif

#if 0

	//count_find_search_test();
	//min_max_test();
	//find_test();
	copy_test();
	//mismatch_test();
	//permutation_test();
	count_find_search_test();
	//min_max_test();
	//find_test();

#endif

#if 0
	derive dr;
	dr.foo();
#endif
	
#if 0
	cout << "begin" << endl;
	char    szTmp[] = "hello world";

	Mytask  taskobj[20];
	
	for (int i = 0; i<20; i++)
	{
		taskobj[i].setArg((void*)szTmp);
	}

	thread_pool::ThreadPool threadPool(10);

	threadPool.start();
	lz::Mytask  taskobj[20];

	for(int i = 0; i<20; i++)
	{
	   threadPool.addTask(&taskobj[i]);
	   //threadPool.addTask(&taskobj);
	}

	while(1)
	{
	   cout<<"there are still %d "<<threadPool.size()<<"tasks need to process\r\n"<<endl;
	   
	   if(threadPool.size() == 0)
	   {
		   threadPool.stop();
		   cout<<"now will exit from main\n"<<endl;
		   exit(0);
	   }
	   
	   sleep(2);
	}

	cout<<"end"<<endl;
#endif

#if 0
	multi_inheritance();
#endif

#if 1
	//simple_factory_test();
	//simple_singleton_test();
	//binary_tree_findpath_test();
	//binary_tree_traverse_print();
	//binary_tree_ztraverse_print_test();
	//binary_tree_is_symmetrical_test();
	//binary_tree_isbanance_depth_test();
	//binary_tree_has_subtree();
	//binary_tree_mirror_test();
	//binary_tree_reconstruct_test();
	binary_tree_treedepth_test();


#endif	
	
#if 0
	myvector<double> vec(2);

	vec[0] = 1.0;
	vec[1] = 2.0;

	myvector<double> vec2(vec);

	myvector<double> vec3;
	vec3 = vec2;

	cout<<vec<<";"<<vec2<<";"<<vec3;
#endif	
	
#if 0
	mystack_test();
#endif

#if 0
    myclock clk;
    clk.run();
#endif

#if 0
	mystring_test();
#endif

#if 0
	vector<int> vec;
	vector<int>::iterator vet;
	int value;
	
	srand(time(NULL));

	while(1)
	{
		value = rand()%10;
		vet = vec.begin();
		for (; vet != vec.end(); ++vet)
		{
			if (*vet == value)
				break;
	
		}

		if (vet == vec.end())
		{
			vec.push_back(value);
		}

		if (vec.size() == 7)
			break;
	}

	for (vet = vec.begin(); vet != vec.end(); ++vet)
	{
		cout<<*vet<<" ";
	}
#endif

#if 0
	Stu s("liutf", 28);

	s.growup().growup();
	s.display();
#endif

	return 0;
}

void multi_inheritance()
{
	technician 		t("ma", 240);
	salesman   		s("pu", 100000);
	manager    		m("bai");
	sales_manager 	sm("wang", 1000000);

	employee *p[4] = {&t, &s, &m, &sm};
	int i;
	
	for (i = 0; i<4;++i)
		cout<<"i:"<<i<<" num:"<<p[i]->get_cardnum()<<" salary:"<<p[i]->get_salary()<<endl;
	
	cout<<"max num:"<<employee::get_maxnum()<<endl;
}

void mystring_test()
{
	mystring s1("hehe");
	s1.display();

	mystring s2;

	s2.display();

	mystring s3(s1);
	mystring s4 = s1;

	s3.display();

	s4.display();

	mystring s5;
	s5 = s3 + s4;
	s5.display();
}


 
