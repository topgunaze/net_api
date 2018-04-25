#include "struct.hpp"
#include<stack>
#include <queue>

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

void findPath(TreeNode *pRoot, vector<vector<int> > &pathes, vector<int> &onePath, int expectNumber, int &curSum)
{
	curSum += pRoot->val;
	onePath.push_back(pRoot->val);

	bool isLeaf = false;
	if(pRoot->left == NULL && pRoot->right == NULL)
		isLeaf = true;
	
	if(isLeaf && curSum == expectNumber)
	{
		pathes.push_back(onePath);
	}

	if(pRoot->left != NULL)
		findPath(pRoot->left, pathes, onePath, expectNumber, curSum);
	
	if(pRoot->right != NULL)
		findPath(pRoot->right, pathes, onePath, expectNumber, curSum);

	curSum -= pRoot->val;
	onePath.pop_back();
}

vector<vector<int> > binary_tree_findpath(TreeNode* root, int expectNumber)
{
	vector<vector<int> > pathes;
	if(root == NULL)
		return pathes;

	vector<int> onePath;
	int curSum = 0;
	findPath(root, pathes, onePath, expectNumber, curSum);

	return pathes;	
}

void binary_tree_findpath_test()
{
	TreeNode t8(8, NULL, NULL);
	
	TreeNode t7(7, NULL, NULL);
	TreeNode t6(6, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, &t8, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);

	vector<vector<int> > ivv = binary_tree_findpath(&t1, 4);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}


vector<vector<int> > Solution_bt_traverse_print::operator()(TreeNode *pRoot)
{
	vector<vector<int> > result;
	if(pRoot == NULL)
	    return result;

	std::queue<TreeNode*> nodes;
	nodes.push(pRoot);
	int nextLevel = 0;
	int toBePrinted = 1;

	vector<int> oneRow;

	while(!nodes.empty())
	{
	    TreeNode* pNode = nodes.front();
	    oneRow.push_back(pNode->val);

	    if(pNode->left != NULL)
	    {
	        nodes.push(pNode->left);
	        ++nextLevel;
	    }
		
	    if(pNode->right != NULL)
	    {
	        nodes.push(pNode->right);
	        ++nextLevel;
	    }

	    nodes.pop();
	    --toBePrinted;
		
	    if(toBePrinted == 0)
	    {
	        result.push_back(oneRow);
	        oneRow.clear();
			
	        toBePrinted = nextLevel;
	        nextLevel = 0;
	    }
	}

	return result;
}

void binary_tree_traverse_print()
{
	TreeNode t8(8, NULL, NULL);
	
	TreeNode t7(7, NULL, NULL);
	TreeNode t6(6, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, &t8, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);

	Solution_bt_traverse_print obj;
	
	vector<vector<int> > ivv = obj(&t1);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}

vector<vector<int> > Solution_bt_ztraverse_print::operator()(TreeNode* pRoot) 
{
    vector<vector<int> > result;
    if(pRoot == NULL)
        return result;

    stack<TreeNode*> levels[2];
    int current = 0;
    int next = 1;

    levels[current].push(pRoot);
    vector<int> oneRow;
	
    while(!levels[0].empty() || !levels[1].empty())
    {
        TreeNode *pNode = levels[current].top();
        levels[current].pop();
        oneRow.push_back(pNode->val);

        if(current == 0)
        {
            if(pNode->left != NULL)
                levels[next].push(pNode->left);
			
            if(pNode->right != NULL)
                levels[next].push(pNode->right);
        }
        else
        {
            if(pNode->right != NULL)
                levels[next].push(pNode->right);
			
            if(pNode->left != NULL)
                levels[next].push(pNode->left);
        }
		
        if(levels[current].empty())
        {
            result.push_back(oneRow);
            oneRow.clear();
			
            current = 1 - current;
            next = 1 - next;
        }
    }
	
    return result;
}

