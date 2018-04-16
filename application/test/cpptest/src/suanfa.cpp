#include "suanfa.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <iterator>
#include<algorithm>

using namespace std;

template<typename T>
bool iseven(const T &val)
{
	return (val%2 == 0);
}

bool absint(int v1, int v2)
{
	return (abs(v1) < abs(v2));
}
	

void count_find_search_test()
{
	vector<int> iv0;

	for(int i = -9; i<10; ++i)
		iv0.push_back(i);

	vector<int>	::iterator it = iv0.begin();
	while(it != iv0.end())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;

	cout<<"for count count_if test"<<endl;
	size_t n = count(iv0.begin(), iv0.end(), 80);

	cout <<"80 "<<n<<endl;

	n = count_if(iv0.begin(), iv0.end(), iseven<int>);
	cout<<"iseven "<<n<<endl;

	cout<<"for find find_if test"<<endl;
	it = find(iv0.begin(), iv0.end(), 3);
	if (it != iv0.end())
		cout<<"find 3 "<<distance(iv0.begin(), it)<<endl;

	cout<<"iseven"<<endl;
	vector<int>::iterator it0;
	it0 = find_if(iv0.begin(), iv0.end(), iseven<int>);
	while(it0 != iv0.end())
	{
		cout<<distance(iv0.begin(), it0)<<" ";
		cout<<*it0<<"  ";
		it0 = find_if(++it0, iv0.end(), iseven<int>);
	}
	cout<<endl;

	cout<<"for search search_n test"<<endl;
	list<int> il0;
	for (int i = 0; i<10; ++i)
		il0.push_back(i);

	vector<int>::iterator vit;
	for (vit = iv0.begin(); vit != iv0.end(); ++vit)
		cout<<*vit<<" ";
	cout<<endl;

	vit = search(iv0.begin(), iv0.end(), il0.begin(), il0.end());
	if (vit != iv0.end())
		cout<<"search  "<<distance(iv0.begin(), vit)<<endl;

	deque<int> id0;
	id0.push_back(0);
	id0.push_back(1);
	id0.push_back(2);
	id0.push_back(2);
	id0.push_back(2);
	id0.push_back(4);
	id0.push_back(5);
	id0.push_back(5);

	deque<int>::iterator itd;
	for (itd = id0.begin(); itd != id0.end(); ++itd)
		cout<<*itd<<" ";
	cout<<endl;
	
	itd = search_n(id0.begin(), id0.end(), 3, 2);
	if (itd != id0.end())
		cout<<"dis 3=2"<<distance(id0.begin(), itd)<<endl;
	
	itd = search_n(id0.begin(), id0.end(), 3, 2, greater<int>());
	if (itd != id0.end())
		cout<<"dis 3>2"<<distance(id0.begin(), itd)<<endl;

#if 0
	itd = search_n(id0.begin(), id0.end(), bind2nd(greater<int>(), 2));
	if (itd != id0.end())
		cout<<"dis 3>2"<<distance(id0.begin(), itd)<<endl;
#endif	
}

void min_max_test()
{
	deque<int> id0;
	
	for (int i = 2; i<8; ++i)
		id0.insert(id0.end(), i);
	for (int i = -3; i<5; ++i)
		id0.insert(id0.end(), i);

	deque<int>::iterator it = id0.begin();
	while(it != id0.end())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;

	it = min_element(id0.begin(), id0.end());
	cout<<"min "<<*it<<endl;

	it = max_element(id0.begin(), id0.end());
	cout<<"max "<<*it<<endl;

	it = min_element(id0.begin(), id0.end(), absint);
	cout<<"abs min "<<*it<<endl;	
}

void find_test()
{
	list<int> il0;

	for (int i = 0; i<5; ++i)
		il0.push_back(i);

	for (int i = 0; i<5; ++i)
		il0.push_back(i);

	list<int>::iterator it, it1, it2;
	for (it = il0.begin(); it != il0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	it1 = find(il0.begin(), il0.end(), 4);
	if (it1 == il0.end())
		return ;
	else
	{
		it2 = find(++it1, il0.end(), 4);
		if (it2 == il0.end())
			return;
		else
		{
			--it1;
			++it2;
			for (it = it1; it!=it2; ++it)
				cout<<*it<<" ";
		}
	}
	cout<<endl;

	it = find_if(il0.begin(), il0.end(), bind2nd(greater<int>(), 3));
	while(it != il0.end())
	{
		cout<<*it<<" ";
		it = find_if(++it, il0.end(), bind2nd(greater<int>(), 3));
	}
	cout<<endl;

	it = find_if(il0.begin(), il0.end(), bind2nd(modulus<int>(), 2));
	while(it != il0.end())
	{
		cout<<*it<<" ";
		it = find_if(++it, il0.end(), bind2nd(modulus<int>(), 2));
	}
	cout<<endl;

	it = find_if(il0.begin(), il0.end(), not1(bind2nd(modulus<int>(), 2)));
	while(it != il0.end())
	{
		cout<<*it<<" ";
		it = find_if(++it, il0.end(), not1(bind2nd(modulus<int>(), 2)));
	}
	cout<<endl;
}

void copy_test()
{
	vector<int> iv0;
	list<int>   il0;

	for (int i = 0; i<10; ++i)
		il0.push_back(i);

	for (list<int>::iterator it = il0.begin(); it != il0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	iv0.resize(10);
	copy(il0.begin(), il0.end(), iv0.begin());
	for (vector<int>::iterator it = iv0.begin(); it != iv0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;
	
	copy(il0.begin(), il0.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

#if 1
	copy(il0.rbegin(), il0.rend(), back_inserter(iv0));
	for (vector<int>::iterator it = iv0.begin(); it != iv0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	
#endif

	vector<int> iv1;
	vector<int> iv2;
	for (int i = 0; i<5; ++i)
		iv1.push_back(i);

	for(vector<int>::iterator it = iv1.begin(); it != iv1.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	iv2.resize(iv1.size());
	copy_backward(iv1.begin(), iv1.end(), iv2.end());
	for(vector<int>::iterator it = iv2.begin(); it != iv2.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;
}

void mismatch_test()
{
	vector<int> iv0;
	vector<int> iv1;

	for(int i = 0; i<5; ++i)
	{
		iv0.push_back(i);
		iv1.push_back(i+1);
	}

	pair<vector<int>::iterator, vector<int>::iterator> value;
	value = mismatch(iv0.begin(), iv0.end(), iv1.begin());
	if (value.first != iv0.end())
	{
		cout<<"mismatch first "<<*value.first<<" second "<<*value.second<<endl;
	}
}

void permutation_test()
{
	vector<int> iv0;
	vector<int> iv1;

	for (int i = 0; i<3; i++)
	{
		iv0.push_back(i);
		iv1.push_back(3 - i);
	}

	cout<<"iv0 "<<endl;
	for (vector<int>::iterator it = iv0.begin(); it != iv0.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	cout<<"next "<<endl;
	while(next_permutation(iv0.begin(), iv0.end()))
	{
		for (vector<int>::iterator it = iv0.begin(); it != iv0.end(); ++it)
			cout<<*it<<" ";
		cout<<endl;
	}

	cout<<"iv1 "<<endl;
	for (vector<int>::iterator it = iv1.begin(); it != iv1.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	cout<<"prev "<<endl;
	while(prev_permutation(iv1.begin(), iv1.end()))
	{
		for (vector<int>::iterator it = iv1.begin(); it != iv1.end(); ++it)
			cout<<*it<<" ";
		cout<<endl;
	}
}

