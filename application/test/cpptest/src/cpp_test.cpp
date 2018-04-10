#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpp_test.hpp"
#include "threadpool.hpp"
#include "containers.hpp"
#include "funcobject.hpp"


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

void mylist::initlist()
{
	p_head = NULL;
	size   = 0;
}

void mylist::insertlist(node *p_node)
{
	if (p_node == NULL)
		return ;

	if (p_head)
	{
		p_node->p_next = p_head;
		p_head = p_node;
	}
	else
	{
		p_head = p_node;
		p_head->p_next = NULL;
	}

	++size;
}

void mylist::deletenode(node *p_node)
{
	node* p_tmp = p_head;
	node* p_cur = p_head->p_next;

	if (p_node == NULL)
		return ;
	
	if (p_node == p_head)
	{
		p_head = p_head->p_next;
		--size;
		delete p_head;
		
		return;
	}

	while(p_cur != NULL)
	{
		if (p_cur == p_node)
		{
			p_tmp->p_next = p_cur->p_next;
			--size;
			delete p_cur;
			
			return;
		}
		else
		{
			p_tmp = p_cur;
			p_cur = p_cur->p_next;
		}
	}

	return;
}

node *mylist::searchlist(int value)
{
	node *p_tmp = p_head;
	
	while(p_tmp)
	{
		if (p_tmp->data == value)
			return p_tmp;
		else
			p_tmp = p_tmp->p_next;
	}

	return NULL;
}

void mylist::sortlist()
{

}

void mylist::destroylist()
{
	node *p_tmp;
	
	while(p_head)
	{
		p_tmp = p_head; 
		p_head = p_head->p_next;
		delete p_tmp;
	}
}

void mylist::printlist()
{
	node *p_tmp = p_head;
	
	while(p_tmp)
	{
		cout<<p_tmp->data<<" ";
		p_tmp = p_tmp->p_next;
	}

	cout<<endl;
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

	//funcobject_test();
	//signal_funcobj_test();
	dual_funcobj_test();


#if 0
	mylist l;
	node *p_node;
	node *p_node_array[20];

	for (int i = 0; i<20; ++i)
	{
		p_node = new node;
		p_node->data = i;
		l.insertlist(p_node);
		p_node_array[i] = p_node;
	}
	
	l.printlist();

	l.deletenode(p_node_array[10]);

	l.printlist();
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

void mystack_test()
{
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
}
