#ifndef  __FUNCOBJECT_HPP__
#define __FUNCOBJECT_HPP__

class absdouble
{
	public:
		double operator()(double val)
		{
			return (val < 0) ? -val : val;
		}
};

struct absint
{
	//函数调用操作符
	int operator()(int val)
	{
		return (val > 0) ? val : -val; 
	}
};

template<typename T>
void displayelemet_f(T val);

template<typename T>
class displayelemet_c
{
public:
	displayelemet_c():count(0){}
	void operator()(T &val);
	
public:
	int count;
};

template<typename T>
struct displayelemet_s
{
	int count;
	displayelemet_s();
	void operator()(T &val);
};

template<typename T>
class ismultiple
{
	public:
		T divisor;
	public:
		ismultiple(const T &div):divisor(div){}
		bool operator()(const T &val)
		{
			return (val%divisor == 0);
		}
};

template<typename T>
class multiply
{
	public:
		T operator()(const T &a, const T &b)
		{
			return a*b;
		}
};

void funcobject_test();
void signal_funcobj_test();
void dual_funcobj_test();

#endif
