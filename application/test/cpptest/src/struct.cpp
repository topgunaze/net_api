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

		p_node->next = NULL;
		
		return;
	}

	while(p_cur != NULL)
	{
		if (p_cur == p_node)
		{
			p_tmp->next = p_cur->next;
			--size;

			p_node->next = NULL;
			
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

	size = 0;
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
	mylist<int> l0;
	mylist<int> l1;
	node<int> 	*p_node;
	node<int> 	*p_node_array0[20];
	node<int> 	*p_node_array1[20];

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		l0.inserttail(p_node);
		p_node_array0[i] = p_node;
	}

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		l1.inserthead(p_node);
		p_node_array1[i] = p_node;
	}
	
	l0.printlist();
	l1.printlist();

	l0.deletenode(p_node_array0[10]);

	l0.printlist();

	mylist_reverse_print1(l0);

	mylist_reverse_print2(l0.gethead());
	cout<<endl;

	mylist_reverse0(l0);

	l0.printlist();

	l1.deletenode(p_node_array1[0]);

	l1.printlist();

	cout<<"end k test"<<endl;
	node<int>* p_node_k;
	
	p_node_k = mylist_end_k(l1, 1);
	cout<<"end 1 "<<p_node_k->data<<endl;

	p_node_k = mylist_end_k(l1, 2);
	cout<<"end 2 "<<p_node_k->data<<endl;

	p_node_k = mylist_end_k(l1, l1.getsize());

	cout<<"end size "<<p_node_k->data<<endl;

	cout<<"first common test"<<endl;
	node<int> 	*p_node_array3[20];

	//list 相同
	mylist<int> lf0;
	mylist<int> lf1;
	
	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		p_node_array3[i] = p_node;
	}
		
	for (int i = 0; i<20; ++i)
	{
		lf0.inserttail(p_node_array3[i]);
		lf1.inserttail(p_node_array3[i]);
	}
	cout<<"before"<<endl;
	p_node = mylist_first_common_node_test(lf0, lf1);
	if (p_node)
		cout <<" node "<<p_node->data<<endl;
	else
		cout<<" not have "<<endl;

	//list 重合
	mylist<int> lf00;
	mylist<int> lf11;
	
	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		p_node_array3[i] = p_node;
	}

	for (int i = 0; i<20; ++i)
	{
		lf00.inserttail(p_node_array3[i]);
	}

	for(int i = 3; i<20; ++i)
	{
		lf11.inserttail(p_node_array3[i]);
	}

	p_node = mylist_first_common_node_test(lf00, lf11);
	if (p_node)
		cout <<" node "<<p_node->data<<endl;
	else
		cout<<" not have "<<endl;

	//list 部分重合
	mylist<int> lf000;
	mylist<int> lf111;
	
	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		p_node_array3[i] = p_node;
	}

	for (int i = 0; i<20; ++i)
	{
		lf000.inserttail(p_node_array3[i]);
	}

	for(int i = 5; i<20; ++i)
	{
		lf111.inserttail(p_node_array3[i]);
	}

	p_node = new node<int>;
	p_node->data = 10;
	p_node->next = NULL;
	
	lf111.inserthead(p_node);

	p_node = new node<int>;
	p_node->data = 10;
	p_node->next = NULL;

	lf111.inserthead(p_node);

	p_node = mylist_first_common_node_test(lf000, lf111);
	if (p_node)
		cout <<" node "<<p_node->data<<endl;
	else
		cout<<" not have "<<endl;

	cout<<"the ring judge "<<endl;

	mylist<int> lr0;
	node<int> 	*p_node_array4[20];
	int         len;

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		p_node_array4[i] = p_node;
	}

	lr0.inserttail(p_node_array4[0]);
	lr0.inserttail(p_node_array4[1]);
	lr0.inserttail(p_node_array4[2]);
	lr0.inserttail(p_node_array4[3]);
	lr0.inserttail(p_node_array4[4]);
	lr0.inserttail(p_node_array4[5]);	
	lr0.inserttail(p_node_array4[6]);	
	lr0.inserttail(p_node_array4[7]);

	p_node_array4[7]->next = p_node_array4[3];
	
	p_node = mylist_ring_judge(lr0);

	if (!p_node)
		cout<<" not have ring "<<endl;
	else
	{
		len = mylist_ring_len(p_node);
		p_node = mylist_ring_entry(lr0.gethead(), p_node);
		cout<<" the ring len "<<len<<" entry data "<<p_node->data<<endl;
	}

	cout<<"mylist_merge_traverse"<<endl;
	mylist<int> lm0;
	mylist<int> lm1;
	mylist<int> lm3;
	int			ret;
	node<int> 	*p_node_array5[20];

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i;
		p_node->next = NULL;
		p_node_array5[i] = p_node;
	}

	for (int i = 0; i<20; ++i)
	{
		if(i%2)
			lm0.inserttail(p_node_array5[i]);
		else
			lm1.inserttail(p_node_array5[i]);
	}

	lm0.printlist();
	lm1.printlist();

	ret = mylist_merge_traverse(lm0, lm1, lm3);
	if (ret)
		cout<<"list merge fail "<<endl;
	else
	{
		cout<<"list merge suc"<<endl;
		lm0.printlist();
		lm1.printlist();
		lm3.printlist();
	}

	cout<<"mylist_merge_recursive "<<endl;
	for (int i = 0; i<20; ++i)
	{
		if(i%2)
			lm0.inserttail(p_node_array5[i]);
		else
			lm1.inserttail(p_node_array5[i]);
	}

	node<int> *p_node_first = lm0.gethead(), *p_node_second = lm1.gethead();
	lm0.printlist();
	lm1.printlist();
	
	lm0.initlist();
	lm1.initlist();
	
	p_node = mylist_merge_recursive(p_node_first, p_node_second);
	while(p_node)
	{
		cout<<p_node->data<<" ";
		p_node = p_node->next;
	}
	
	lm0.printlist();
	lm1.printlist();
	
	cout<<endl;

	cout<<"list repeat del"<<endl;
	mylist<int> ld1;

	for (int i = 0; i<20; ++i)
	{
		p_node = new node<int>;
		p_node->data = i/2;
		p_node->next = NULL;

		ld1.inserttail(p_node);
	}

	ld1.printlist();
	
	p_node = mylist_repeat_del(ld1.gethead());
	while(p_node)
	{
		cout<<p_node->data<<" ";
		p_node = p_node->next;
	}

	cout<<endl;
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

