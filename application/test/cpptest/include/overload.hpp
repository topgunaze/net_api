#ifndef __OVERLOAD__
#define __OVERLOAD__

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

void bool_test();

void type_test();

#endif
