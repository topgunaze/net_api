#ifndef __STRUCT_HPP__
#define __STRUCT_HPP__

#include <iostream>

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

template<typename T>
struct node
{
	T    	    data;
	struct node *next;
};

template<typename T>
class mylist
{
public:

	mylist():p_head(NULL), size(0){};
	~mylist(){};
	
	void initlist();
	void inserthead(node<T> *p_node);
	void inserttail(node<T> *p_node);
	void deletenode(node<T> *p_node);
	node<T> *searchlist(T value);
	void sortlist();
	
	void destroylist();
    void printlist();
	node<T>* gethead(){return p_head;}
	int getsize(){return size;}
	void sethead(node<T> *head){p_head = head;};
	
private:
	node<T> *p_head;
	int  	size;
};

void mystack_test();

void mylist_test();
void mylist_reverse0(mylist<int>& l);
void mylist_reverse_print1(mylist<int>& l);
void mylist_reverse_print2(node<int> *head);


#endif