//end k
node<int>* mylist_end_k(mylist<int>& l, int k)
{
	//node<int> *p_node_k;
	//node<int> *p_node;
	node<int> *p_node_k, *p_node;

	p_node_k = p_node = l.gethead();

	if (!p_node || !k)
		return NULL;

	--k;
	while(k && p_node)
	{
		p_node = p_node->next;
		--k;
	}

	if (!p_node)
	{
		return NULL;
	}

	while(p_node->next)
	{
		p_node 	 = p_node->next;
		p_node_k = p_node_k->next;
	}

	return p_node_k;
}

//the first common node
node<int>* mylist_first_common_node_test(mylist<int>& l1, mylist<int>& l2)
{
	int diff;
	node<int> *p_node_long, *p_node_short;

	if (l1.getsize() >= l2.getsize())
	{
		p_node_long  = l1.gethead();
		p_node_short = l2.gethead();

		diff = l1.getsize() - l2.getsize();
	}
	else
	{
		p_node_long  = l2.gethead();
		p_node_short = l1.gethead();

		diff = l2.getsize() - l1.getsize();
	}

	while(diff)
	{
		p_node_long = p_node_long->next;
		--diff;
	}

	while(p_node_long && p_node_short)
	{
		if (p_node_long == p_node_short)
			return p_node_long;

		p_node_long  = p_node_long->next;
		p_node_short = p_node_short->next;
	}


	return NULL;
}

//ring judge
node<int>* mylist_ring_judge(mylist<int>& l)
{
	node<int> *p_node_fast, *p_node_slow;

	p_node_fast = p_node_slow = l.gethead();

	if (!p_node_fast)
		return NULL;

	while(p_node_fast && p_node_slow)
	{
		if (p_node_slow->next)
			p_node_slow = p_node_slow->next;
		else
			break;

		if (p_node_fast->next && p_node_fast->next->next)
			p_node_fast = p_node_fast->next->next;
		else
			break;

		if (p_node_fast == p_node_slow)
			return p_node_fast;
	}

	return NULL;
}

