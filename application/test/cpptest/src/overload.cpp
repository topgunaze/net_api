#include "overload.hpp"
#include <iostream>
#include <string.h>

using namespace std;

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



void point1d::dis()
{
	cout<<"x "<<x<<endl;	
}

void point2d::dis()
{
	cout<<"x "<<x<<" y "<<y<<endl;	
}

void point3d::dis()
{
	cout<<"x "<<x<<" y "<<y<<" z "<<z<<endl;	
}

void bool_test()
{
	ol_bool b(1);

	if (b)
		cout<<"b true"<<endl;
	else
		cout<<"b false"<<endl;

	if (b == false)
		cout<<"b false"<<endl;
	else
		cout<<"b true"<<endl;
}

void type_test()
{	
	point1d p1d(10);
	p1d.dis();
	
	point3d p3d(1,2,3);
	p3d.dis();

	point2d p2d0 = p1d;
	p2d0.dis();
	
	point2d p2d1(p3d);
	p2d1.dis();
}

