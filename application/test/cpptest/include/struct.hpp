#ifndef __STRUCT_HPP__
#define __STRUCT_HPP__

#include <iostream>
#include <vector>

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

struct TreeNode 
{
    int 			val;
    struct TreeNode *left;
    struct TreeNode *right;
	
    TreeNode(int x, TreeNode *l = NULL, TreeNode *r = NULL):val(x), left(l), right(r){}
};

struct TreeLinkNode
{
    int 				val;
    struct TreeLinkNode *left;
    struct TreeLinkNode *right;
	struct TreeLinkNode *next;
	
    TreeLinkNode(int x, TreeLinkNode *l = NULL, TreeLinkNode *r = NULL, TreeLinkNode *n = NULL):
		val(x), left(l), right(r), next(n){}	
};

class Solution_bt_traverse_print
{
public:
    std::vector<std::vector<int> > operator()(TreeNode* pRoot);
};

class Solution_bt_ztraverse_print 
{
public:
    std::vector<std::vector<int> > operator()(TreeNode* pRoot);
};

class Solution_bt_is_symmetrical
{
public:
    bool operator()(TreeNode* pRoot);
	
private:
    bool is_symmetrical(TreeNode *pRoot1, TreeNode *pRoot2);
};

class Solution_bt_isbalance_depth {
public:
    bool operator()(TreeNode* pRoot, int &depth);

private:
    bool IsBalanced(TreeNode *pRoot, int *depth);
};

class Solution_bt_print_t2b {
public:
    std::vector<int> operator()(TreeNode* root);
};

class Solution_bt_HasSubtree
{
public:
    bool operator()(TreeNode* pRoot1, TreeNode* pRoot2);
    
private:
    bool DoesTree1HaveTree2(TreeNode *Tree1, TreeNode *Tree2);
};

class Solution_mirror_recursive{
public:
    void operator()(TreeNode *pRoot);
};

class Solution_mirror_statck{
public:
	void operator()(TreeNode *pRoot);
};

class Solution_bt_reConstructBinaryTree {
public:
	TreeNode* operator()(std::vector<int> pre, std::vector<int> vin);
};

class Solution_bt_TreeDepth {
public:
    int operator()(TreeNode* pRoot);
};

class Solution_bt_TreeDepth_traverse {
public:
    int operator()(TreeNode* pRoot);
};

void mystack_test();

void mylist_test();
void mylist_reverse0(mylist<int>& l);
void mylist_reverse_print1(mylist<int>& l);
void mylist_reverse_print2(node<int> *head);
node<int>* mylist_end_k(mylist<int>& l, int k);
node<int>* mylist_first_common_node_test(mylist<int>& l1, mylist<int>& l2);
node<int>* mylist_ring_judge(mylist<int>& l);
int mylist_ring_len(node<int>* p_node);
node<int>* mylist_ring_entry(node<int>* p_node_head, node<int>* p_node_cur);
int mylist_merge_traverse(mylist<int>& l1, mylist<int>& l2, mylist<int>& l);
node<int>* mylist_merge_recursive(node<int> *p_node_1, node<int>* p_node_2);
node<int>* mylist_repeat_del(node<int>* p_node);

template<typename T>
void quicksort(T *array, int left, int right);
void quicksort_test();
void binary_tree_findpath_test();
void binary_tree_traverse_print();
void binary_tree_ztraverse_print_test();
void binary_tree_is_symmetrical_test();
void binary_tree_isbanance_depth_test();
void binary_tree_has_subtree(void);
void binary_tree_mirror_test();
void binary_tree_reconstruct_test();
void binary_tree_treedepth_test();

#endif