void binary_tree_ztraverse_print_test()
{
	TreeNode t8(8, NULL, NULL);
	
	TreeNode t7(7, NULL, NULL);
	TreeNode t6(6, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, &t8, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);


	Solution_bt_ztraverse_print obj;
	
	vector<vector<int> > ivv = obj(&t1);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}

bool Solution_bt_is_symmetrical::operator()(TreeNode* pRoot)
{
    return is_symmetrical(pRoot->left, pRoot->right);
}

bool Solution_bt_is_symmetrical::is_symmetrical(TreeNode *pRoot1, TreeNode *pRoot2)
{
    if(pRoot1 == NULL && pRoot2 == NULL)
        return true;
	
    if(pRoot1 == NULL || pRoot2 == NULL)
        return false;
	
    if(pRoot1->val != pRoot2->val)
        return false;
	
    return is_symmetrical(pRoot1->left, pRoot2->right) && is_symmetrical(pRoot1->right, pRoot2->left);
}

void binary_tree_is_symmetrical_test()
{

	TreeNode t7(4, NULL, NULL);
	TreeNode t6(5, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, NULL, NULL);
	
	TreeNode t3(2, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);


	Solution_bt_is_symmetrical obj;
	
	bool flag = obj(&t1);
	if (flag)
		cout<<" is_symmetrical"<<endl;

	else
		cout<<" not is_symmetrical"<<endl;

	TreeNode tt7(4, NULL, NULL);
	TreeNode tt6(65, NULL, NULL);
	
	TreeNode tt5(5, NULL, NULL);
	TreeNode tt4(4, NULL, NULL);
	
	TreeNode tt3(2, &tt6, &tt7);
	TreeNode tt2(2, &tt4, &tt5);
	TreeNode tt1(1, &tt2, &tt3);
	
	flag = obj(&tt1);
	if (flag)
		cout<<" is_symmetrical"<<endl;

	else
		cout<<" not is_symmetrical"<<endl;
}

class Solution_nextnode {
public:
	TreeLinkNode* GetNext(TreeLinkNode* pNode)
	{
		if(pNode == NULL)
			return NULL;

		TreeLinkNode* pNext = NULL;
		if(pNode->right != NULL)
		{
			TreeLinkNode* pRight = pNode->right;
			while(pRight->left != NULL)
				pRight = pRight->left;

			pNext = pRight;
		}
		else if(pNode->next != NULL)
		{
			TreeLinkNode* pCurrent = pNode;
			TreeLinkNode* pParent = pNode->next;
			while(pParent != NULL && pCurrent == pParent->right)
			{
				pCurrent = pParent;
				pParent = pParent->next;
			}

			pNext = pParent;
		}

		return pNext;
	}
};

bool Solution_bt_isbalance_depth::operator()(TreeNode* pRoot, int &depth) 
{
    if(pRoot == NULL)
        return true;
	
    depth = 0;
	
    return IsBalanced(pRoot, &depth);
}

bool Solution_bt_isbalance_depth::IsBalanced(TreeNode *pRoot, int *depth)
{
    if(pRoot == NULL)
    {
        *depth = 0;
        return true;
    }
	
    int left, right;
	
    if(IsBalanced(pRoot->left, &left) && IsBalanced(pRoot->right, &right))
    {
        int diff = left - right;
        if(diff <= 1 && diff >= -1)
        {
            *depth = left > right? (left+1) : (right+1);
            return true;
        }
    }
	
    return false;
}

void binary_tree_isbanance_depth_test()
{
	TreeNode t8(4, NULL, NULL);

	TreeNode t7(4, NULL, NULL);
	TreeNode t6(5, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, &t8, NULL);
	
	TreeNode t3(2, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);


	Solution_bt_isbalance_depth obj;
	int depth;
	
	bool flag = obj(&t1, depth);
	if (flag)
		cout<<" is_balance "<<depth<<endl;

	else
		cout<<" not is_balance"<<depth<<endl;	
}

