#include "containers.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

/*
对象的定义与初始化
对象的操作
*/

bool Comp(const int& a, const int& b) 
{
	return a > b;
}

void vector_test()
{
	vector<int> 	ivec0;
	vector<int>     ivec1(10);
	vector<int>     ivec2(10, 1);
	vector<int>     ivec3(ivec2);
	vector<string> 	svec0;
	
	svec0.push_back("135");
	svec0.push_back("246");

	cout << "ivec0 size = " << ivec0.size() << " max size = " << ivec0.max_size() << " real size = " << ivec0.capacity() <<endl;
 
	for(vector<int>::size_type i = 0; i < 5; ++i)
	{
		ivec0.push_back(i);
	}
 
	cout << "extended vector size = " << ivec0.size() << endl;
 
	for(vector<int>::size_type i = 0; i < 5; ++i)
	{
		cout << "value of ivec0 [" << i << "] = " << ivec0[i] << endl;
	}

	reverse(ivec0.begin(), ivec0.end());

	for(vector<int>::size_type i = 0; i < 5; i++)
	{
		cout << "value of ivec0 [" << i << "] = " << ivec0.at(i) << endl;
	}

 	sort(ivec0.begin(), ivec0.end());
	
	// 使用迭代器 iterator 访问值
	vector<int>::iterator v = ivec0.begin();
	while( v != ivec0.end()) 
   	{
		cout << *v << endl;
		++v;
	}

	sort(ivec0.begin(), ivec0.end(), Comp);

	for (vector<string>::size_type i = 0; i < svec0.size(); ++i) 
	{
		cout << svec0.at(i) << endl;
	}

	svec0.push_back("79");
	svec0.push_back("810");
	
	for (vector<string>::size_type i = 0; i < svec0.size(); ++i) 
	{
		cout << svec0.at(i) << endl;
	}

	while(!svec0.empty())
	{
		cout<<svec0.back()<<" ";
		//cout<<svec0.front()<<" ";
		svec0.pop_back();
	}

	cout<<endl;
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

	l0.erase(l0.begin(), l0.end());

	l0.push_back(1);
	l0.push_back(2);
	l0.push_back(3);
	l0.push_front(10);
	l0.push_front(9);
	l0.push_front(8);
			
	l0.insert(l0.begin(), 4, 11);
	l0.insert(l0.end(), 1, 12);	

	cout<<"l0"<<endl;
	for (list<int>::iterator it = l0.begin(); it != l0.end(); ++it)
	{
		cout<<*it<<" ";
	}
	
	cout<<endl;

	cout<<"l1"<<endl;
	for (list<int>::iterator it = l1.begin(); it != l1.end(); ++it)
	{
		cout<<*it<<" ";
	}
	
	cout<<endl;

	list<int>::iterator iter = l1.begin();
	++iter;
	l1.insert(iter, 1);
	++iter;
	l1.insert(iter, 4, 2);
	
	cout<<"l1"<<endl;
	for (list<int>::iterator it = l1.begin(); it != l1.end(); ++it)
	{
		cout<<*it<<" ";
	}
	
	cout<<endl;

	cout<<"l0"<<endl;

	l0.insert(l0.begin(), ++l1.begin(), --l1.end());
	for (list<int>::iterator it = l0.begin(); it != l0.end(); ++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	
	//反转
	cout<< "l0 reserve"<<endl;
	l0.reverse();
	for (list<int>::iterator it = l0.begin(); it != l0.end(); ++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	
	//排序
	cout<<"l0 sort"<<endl;
	l0.sort();
	for (list<int>::iterator it = l0.begin(); it != l0.end(); ++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
}

void deque_test()
{
	deque<int> d0;
	deque<int> d1(3, 2);
	deque<int> d2(d1);
	deque<int> d3(d2.begin(), d2.end());
	
	d0 = d3;

	for(deque<int>::iterator it = d0.begin(); it != d0.end(); ++it)
	{
		cout<<*it<<" ";
	}

	cout<<endl;
	
	d0.erase(d0.begin(), d0.end());
	d0.push_back(1);
	d0.push_back(2);

	d0.push_front(9);
	d0.push_front(10);

	while(!d0.empty())
	{
		cout<<d0.front()<<" ";
		d0.pop_front();
	}

	for(int i = 0; i < 10; ++i)
		d1.push_front(i);

	//distance
	for (deque<int>::iterator it = d1.begin(); it != d1.end(); ++it)
	{
		cout<<"d1["<<distance(d1.begin(), it)<<"]: "<<*it<<endl;
	}
	
	cout<<endl;
}

char tolower(char ch)
{
    if(ch>='A'&&ch<='Z')
        return ch+32;
    else
        return ch;
}

char toupper(char ch)
{
    if(ch>='a' && ch<='z')
        return ch-32;
    else
        return ch;
}

void string_test()
{
	const char* p_contents = "this is a c string";
	string str1(p_contents);
	string str2(10, 'a');
	string str3(p_contents, 5);

	cout<<"str1 "<<str1<<endl;
	cout<<"str2 "<<str2<<endl;
	cout<<"str3 "<<str3<<endl;

	//迭代
	for (unsigned int i = 0; i< str2.length(); ++i)
	{
		cout<< str2[i];
	}

	cout<<endl;
	
	for (string::const_iterator st = str3.begin(); st < str3.end(); ++st)
	{
		cout<< *st;
	}

	cout<<endl;

	//追加
	str1 += str2;
	str3.append(str2);

	cout<<"str1 "<<str1<<endl;
	cout<<"str2 "<<str2<<endl;

	//search
	int count = 0;
	size_t p = str3.find("a", 0);
	while(p != str3.npos)
	{
		cout << "position " <<p<<endl;
		++count;
		p = str3.find("a", p+1);
	}
	
	if (count)		
		cout << "count " <<count<<endl;
	else
		cout<<"not find"<<endl;

	//清除
	cout<<"str1 "<<str1<<endl;
	cout<<"str2 "<<str2<<endl;
	cout<<"str3 "<<str3<<endl;

	str1.erase(1, 4);
	str2.erase(str2.begin(), str2.end());
	//string::iterator it = find(str3.begin(), str3.end(), "a");
	//if (it != str3.end())
		//str3.erase(it);

	cout<<"str1 "<<str1<<" length "<< str1.length()<<endl;
	cout<<"str2 "<<str2<<" length "<< str2.length()<<endl;
	//cout<<"str3 "<<str3<<"length "<< str3.length()<<endl;

	//反转
	reverse(str3.begin(), str3.end());
	cout<<"str3 "<<str3<<endl;

	//大小写转换
	//string str4;
	//str4.resize(str3.size());
	
	//transform(str3.begin(), str3.end(), str4.begin(), toupper);
	//cout<<"str3 "<<str3<<endl;
}

void set_test()
{
	set<int> iset0;
	set<int> iset1;

	//insert
	if (iset0.empty())
		for(int i=0; i<10; ++i)
			iset0.insert(10-i);
		
	cout<<"iset0 size "<<iset0.size()<<endl;
	for(set<int>::iterator it = iset0.begin(); it != iset0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	for(int i=0; i<10; ++i)
		iset0.insert(i*2);

	cout<<"iset0 size "<<iset0.size()<<endl;
	for(set<int>::iterator it = iset0.begin(); it != iset0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	//find & delete
	set<int>::iterator iter = iset0.find(2);
	if (iter == iset0.end())
	{
		cout<<"not find"<<endl;
	}
	else
	{
		iset0.erase(iter);
		iset0.insert(3);
	}
	
	for(set<int>::iterator it = iset0.begin(); it != iset0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	multiset<int> multiset0;

	multiset0.insert(iset0.begin(), iset0.end());

	for (int i = 0; i<10; ++i)
		multiset0.insert(i*3);
	
	for(multiset<int>::iterator it = multiset0.begin(); it != multiset0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	cout<<"multi 3 count "<<multiset0.count(3)<<endl;

	//find & delete
	multiset<int>::iterator multi_iter = multiset0.find(3);
	while (multi_iter != multiset0.end())
	{
		multiset0.erase(multi_iter);
		multi_iter = multiset0.find(3);
	}

	for(multiset<int>::iterator it = multiset0.begin(); it != multiset0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	cout<<"multi 3 count "<<multiset0.count(3)<<endl;
}

void map_test()
{
	map<int, string> ismap0;

	ismap0.insert(map<int, string>::value_type(1, "one"));
	ismap0.insert(map<int, string>::value_type(2, "two"));
	ismap0.insert(map<int, string>::value_type(3, "three"));
	ismap0.insert(map<int, string>::value_type(3, "three"));
	ismap0.insert(make_pair(4,"four"));
	ismap0.insert(pair<int, string>(5,"five"));
	ismap0[10] = "ten";

	cout<<"ismap0 size "<<ismap0.size()<<endl;
	
	for(map<int, string>::iterator it =ismap0.begin(); it != ismap0.end(); ++it)
	{
		cout<<it->first<<" "<<it->second<<endl;
	}


	multimap<int, string> ismmap0;
	ismmap0.insert(multimap<int, string>::value_type(11, "oneone"));
	ismmap0.insert(multimap<int, string>::value_type(12, "onetwo"));
	ismmap0.insert(multimap<int, string>::value_type(13, "onethree"));
	ismmap0.insert(multimap<int, string>::value_type(13, "onethree"));
	ismmap0.insert(make_pair(14, "onefour"));
	ismmap0.insert(pair<int, string>(15, "onefive"));
	//ismmap0[10] = "ten";
	
	cout<<"ismmap0 size "<<ismmap0.size()<<endl;
	
	for(multimap<int, string>::iterator it =ismmap0.begin(); it != ismmap0.end(); ++it)
	{
		cout<<it->first<<" "<<it->second<<endl;
	}

	multimap<int, string>::iterator iter = ismmap0.find(13);
	if (iter != ismmap0.end())
	{
		cout<<"find 13"<<endl;
		size_t count = ismmap0.count(13);
		for (size_t i = 0; i < count; ++i)
		{
			//cout<<"ismmap0["<<distance(iter, ismmap0.begin())<<"] "<<iter->first<<" "<<iter->second<<endl;
			cout<<iter->first<<" "<<iter->second<<endl;
			++iter;
		}
	}

	iter = ismmap0.find(13);
	while(iter != ismmap0.end())
	{
		ismmap0.erase(iter);
		iter = ismmap0.find(13);
	}

	for(multimap<int, string>::iterator it =ismmap0.begin(); it != ismmap0.end(); ++it)
	{
		cout<<it->first<<" "<<it->second<<endl;
	}
		
}

