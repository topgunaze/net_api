#ifndef __OVERLOAD__
#define __OVERLOAD__

#include <iostream>

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

template <typename T> class myvector;
template <typename T> std::ostream& operator<<(std::ostream &out, const myvector<T> &obj);

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

	friend std::ostream& operator<<<T>(std::ostream &out, const myvector<T> &obj);
	
private:
	T   *p_base;
	int size;
};


//类型转换构造
class point1d
{
public:
	point1d(int a):x(a){}

	void dis();

	friend class point2d;
	
private:
	int x;
};

class point2d
{
public:
	point2d(int a, int b):x(a),y(b){}

	point2d(const point1d& p1d)
	{
		x = p1d.x;
		y = 0;
	}

	point2d(const point2d& p2d)
	{
		x = p2d.x;
		y = p2d.y;
	}

	void dis();
		
private:
	int x;
	int y;
};

//类型转换操作符
class point3d
{
public:
	point3d(int a, int b, int c):x(a),y(b),z(c){}

	operator point2d()
	{
		return point2d(x, y);
	}

	void dis();
	
private:
	int x;
	int y;
	int z;
};

class ol_bool
{
public:
	ol_bool(bool v):val(v){}

	operator bool()
	{
		return val;
	}

	bool operator ==(const bool &v)
	{
		return val == v;
	}
	
private:
	bool val;
};

void mystring_test();

void bool_test();

void type_test();

#endif
