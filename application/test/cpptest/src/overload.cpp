#include "overload.hpp"
#include <iostream>

using namespace std;

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