vector<int> Solution_bt_print_t2b::operator()(TreeNode* root)
{
	vector<int> result;
	if(root == NULL)
	    return result;

	queue<TreeNode *> nodes;
	nodes.push(root);

	while(!nodes.empty())
	{
	    TreeNode *pNode = nodes.front();
	    result.push_back(pNode->val);

	    if(pNode->left != NULL)
	        nodes.push(pNode->left);
		
	    if(pNode->right != NULL)
	        nodes.push(pNode->right);

	    nodes.pop();
	}

	return result;
}

bool Solution_bt_HasSubtree::operator()(TreeNode* pRoot1, TreeNode* pRoot2)
{
    bool result = false;
	
    if(pRoot1 != NULL && pRoot2 != NULL)
    {
        if(pRoot1->val == pRoot2->val)
            result = DoesTree1HaveTree2(pRoot1, pRoot2);
		
        if(!result)
            result = operator()(pRoot1->left, pRoot2);
		
        if(!result)
            result = operator()(pRoot1->right, pRoot2);
    }
	
    return result;
}
	
bool Solution_bt_HasSubtree::DoesTree1HaveTree2(TreeNode *Tree1, TreeNode *Tree2)
{
    if(Tree2 == NULL)
        return true;
	
    if(Tree1 == NULL)
        return false;
	
    if(Tree1->val != Tree2->val)
        return false;

    return DoesTree1HaveTree2(Tree1->left, Tree2->left) &&
        DoesTree1HaveTree2(Tree1->right, Tree2->right);
}

void binary_tree_has_subtree(void)
{
	//TreeNode t8(4, NULL, NULL);

	TreeNode t7(4, NULL, NULL);
	TreeNode t6(5, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, NULL, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);

	
	TreeNode tt3(5, NULL, NULL);
	TreeNode tt2(4, NULL, NULL);
	TreeNode tt1(2, &tt2, &tt3);

	Solution_bt_HasSubtree obj;
	
	bool flag = obj(&t1, &tt1);
	if (flag)
		cout<<" is_subtree "<<endl;

	else
		cout<<" not is_subtree"<<endl;		
}

void Solution_mirror_recursive::operator()(TreeNode *pRoot) 
{
    if(pRoot == NULL || (pRoot->left == NULL && pRoot->right == NULL))
        return;

    if(pRoot->left != NULL)
        operator()(pRoot->left);
	
    if(pRoot->right != NULL)
        operator()(pRoot->right);
	
    TreeNode *temp = pRoot->left;
    pRoot->left = pRoot->right;
    pRoot->right = temp;
}

void Solution_mirror_statck::operator()(TreeNode *pRoot) 
{
	if(pRoot == NULL || (pRoot->left == NULL && pRoot->right == NULL))
		return;

	stack<TreeNode*> stackNodes;
	stackNodes.push(pRoot);

	while(stackNodes.size() > 0)
	{
	    TreeNode *pNode = stackNodes.top();
	    stackNodes.pop();

	    TreeNode *pTemp = pNode->left;
	    pNode->left = pNode->right;
	    pNode->right = pTemp;

	    if(pNode->left != NULL)
	        stackNodes.push(pNode->left);
		
	    if(pNode->right != NULL)
	        stackNodes.push(pNode->right);
	}
}

void binary_tree_mirror_test()
{
	TreeNode t7(7, NULL, NULL);
	TreeNode t6(6, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, NULL, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);

	cout<<"old "<<endl;
	
	Solution_mirror_recursive obj1;

	Solution_bt_traverse_print obj_print;
	
	vector<vector<int> > ivv = obj_print(&t1);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}

	cout<<"mirror first"<<endl;
	obj1(&t1);

	ivv = obj_print(&t1);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}

	cout<<"mirror second"<<endl;
	Solution_mirror_statck obj2;
	obj2(&t1);

	ivv = obj_print(&t1);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}

class Solution_VerifySquenceOfBST {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if(sequence.size()<=0)
            return false;
        int start=0, end=sequence.size()-1;
        return isLastOrder(sequence, start, end);
    }

