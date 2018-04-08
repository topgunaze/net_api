#include "containers.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

bool Comp(const int& a, const int& b) 
{
	return a > b;
}

int vector_test()
{
	vector<int> vec; 
	int i;

	cout << "vector size = " << vec.size() << " max size = " << vec.max_size() << endl;
	cout << "real size = " << vec.capacity() << endl;
 
	for(i = 0; i < 5; i++)
	{
		vec.push_back(i);
	}
 
	cout << "extended vector size = " << vec.size() << endl;
 
	for(i = 0; i < 5; i++)
	{
		cout << "value of vec [" << i << "] = " << vec[i] << endl;
	}

	reverse(vec.begin(), vec.end());

	for(i = 0; i < 5; i++)
	{
		cout << "value of vec [" << i << "] = " << vec.at(i) << endl;
	}

 	sort(vec.begin(), vec.end());
	
	// 使用迭代器 iterator 访问值
	vector<int>::iterator v = vec.begin();
	while( v != vec.end()) 
   	{
		cout << "value of v = " << *v << endl;
		++v;
	}

	sort(vec.begin(), vec.end(), Comp);

	for (size_t i = 0; i < vec.size(); ++i) 
	{
		cout << vec.at(i) << endl;
	}

	return 0;
}

void list_test()
{
	list<int> l0;
	list<int> l1(5);
	list<int> l2(5, 2);
	list<int> l3(l2);
	list<int> l4(l2.begin(), l2.end());

	for(list<int>::iterator it = l4.begin();it!=l4.end(); ++it)
	{
		cout << *it << "\t";
	}

	cout<<endl;

	for(list<int>::reverse_iterator it = l4.rbegin(); it != l4.rend(); ++it)
	{
		cout<<*it<<"\t";
	}

	cout<<endl;

	if (l0.empty())
	{
		for(int i = 0; i<20; ++i)
			l0.push_back(i);
	}

	for(list<int>::iterator it = l0.begin(); it != l0.end(); ++it)
	{
		cout<<*it<<"\t";
	}

	cout<<endl;
}

