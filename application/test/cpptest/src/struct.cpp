#include "struct.hpp"
#include<stack>

using namespace std;

template <typename T>
mystack<T>::mystack(int size)
{
	this->size   = size;
	p_base = new T[size];
	top    = 0;
}

template <typename T>
mystack<T>::~mystack()
{
	delete []p_base;
	top    = 0;
	size   = 0;
}

template <typename T>
bool mystack<T>::isfull()
{
	return top == size ? true : false;
}

template <typename T>
bool mystack<T>::isempty()
{
	return top == 0 ? true : false;
}

template <typename T>
T mystack<T>::pop()
{
	return p_base[--top];
}

template<typename T>
bool mystack<T>::push(T data)
{
	if (isfull())
		return false;
	
	p_base[top++] = data;
	return true;
}

template<typename T>
void mylist<T>::initlist()
{
	p_head = NULL;
	size   = 0;
}

template<typename T>
void mylist<T>::inserthead(node<T> *p_node)
{
	if (p_node == NULL)
		return ;

	if (p_head)
	{
		p_node->next = p_head;
		p_head = p_node;
	}
	else
	{
		p_head = p_node;
		p_head->next = NULL;
	}

	++size;
}

template<typename T>
void mylist<T>::inserttail(node<T> *p_node)
{
	node<T>* p_node_tmp = p_head;
	
	if (p_node == NULL)
		return ;

	if (p_head)
	{
		while(p_node_tmp->next)
			p_node_tmp = p_node_tmp->next;

		p_node_tmp->next = p_node;
		p_node->next = NULL;
	}
	else
	{
		p_head = p_node;
		p_head->next = NULL;
	}

	++size;
}

template<typename T>
void mylist<T>::deletenode(node<T> *p_node)
{
	node<T>* p_tmp = p_head;
	node<T>* p_cur = p_head->next;

	if (p_node == NULL)
		return ;
	
	if (p_node == p_head)
	{
		p_head = p_head->next;
		--size;
		delete p_node;
		
		return;
	}

	while(p_cur != NULL)
	{
		if (p_cur == p_node)
		{
			p_tmp->next = p_cur->next;
			--size;
			delete p_cur;
			
			return;
		}
		else
		{
			p_tmp = p_cur;
			p_cur = p_cur->next;
		}
	}

	return;
}

template<typename T>
node<T> *mylist<T>::searchlist(T value)
{
	node<T> *p_tmp = p_head;
	
	while(p_tmp)
	{
		if (p_tmp->data == value)
			return p_tmp;
		else
			p_tmp = p_tmp->next;
	}

	return NULL;
}

template<typename T>
void mylist<T>::sortlist()
{

}

template<typename T>
void mylist<T>::destroylist()
{
	node<T> *p_tmp;
	
	while(p_head)
	{
		p_tmp = p_head; 
		p_head = p_head->next;
		delete p_tmp;
	}
}

template<typename T>
void mylist<T>::printlist()
{
	node<T> *p_tmp = p_head;
	
	while(p_tmp)
	{
		cout<<p_tmp->data<<" ";
		p_tmp = p_tmp->next;
	}

	cout<<endl;
}

void mystack_test()
{
	mystack<double> s(100);

	if (!s.isfull())
		s.push(10.4);

	if (!s.isfull())
		s.push(9.4);

	if (!s.isfull())
		s.push(7.4);

	if (!s.isfull())
		s.push(1.4);

	while(!s.isempty())
		cout<<s.pop()<<" ";	
}

void mylist_test()
{
	mylist<int> l;
	node<int> 	*p_node;
	node<int> 	*p_node_array[20];

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		l.insertlist(p_node);
		p_node_array[i] = p_node;
	}
	
	l.printlist();

	l.deletenode(p_node_array[10]);

	l.printlist();

	mylist_reverse_print1(l);

	mylist_reverse_print2(l.gethead());
	cout<<endl;

	mylist_reverse0(l);

	l.printlist();
	
}

//修改了链表结构
void mylist_reverse0(mylist<int>& l)
{
	node<int> *p_node = l.gethead();
	node<int> *p_node_pre = NULL;
	node<int> *p_node_next = p_node->next;

	if (!p_node)
		return;

	if (!p_node->next)
		return;

	p_node->next = NULL;
	while(p_node_next)
	{
		p_node_pre = p_node;
		p_node = p_node_next;
		p_node_next = p_node_next->next;
		p_node->next = p_node_pre;
	}

	l.sethead(p_node);
}

//不修改链表结构,栈
void mylist_reverse_print1(mylist<int>& l)
{
	stack<node<int>*> is;
	node<int>  *p_node = l.gethead();

	while(p_node)
	{
		is.push(p_node);
		p_node = p_node->next;
	}

	while(!is.empty())
	{
		p_node = is.top();
		cout<<p_node->data<<" ";
		is.pop();
	}

	cout<<endl;
}

//不修改链表结构,栈->递归
void mylist_reverse_print2(node<int> *head)
{
	if (!head)
		return;

	mylist_reverse_print2(head->next);
	cout<<head->data<<" ";
}

