#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpp_test.hpp"


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

int main()
{

	myvector<double> vec(2);

	vec[0] = 1.0;
	vec[1] = 2.0;

	myvector<double> vec2(vec);

	myvector<double> vec3;
	vec3 = vec2;

	cout<<vec<<";"<<vec2<<";"<<vec3;
	
	
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