int mylist_ring_len(node<int>* p_node)
{
	int count = 0;
	node<int> *p_tmp = p_node;

	while(p_tmp)
	{
		++count;
		p_tmp = p_tmp->next;

		if (p_tmp == p_node)
		{
			return count;
		}
	}

	return count;
}

node<int>* mylist_ring_entry(node<int>* p_node_head, node<int>* p_node_cur)
{
	node<int> *p_node_first = p_node_head,  *p_node_second = p_node_cur;

	if (!p_node_head || !p_node_cur)
		return NULL;

	//len = nR-x = (n-1)R + (R-x)
	while(p_node_first != p_node_second)
	{
		p_node_first  = p_node_first->next;
		p_node_second = p_node_second->next;
	}

	return p_node_first;
}

int mylist_merge_traverse(mylist<int>& l1, mylist<int>& l2, mylist<int>& l)
{
	node<int> *p_node_1 = l1.gethead(), *p_node_2 = l2.gethead();
	
	if (!p_node_1 && !p_node_2)
	{
		return -1;
	}

	while(p_node_1 && p_node_2)
	{
		if (p_node_1->data <= p_node_2->data)
		{
			l1.deletenode(p_node_1);
			l.inserttail(p_node_1);
			p_node_1 = l1.gethead();
		}
		else
		{
			l2.deletenode(p_node_2);
			l.inserttail(p_node_2);
			p_node_2 = l2.gethead();
		}
	}

	//cout<<"merge test"<<endl;
	//l1.printlist();
	//l2.printlist();
	//l.printlist();

	while(p_node_1)
	{
		l1.deletenode(p_node_1);
		l.inserttail(p_node_1);
		p_node_1 = l1.gethead();
	}

	while(p_node_2)
	{
		l2.deletenode(p_node_2);
		l.inserttail(p_node_2);
		p_node_2 = l2.gethead();
	}

	return 0;
}

node<int>* mylist_merge_recursive(node<int> *p_node_1, node<int>* p_node_2)
{
	node<int> *p_node;

	if (!p_node_1)
		return p_node_2;
	else if (!p_node_2)
		return p_node_1;
	
	if (p_node_1->data <= p_node_2->data)
	{
		p_node = p_node_1;
		p_node->next = mylist_merge_recursive(p_node_1->next, p_node_2);
	}
	else
	{
		p_node = p_node_2;
		p_node->next = mylist_merge_recursive(p_node_1, p_node_2->next);
	}

	return p_node;
}

node<int>* mylist_repeat_del(node<int> *p_node)
{
	node<int> *p_node_head = p_node, *p_node_cur = p_node, *p_node_next;
	
	if (!p_node_cur)
	{
		return NULL;
	}
	
	p_node_next = p_node->next;
	if (!p_node_next)
		return p_node_cur;

	while(p_node_next)
	{
		if (p_node_cur->data == p_node_next->data)
		{
			p_node_cur->next = p_node_next->next;
			//p_node_next del
			p_node_next = p_node_next->next;
		}
		else
		{
			p_node_cur  = p_node_cur->next;
			p_node_next = p_node_next->next; 
		}	
	}

	return p_node_head;
}

template<typename T>
void quicksort(T *array, int left, int right)
{
    int l = left, r = right;
	T 	tmp, base = array[left];
	
    if(l >= r)
       return;
                                   
    while(l != r)
    {
		//顺序很重要，要先从右边开始找
		while(array[r] >= base && l < r)
			r--;
		
		//再找右边的
		while(array[l] <= base && l < r)
			l++;
		
		//交换两个数在数组中的位置
		if(l < r)
		{
			tmp  	 = array[l];
			array[l] = array[r];
			array[r] = tmp;
		}
	}
	
    //最终将基准数归位
    array[left] = array[l];
    array[l]	= base;
                                 
    quicksort(array, left, l-1);//继续处理左边的，这里是一个递归的过程
    quicksort(array, l+1, right);//继续处理右边的 ，这里是一个递归的过程
}

void quicksort_test()
{
	int array[10] = {3, 2, 1 , 1, 13 ,11, 34, 5, 13, 99};

	quicksort<int>(array, 0, 9);

	for(int i = 0; i<10;++i)
	{
		cout<<array[i]<<" ";
	}
	cout<<endl;
}