private:
    bool isLastOrder(vector<int> &sequence, int start, int end)
    {
        if(start > end)
            return false;

        int root = sequence[end];
        int i = start;
        for(; i<end; i++)
        {
            if(sequence[i]>root)
                break;
        }
        int j = i;
        for(; j<end; j++)
        {
            if(sequence[j]<root)
                return false;
        }
        bool left = true;
        if(i-1 > start)
            left = isLastOrder(sequence, start, i-1);

        bool right = true;
        if(i < end-1)
            right = isLastOrder(sequence, i, end-1);

        return(left && right);
    }

};

/*
输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。
假设输入的前序遍历和中序遍历的结果中都不含重复的数字。
例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
*/

 /* 思路（递归）：根据前序遍历的第一个数字创建根节点；
 在中序便利找到根节点的位置；
 确定左右子树节点数量；递归构建左右子树；
 */

TreeNode* Solution_bt_reConstructBinaryTree::operator()(vector<int> pre, vector<int> vin) 
{
    if(pre.empty() || vin.empty() || pre.size() != vin.size())
        return NULL;

    vector<int> left_pre, right_pre, left_vin, right_vin;
    TreeNode *node = new TreeNode(pre[0]);

    size_t left_length = 0;
    while(pre[0] != vin[left_length] && left_length < pre.size())
        ++left_length;

    for(size_t i = 0; i<left_length; i++)
    {
        left_pre.push_back(pre[i+1]);
        left_vin.push_back(vin[i]);
    }

    for(size_t i = left_length+1; i < pre.size(); i++)
    {
        right_pre.push_back(pre[i]);
        right_vin.push_back(vin[i]);
    }
	
    node->left = operator()(left_pre, left_vin);
    node->right = operator()(right_pre, right_vin);

    return node;
}

void binary_tree_reconstruct_test()
{
	vector<int> iv1;
	vector<int> iv2;

	iv1.push_back(1);
	iv1.push_back(3);
	iv1.push_back(4);

	iv2.push_back(3);
	iv2.push_back(1);
	iv2.push_back(4);

	TreeNode    *p_tree;
	
	Solution_bt_reConstructBinaryTree obj;
	p_tree = obj(iv1, iv2);

	Solution_bt_traverse_print obj_print;

	vector<vector<int> > ivv = obj_print(p_tree);

	for (vector<vector<int> >::iterator vit = ivv.begin(); vit != ivv.end(); ++vit)
	{
		cout<<"the next:"<<endl;
		for (vector<int>::iterator it = (*vit).begin(); it != (*vit).end(); ++it)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}

int Solution_bt_TreeDepth::operator()(TreeNode* pRoot)
{
    if(pRoot == NULL)
        return 0;
	
    int left = operator()(pRoot->left);
    int right = operator()(pRoot->right);

    return left >= right ? (left+1):(right+1);
}

int Solution_bt_TreeDepth_traverse::operator()(TreeNode* pRoot)
{
    queue<TreeNode*> q;
    if(!pRoot)
        return 0;
	
    q.push(pRoot);
    int level=0;
    while(!q.empty())
    {
        size_t len = q.size();
        level++;
		
        while(len--)
        {
            TreeNode *tmp = q.front();
            q.pop();
			
            if(tmp->left)
                q.push(tmp->left);
			
            if(tmp->right)
                q.push(tmp->right);
        }
    }
	
    return level;
}

void binary_tree_treedepth_test()
{
	TreeNode t8(8, NULL, NULL);

	TreeNode t7(7, NULL, NULL);
	TreeNode t6(6, NULL, NULL);
	
	TreeNode t5(5, NULL, NULL);
	TreeNode t4(4, &t8, NULL);
	
	TreeNode t3(3, &t6, &t7);
	TreeNode t2(2, &t4, &t5);
	TreeNode t1(1, &t2, &t3);

	
	Solution_bt_TreeDepth obj1;

	Solution_bt_TreeDepth_traverse obj2;

	int dep = obj1(&t1);
	cout<<dep<<endl;
	
	dep = obj2(&t1);
	cout<<dep<<endl;
	
}

