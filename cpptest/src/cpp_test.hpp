#ifndef CPP_TEST
#define CPP_TEST

#include <string.h>

using namespace std;

class Stu
{
public:
	Stu(string name, int age)//:name(name),age(age)
		{
			this->name = name;
			this->age  = age;
		};

	~Stu(){};

	Stu& growup(void);
	void display(void);

private:
	string name;
	int    age;
};

class mystring
{
public:
	mystring(const char* p_string = NULL);
	mystring(const mystring &other);
	mystring& operator=(const mystring &other);
	mystring operator+(const mystring &other);
	
	bool operator==(const mystring &other);
	bool operator>(const mystring &other);
	bool operator<(const mystring &other);
	char& operator[](int idx);
	
	void display(void);
	
	virtual ~mystring();
	
private:
	char* p_str;
};

class myclock
{
public:
    myclock();
    /*myclock(int h = 0, int m = 0, int s = 0):hour(h), min(m), sec(s)
        {
        
        }*/
    
	~myclock(){};
	void run(void);
	
private:
    void tick();
    void show();
    
	int hour;
	int min;
	int sec;
};

template<typename T>
class mystack
{
public:
	mystack(int size);
	~mystack();

	bool isfull(void);
	bool isempty(void);
	bool push(T data);
	T pop(void);

private:
	T  *p_base;
	int top;
	int size;
	
};

template <typename T> class myvector;
template <typename T> ostream& operator<<(ostream &out, const myvector<T> &obj);


template<typename T>
class myvector
{
public:
	myvector(int size = 0);
	myvector(const myvector<T> &obj);
	~myvector();
	myvector<T>& operator=(const myvector<T> &obj);
	T& operator[](int idx);

	int getsize(void);

	friend ostream& operator<<<T>(ostream &out, const myvector<T> &obj);
	
private:
	T   *p_base;
	int size;
};

class base
{
public:
	void foo(void)
	{
		func();
	}

	virtual void func(void)
	{
		cout<<"base func"<<endl;
	}
};

class derive:public base
{
private:
	void func(void)
	{
		cout<<"derive func"<<endl;
	}
};

#endif
