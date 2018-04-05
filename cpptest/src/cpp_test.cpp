#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpp_test.hpp"
#include "threadpool.hpp"


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

mystring::mystring(const char* p_string)
{
	int len;
	
	if(p_string == NULL)
	{
		p_str = new char[1];
		*p_str = '\0';
	}
	else
	{
		len = strlen(p_string)+1;
		p_str = new char[len];
		strcpy(p_str, p_string);
	}
}

mystring::mystring(const mystring &other)
{
	if (this == &other)
		return ;

	int len = strlen(other.p_str) + 1;
	p_str = new char[len];
	strcpy(p_str, other.p_str);
}

mystring& mystring::operator=(const mystring &other)
{
	if (this == &other)
		return *this;

	int len = strlen(other.p_str) + 1;
	delete []p_str;
	
	p_str = new char[len];
	strcpy(p_str, other.p_str);

	return *this;
}

mystring mystring::operator+(const mystring &other)
{
	mystring str;
	int len = strlen(p_str) + strlen(other.p_str) + 1;

	delete []str.p_str;
	str.p_str = new char[len];
	strcpy(str.p_str, p_str);
	strcpy(str.p_str+strlen(p_str), other.p_str);

	return str;
}

bool mystring::operator==(const mystring &other)
{
	if (strcmp(p_str, other.p_str) == 0)
		return true;
	else
		return false;
}

bool mystring::operator>(const mystring &other)
{
	if (strcmp(p_str, other.p_str) > 0)
		return true;
	else
		return false;
}

bool mystring::operator<(const mystring &other)
{
	if (strcmp(p_str, other.p_str) < 0)
		return true;
	else
		return false;
}

char& mystring::operator[](int idx)
{
	return p_str[idx];
}

void mystring::display(void)
{
	cout<<p_str<<endl;
}

mystring:: ~mystring()
{
	delete []p_str;
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

template <typename T>
mystack<T>::mystack(int size)
{
	this->size   = size;
	p_base = new T[size];
	top    = 0;
}

template <typename T>
mystack<T>::~mystack()
{
	delete []p_base;
	top    = 0;
	size   = 0;
}

template <typename T>
bool mystack<T>::isfull()
{
	return top == size ? true : false;
}

template <typename T>
bool mystack<T>::isempty()
{
	return top == 0 ? true : false;
}

template <typename T>
T mystack<T>::pop()
{
	return p_base[--top];
}

template<typename T>
bool mystack<T>::push(T data)
{
	if (isfull())
		return false;
	
	p_base[top++] = data;
	return true;
}

template<typename T>
myvector<T>::myvector(int size)
{
	this->size   = size;
	this->p_base = new T[size];
}

template <typename T>
myvector<T>::myvector(const myvector<T> &obj)
{
	size   = obj.size;
	p_base = new T[size];

	memcpy(p_base, obj.p_base, sizeof(T)*size);
}

template<typename T>
myvector<T>::~myvector()
{
	if (p_base)
	{
		delete []p_base;
		p_base = NULL;
		size = 0;
	}
}

template <typename T>
myvector<T>& myvector<T>::operator=(const myvector<T> &obj)
{
	if (&obj == this)
		return *this;

	delete []p_base;
	
	size   = obj.size;
	p_base = new T[size];

	memcpy(p_base, obj.p_base, sizeof(T)*size);
	
	return *this;
}

template <typename T>
T& myvector<T>::operator[](int idx)
{
	return p_base[idx];
}

template<typename T>
int myvector<T>::getsize()
{
	return size;
}

template <typename T>
ostream& operator<<(ostream &out, const myvector<T> &obj)
{
	int i;
	
	for (i = 0; i < obj.size; ++i)
	{
		out<<obj.p_base[i]<<" ";
	}
	out<<endl;
	
	return out;
}

/*
使用的同步原语有 
pthread_mutex_t mutex_l;//互斥锁
pthread_cond_t condtion_l;//条件变量
使用的系统调用有
pthread_mutex_init();
pthread_cond_init();
pthread_create(&thread_[i],NULL,threadFunc,this)
pthread_mutex_lock()
pthread_mutex_unlock()
pthread_cond_signal()
pthread_cond_wait()
pthread_cond_broadcast();
pthread_join()
pthread_mutex_destory()
pthread_cond_destory()

*/

int main()
{
<<<<<<< HEAD
	derive dr;
	dr.foo();

#if 0
=======
	cout << "begin" << endl;
	char    szTmp[] = "hello world";
	
//#if 0
	lz::Mytask  taskobj[20];

	for (int i = 0; i<20; i++)
	{
		taskobj[i].setArg((void*)szTmp);
	}
//#endif

#if 0
	lz::Mytask	taskobj;
	
	taskobj.setArg((void*)szTmp);
#endif

	lz::ThreadPool threadPool(10);
	threadPool.start();

	for(int i = 0; i<20; i++)
	{
	   threadPool.addTask(&taskobj[i]);
	   //threadPool.addTask(&taskobj);
	}

	while(1)
	{
	   printf("there are still %d tasks need to process\n", threadPool.size());
	   
	   if(threadPool.size() == 0)
	   {
		   threadPool.stop();
		   printf("now will exit from main\n");
		   exit(0);
	   }
	   
	   sleep(2);
	}

	cout<<"end"<<endl;

#if 0
	technician 		t("ma", 240);
	salesman   		s("pu", 100000);
	manager    		m("bai");
	sales_manager 	sm("wang", 1000000);

	employee *p[4] = {&t, &s, &m, &sm};
	int i;
	
	for (i = 0; i<4;++i)
		cout<<"i:"<<i<<" num:"<<p[i]->get_cardnum()<<" salary:"<<p[i]->get_salary()<<endl;
	
	cout<<"max num:"<<employee::get_maxnum()<<endl;
#endif

#if 0
	/*forbide: virtual func
	newspaper *p_paper = NULL;
	book      *p_bk;
	people    p;
	
	p.tell(p_bk);*/
	//p.tell(p_paper);

	people    *p = (people*)2;
	p->test();
#endif	
	
#if 0
>>>>>>> c375de27aeddeaaa77db7cb2f5aa7afb7a8f5f70
	myvector<double> vec(2);

	vec[0] = 1.0;
	vec[1] = 2.0;

	myvector<double> vec2(vec);

	myvector<double> vec3;
	vec3 = vec2;

	cout<<vec<<";"<<vec2<<";"<<vec3;
#endif	
	
#if 0
	mystack<double> s(100);

	if (!s.isfull())
		s.push(10.4);

	if (!s.isfull())
		s.push(9.4);

	if (!s.isfull())
		s.push(7.4);

	if (!s.isfull())
		s.push(1.4);

	while(!s.isempty())
		cout<<s.pop()<<" ";
#endif

#if 0
    myclock clk;
    clk.run();
#endif

#if 0
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
