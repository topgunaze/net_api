#include "funcobject.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <deque>


using namespace std;


template<typename T>
void displayelemet_f(T val)
{
	cout<<val<<endl;
}

template<typename T>
void displayelemet_c<T>::operator()(T &val)
{
	cout<<val<<' ';
	++count;
}

template<typename T>
displayelemet_s<T>::displayelemet_s()
{
	count = 0;
}

template<typename T>
void displayelemet_s<T>::operator()(T &val)
{
	cout<<val<<' ';
	++count;
}

class comparestrnocase
{
public:
	bool operator(const string &str1, const string &str2)
	{
		string str1_tmp;
		str1_tmp.resize(str1.size());
		transform(str1.begin(), str1.end(), str1_tmp.begin(), tolower);

		string str2_tmp;
		str2_tmp.resize(str2.size());
		transform(str2.begin(), str2.end(), str2_tmp.begin(), tolower);

		return (str1_tmp < str2_tmp)
	}
};

void funcobject_test()
{
	int    i = -432;
	absint absi;

	cout<<absi(i)<<endl;

	double    j = -3.14;
	absdouble absd;

	cout<<absd(j)<<endl;

	vector<int> via;
	for (int i = 0; i<10; ++i)
		via.push_back(i+1);

	deque<int> dia;
	for (int i = 0; i<10; ++i)
		dia.push_back(i*2);

	list<char> lca;
	for (char i = 'a'; i<'h'; ++i)
		lca.push_back(i);
	
	displayelemet_c<int> displayele;

	//匿名函数对象
	for_each(via.begin(), via.end(), displayelemet_c<int>());
	cout<<endl;

	displayele = for_each(dia.begin(), dia.end(), displayele);
	cout<<"dia size "<< displayele.count<<endl;
	
	for_each(lca.begin(), lca.end(), displayelemet_s<char>());
	cout<<endl;

	displayelemet_f<int>(3);
}

void signal_funcobj_test()
{
	//函数对象
	ismultiple<int> fo(3);
	vector<int> via0;

	for (int i = 10; i<20; ++i)
		via0.push_back(i);

	vector<int>::iterator it = find_if(via0.begin(), via0.end(), fo);
	if (it != via0.end())
	{
		cout<<"the first div3 "<<*it<<endl;
	}

	it = find_if(via0.begin(), via0.end(), ismultiple<int>(4));
	if (it != via0.end())
	{
		cout<<"the first div4 "<<*it<<endl;
	}	
}

void dual_funcobj_test()
{
	vector<int> via1;
	vector<int> via2;
	vector<int> via3;

	for (int i = 1; i<10; ++i)
		via1.push_back(i);

	for (int i = 1; i<10; ++i)
		via2.push_back(i*2);

	via3.resize(10);

	transform(via1.begin(), via1.end(), via2.begin(), via3.begin(), multiply<int>());

	cout<<"via3 "<<endl;
	for(vector<int>::iterator it = via3.begin(); it != via3.end(); ++it)
		cout<<*it<<' ';
	cout<<endl;


	set<string, comparestrnocase> strs0;
	strs0.insert("Jim");
	strs0.insert("Jam");
	strs0.insert("jbm");
	strs0.insert();
	
}

