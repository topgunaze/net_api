#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <list>
#include <unordered_set>

using namespace std;

class Solution01 {
public:
	bool Find(vector<vector<int> > array, int target) {
		int row = array.size();
		if (row == 0)
			return false;

		int col = array[0].size();
		int i = 0, j = col - 1;
		while (i < row && j >= 0){
			if (array[i][j] == target)
				return true;
			else if (array[i][j] > target){
				j--;
			}
			else{
				i++;
			}			
		}

		return false;
	}

public:
	void test(){
		vector <vector<int>> array1{{ 1, 2, 3, 4, 5 },
		{ 6, 7, 8, 9, 10 },
		{ 11, 12, 13, 14, 15 },
		{ 16, 17, 18, 19, 20 }};

		vector<int> target{ 1, 2, 3, 10, 0, -1, 32, 34, 20, 16, 15, 6 };
		for (int i = 0; i != target.size(); i++){
			cout << "target " << target[i] << boolalpha << "    std:" << (1 <= target[i] && target[i] <= 20) << "  res:"
				<< Find(array1, target[i]) << endl;
		}
	}

};

class Solution02 {
public:
	void replaceSpace(char *str, int length) {
		//find space num
		int count = 0;
		for (int i = 0; i < length; i++) {
			if (str[i] == ' ')
				count++;
		}

		int new_length = length + 2 * count;
		int j = new_length - 1;
		for (int i = length - 1; i >= 0; i--) {
			if (count == 0)
				break;

			if (str[i] == ' ') {
				str[j--] = '0';
				str[j--] = '2';
				str[j--] = '%';
				count--;
			}
			else {
				str[j--] = str[i];
			}
		}
	}

	void test() {
		vector<string> vec{ "We are Happy", "hello",
			"", "  hlee", "hll  ", "  srfe  ", "  se  sd " };
		for (int i = 0; i != vec.size(); i++) {
			char buf[256] = { 0 };
			strcpy(buf, vec[i].c_str());
			replaceSpace(buf, vec[i].size());
			cout << vec[i] << "   " << buf << endl;
		}
	}
};

class Solution03 {
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};

public:
	vector<int> printListFromTailToHead(struct ListNode* head) {
		vector<int> res;
		stack<int> mystack;
		while (head) {
			mystack.push(head->val);
			head = head->next;
		}

		while (!mystack.empty()) {
			res.push_back(mystack.top());
			mystack.pop();
		}

		return res;
	}

	void display(const vector<int> & res) {
		for (auto item : res)
			cout << item << ' ';
		cout << endl;
	}

	void test() {
		ListNode myHead(0), a(1), b(2), c(3), d(4), e(5), f(6);
		a.next = &b; b.next = &c; c.next = &d;
		auto res = printListFromTailToHead(&a);
		display(res);

		res = printListFromTailToHead(&myHead);
		display(res);

		res = printListFromTailToHead(nullptr);
		display(res);

		e.next = &f;
		res = printListFromTailToHead(&e);
		display(res);
	}
};

class Solution04 {
	//Definition for binary tree
	struct TreeNode {
		int val;
		TreeNode *left;
		TreeNode *right;
		TreeNode(int x) : val(x), left(NULL), right(NULL) {}
	};

public:
	struct TreeNode* reConstructBinaryTree(vector<int> pre, vector<int> in) {
		bool valid = true;
		auto res = reConstructBinaryTree(pre, in, valid);
		if (!valid)
			return nullptr;
		return res;
	}

	struct TreeNode* reConstructBinaryTree(vector<int> & pre, vector<int> & in, bool & valid) {
		struct TreeNode * res = nullptr;
		if (!valid || pre.size() != in.size() || pre.size() == 0)
			return res;

		int split = pre[0];
		int split_in = 0;
		bool flag = false;
		for (int i = 0; i != in.size(); i++){
			if (split == in[i]){
				split_in = i;
				flag = true;
				break;
			}
		}

		if (!flag){
			valid = false;
			return res;
		}

		vector<int> pre_prefix(pre.begin() + 1, pre.begin() + split_in + 1);
		vector<int> pre_suffix(pre.begin() + split_in + 1, pre.end());


		vector<int> in_prefix(in.begin(), in.begin() + split_in);
		vector<int> in_suffix(in.begin() + split_in + 1, in.end());
		struct TreeNode * left = reConstructBinaryTree(pre_prefix, in_prefix, valid);
		struct TreeNode * right = reConstructBinaryTree(pre_suffix, in_suffix, valid);
		res = new TreeNode(split);
		res->left = left;
		res->right = right;

		return res;
	}

	void inorder(TreeNode * root, vector<int> & arr){
		if (root){
			inorder(root->left, arr);
			arr.push_back(root->val);
			inorder(root->right, arr);
		}
	}

	void test(){
		vector<vector<int>> pre{ { 1, 2, 4, 7, 3, 5, 6, 8 },
		{}, { 5 }, { 1, 2 }, { 1 }, {1, 3} };
		vector<vector<int>> in{ { 4, 7, 2, 1, 5, 3, 8, 6 },
		{}, { 5 }, { 1, 2 }, {}, {1, 2} };
		for (int i = 0; i != pre.size(); i++){
			TreeNode * res = reConstructBinaryTree(pre[i], in[i]);
			vector<int> arr;
			inorder(res, arr);
			for (auto item : arr)
				cout << item << " ";
			cout << endl;
		}
	}
};

class Solution05
{
public:
	void push(int node) {
		stack1.push(node);
	}

	int pop() {
		if (stack2.empty()){
			while (!stack1.empty()){
				stack2.push(stack1.top());
				stack1.pop();
			}
		}

		if (stack2.empty())
			return -1;

		int res = stack2.top();
		stack2.pop();
		return res;
	}

	void test(){
		vector<vector<int>> arr{ { 1, 2, 3, 4, 5, 6 }, { 5, 3, 6 }, { 1, 2 }, { 0 }, {}, { 1, -1, 2 } };
		for (int i = 0; i != arr.size(); i++){
			for (int j = 0; j != arr[i].size(); j++){
				push(arr[i][j]);
			}

			for (int j = 0; j != arr[i].size(); j++){
				cout << pop() << " ";
			}
			cout << endl;
		}
	}

private:
	stack<int> stack1;
	stack<int> stack2;
};

class Solution06_ {
public:
	int minNumberInRotateArray(vector<int> rotateArray) {
		if (rotateArray.size() == 0)
			return 0;

		int istart = 0, istop = rotateArray.size() - 1;
		while (istart < istop){
			if (istart + 1 == istop)
				return min(rotateArray[istart], rotateArray[istop]);

			int start = rotateArray[istart];
			int stop = rotateArray[istop];
			int imid = istart + ((istop - istart) >> 1);
			int mid = rotateArray[imid];
			if (start < mid && mid > stop){
				istart = imid + 1;
			}
			else if (start > stop && mid < stop){
				istop = imid;
			}
			else{
				int mymin = INT_MAX;
				for (int j = istart; j != istop + 1; j++){
					mymin = min(mymin, rotateArray[j]);
				}
				return mymin;
			}
				
		}
		return rotateArray[istart];
	}

	void test(){
		vector<vector<int>> arr{ { 1, 2, 3, 4, 5 }, { 3, 4, 5, 1, 2 },
		{}, { 1 }, { 2, 1 }, { 1, 2 }, { 3, 1, 2 }, {1, 2, 2, 3, 4,4, 5, 5, 5, 6, 6, 6} , 
		{1, 1, 1,1, 1,1,1} };
		for (int i = 0; i != arr.size(); i++){
			cout << minNumberInRotateArray(arr[i]) << endl;
		}
	}
};

class Solution07 {
public:
	int Fibonacci(int n) {
		if (n < 0)
			throw new exception("invalid input");
		if (n == 0)
			return 0;

		vector<int> arr(50, 1);
		for (int i = 3; i <= n; i++){
			arr[i] = arr[i - 1] + arr[i - 2];
		}
		return arr[n];
	}	

	void test(){
		for (int i = 0; i < 40; i++){
			cout << Fibonacci(i) << endl;
		}
	}
};

class Solution08 {
public:
	int jumpFloor(int number) {
		if (number <= 0)
			return 0;

		vector<int> arr(number + 1, 1);
		for (int i = 2; i <= number; i++){
			arr[i] = arr[i - 1] + arr[i - 2];
		}
		return arr[number];
	}

	void test(){
		for (int i = 0; i != 40; i++){
			cout << jumpFloor(i) << endl;
		}
	}
};

class Solution09 {
public:
	int jumpFloorII(int number) {
		if (number <= 0)
			return 0;

		return pow(2, number - 1);
	}

	void test(){
		for (int i = 0; i != 40; i++){
			cout << jumpFloorII(i) << endl;
		}
	}
};

class Solution10 {
public:
	int rectCover(int number) {
		if (number <= 0)
			return 0;

		vector<int> arr(number + 1, 1);
		for (int i = 2; i <= number; i++){
			arr[i] = arr[i - 1] + arr[i - 2];
		}
		return arr[number];
	}

	void test(){
		for (int i = 0; i != 40; i++){
			cout << rectCover(i) << endl;
		}
	}
};

class Solution11 {
public:
	int  NumberOf1(int n) {
		int count = 0;
		while (n){
			n = n & (n - 1);
			count++;
		}

		return count;
	}

	void test(){
		for (int i = 0; i != 10; i++)
			cout << NumberOf1(i) << endl;
	}
};

class Solution12 {
public:
	double Power(double base, int exponent) {
		if (exponent == 0)
			return 1.0;		

		if (fabs(base) < 1e-5)
			return 0.0;

		if (fabs(base - 1) < 1e-5)
			return 1;

		if (fabs(base + 1) < 1e-5)
			return exponent % 2 ? -1 : 1;

		if (exponent < 0){			
			base = 1.0 / base;
			exponent = -exponent;
		}

		double res = 1.0;
		while (exponent){
			if (exponent % 2){
				res *= base;
			}

			base *= base;
			exponent >>= 1;
		}
		return res;
	}

	void test(){
		vector<double> base{ 0, 1, -1, -2, 2, 4, 0.1, 10 };
		vector<int> exp{ 1, 0, 4, 3, 0, -5, 6, -1 };
		for (int i = 0; i != base.size(); i++){
			cout << Power(base[i], exp[i]) << endl;
		}
	}
};

class Solution13 {
public:
	void reOrderArray01(vector<int> &array) {
		stable_sort(array.begin(), array.end(), cmp);
	}

	void reOrderArray(vector<int> &array) {
		int len = array.size();
		if (len <= 1)
			return;

		vector<int> first(array.begin(), array.begin() + len / 2);
		vector<int> second(array.begin() + len / 2, array.end());

		reOrderArray(first);
		reOrderArray(second);

		int i = 0, j = 0;
		while (i < first.size() || j < second.size()){
			int a = (i < first.size()) ? first[i] : INT_MAX / 2 * 2;
			int b = (j < second.size()) ? second[j] : INT_MAX / 2 * 2;
			if (cmp(a, b)){
				array[i + j] = a;
				i++;
			}
			else{
				array[i + j] = b;
				j++;
			}				
		}
	}

	static bool cmp(const int & a, const int & b){
		if (a % 2 == 1 && b % 2 == 0)
			return true;
		else if (a % 2 == 0 && b % 2 == 1)
			return false;
		else
			return a < b;
	}

	void test(){
		vector<vector<int>> arr{ {}, { 1 }, { 1, 2 }, { 1, 2, 3, 4, 5, 6 } };
		for (int i = 0; i != arr.size(); i++){
			reOrderArray(arr[i]);
			for (int j = 0; j != arr[i].size(); j++)
				cout << arr[i][j] << " ";
			cout << endl;
		}
	}
};

class Solution14 {
	
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};

public:
	ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
		ListNode * p1 = pListHead, *p2 = pListHead;
		for (int i = 0; i != k; i++){
			if (p2 == nullptr)
				return nullptr;

			p2 = p2->next;
		}

		while (p2){
			p2 = p2->next;
			p1 = p1->next;
		}

		return p1;
	}

	void test(){
		ListNode myhead(0), a(1), b(2), c(3), d(4), e(5), f(6);
		a.next = &b, b.next = &c, c.next = &d;
		e.next = &f;
		ListNode * res = nullptr;
		for (int i = 0; i != 7; i++){
			cout << "1\t"<<i <<"  "<< ((res = FindKthToTail(&a, i)) ? res->val : -1) << endl;
			cout << "2\t" << i << "  " << ((res = FindKthToTail(&e, i)) ? res->val : -1) << endl;
			cout << "3\t" << i << "  " << ((res = FindKthToTail(&myhead, i)) ? res->val : -1) << endl;
		}
			
		
	}
};

class Solution15 {
	
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};

public:
	ListNode* ReverseList(ListNode* pHead) {
		ListNode * p = nullptr;
		ListNode * pnext = nullptr;
		while (pHead){
			pnext = pHead->next;
			pHead->next = p;
			p = pHead;
			pHead = pnext;
		}
		
		return p;
	}

	void test(){
		ListNode myhead(0), a(1), b(2), c(3), d(4), e(5), f(6);
		a.next = &b, b.next = &c, c.next = &d;
		e.next = &f;
		vector<ListNode *> vec{ &myhead, &a, &e };
		ListNode * res = nullptr;
		for (int i = 0; i != vec.size(); i++){
			res = ReverseList(vec[i]);
			while (res){
				cout << res->val << " ";
				res = res->next;
			}
			cout << endl;
		}
	}
};

class Solution16____ {	
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};
public:
	ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
	{
		ListNode myHead(0);
		ListNode * p = &myHead;
		while (pHead1 || pHead2){
			if (pHead1 == nullptr){
				p->next = pHead2;
				break;
			}
			else if (pHead2 == nullptr){
				p->next = pHead1;
				break;
			}
			else{
				if (pHead1->val < pHead2->val){
					p->next = pHead1;
					pHead1 = pHead1->next;					
				}
				else{
					p->next = pHead2;
					pHead2 = pHead2->next;
				}
				p = p->next;
			}
		}

		return myHead.next;
	}

	void display(ListNode * res){
		while (res){
			cout << res->val << " ";
			res = res->next;
		}
		cout << endl;
	}

	void test(){
		ListNode myhead(0), myhead1(0), a(1), b(2), c(3), d(4), e(5), f(6);
		a.next = &b, b.next = &c, c.next = &d;
		e.next = &f;
		vector<ListNode *> vec{ &myhead, &a,};
		vector<ListNode *> vec1{ nullptr,  &e };
		ListNode * res = nullptr;
		for (int i = 0; i != vec.size(); i++){
			res = Merge(vec[i], vec1[i]);
			display(res);
		}
	}
};

class Solution17 {
	
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
	{
		if (pRoot2 == nullptr || pRoot1 == nullptr)
			return false;

		return _HasSubtree(pRoot1, pRoot2);
	}

	bool isSame(TreeNode * p1, TreeNode * p2){
		if (p1 == nullptr && p2)
			return false;
		if (p2 == nullptr)
			return true;
		if (p1->val != p2->val)
			return false;
		else
			return isSame(p1->left, p2->left) && isSame(p1->right, p2->right);
	}

	bool _HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
	{
		if (isSame(pRoot1, pRoot2))
			return true;
		else
			return isSame(pRoot1->left, pRoot2) || isSame(pRoot1->right, pRoot2);
	}

	void test(){
		TreeNode a(5), b(4), c(1), d(2), e(3), f(0);
		TreeNode a1(4), b1(1), c1(2), d1(1);
		a.left = &b, b.left = &c, b.right = &d, a.right = &e;
		a1.left = &b1, a1.right = &c1;
		cout << HasSubtree(&a, &a1) << endl;
		cout << HasSubtree(&a, nullptr) << endl;
		cout << HasSubtree(nullptr, &a1) << endl;
		cout << HasSubtree(&a, &a1) << endl;
	}
};

class Solution18 {
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	void Mirror(TreeNode *pRoot) {
		if (pRoot == nullptr)
			return;

		Mirror(pRoot->left);
		Mirror(pRoot->right);
		TreeNode * tmp = pRoot->right;
		pRoot->right = pRoot->left;
		pRoot->left = tmp;
	}

	void display(TreeNode * root){
		if (root == nullptr)
			return;

		display(root->left);
		cout << root->val << " ";
		display(root->right);
	}

	void test(){
		TreeNode a(8), b(6), c(10), d(5), e(7), f(9), g(11), h(1);
		a.left = &b, a.right = &c, b.left = &d, b.right = &e, c.left = &f, c.right = &g;
		vector<TreeNode*> arr{ &a, &h, nullptr };
		for (int i = 0; i != arr.size(); i++){
			Mirror(arr[i]);
			display(arr[i]);
		}
	}
};

class Solution19 {
public:
	vector<int> printMatrix(vector<vector<int> > matrix) {
		vector<int> res;
		int row = matrix.size();
		if (row <= 0)
			return res;

		int col = matrix[0].size();

		vector<int> vec;
		for (int i = 0; i < (min(row, col) + 1)/ 2; i++){
			printAround(matrix, i, row, col, vec);			
		}
			
		return vec;
	}

	void printAround(vector<vector<int> > & matrix, int gap, int height, int width, vector<int> & res){
		if (height <= width && height == 2 * gap + 1){
			for (int i = 0; i < width - 2 * gap; i++){
				res.push_back(matrix[gap][gap + i]);
			}
			return;
		}
		else if (width < height && width == 2 * gap + 1){
			for (int i = 0; i < height - 2 * gap; i++){
				res.push_back(matrix[gap + i][width - 1 - gap]);
			}
			return;
		}

		for (int i = 0; i < width - 2 * gap - 1; i++){
			res.push_back(matrix[gap][gap + i]);
		}
		for (int i = 0; i < height - 2 * gap - 1; i++){
			res.push_back(matrix[gap + i][width - 1 - gap]);
		}
		for (int i = 0; i < width - 2 * gap - 1; i++){
			res.push_back(matrix[height - 1 - gap][width - 1 - gap - i]);
		}
		for (int i = 0; i < height - 2 * gap - 1; i++){
			res.push_back(matrix[height - 1 - gap - i][gap]);
		}
	}

	void test(){
		vector<vector<vector<int>>> matrix{ { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } },
		{ { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 }, { 16, 17, 18, 19, 20 }, { 21, 22, 23, 24, 25 } },
		{{ 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 }, { 16, 17, 18, 19, 20 } },
		{ { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 }, { 16, 17, 18, 19, 20 }, { 21, 22, 23, 24, 25 },
		{ 26, 27, 28, 29, 30 }, {31, 32, 33, 34, 35} },
		{}};

		for (int i = 0; i != matrix.size(); i++){
			auto res = printMatrix(matrix[i]);
			for (auto item : res)
				cout << item << " ";
			cout << endl;
		}
	}
};

class Solution20 {
public:
	void push(int value) {
		mystack.push(value);
		if (!minstack.empty()){
			minstack.push(std::min(minstack.top(), value));
		}
		else{
			minstack.push(value);
		}
	}
	void pop() {
		mystack.pop();
		minstack.pop();
	}
	int top() {
		return mystack.top();
	}
	int min() {
		return minstack.top();
	}

private:
	stack<int> mystack;
	stack<int> minstack;

	void test(){

	}
};

class Solution21 {
public:
	bool IsPopOrder(vector<int> pushV, vector<int> popV) {
		stack<int> mystack;
		int ipush = 0;
		for (int i = 0; i != popV.size(); i++){
			while (mystack.empty() || mystack.top() != popV[i]){
				if (ipush >= pushV.size())
					return false;

				mystack.push(pushV[ipush]);
				ipush++;
			}

			if (mystack.top() == popV[i]){
				mystack.pop();
			}
		}

		return true;
	}

	void test(){
		vector<vector<int>> pushV{ { 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5 }, {}, { 1 } };
		vector<vector<int>> popV{ { 4, 5, 3, 2, 1 }, { 4, 3, 5, 1, 2 }, {}, { 2 } };
		for (int i = 0; i != popV.size(); i++){
			cout << boolalpha << IsPopOrder(pushV[i], popV[i]) << endl;
		}
	}
};

class Solution22 {
	
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	vector<int> PrintFromTopToBottom(TreeNode *root) {
		vector<int> res;
		if (root == nullptr)
			return res;

		queue<TreeNode *> myqueue;
		myqueue.push(root);
		while (!myqueue.empty()){
			TreeNode * front = myqueue.front();
			res.push_back(front->val); 
			myqueue.pop();
			if (front->left)
				myqueue.push(front->left);
			if (front->right)
				myqueue.push(front->right);
		}

		return res;
	}

	void test(){
		TreeNode a(1), b(2), c(3), d(4), e(5), f(6), g(7), h(0);
		a.left = &b, a.right = &c, b.left = &d, b.right = &e, c.left = &f, c.right = &g;
		vector<TreeNode *> vec{ &a, &h, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = PrintFromTopToBottom(vec[i]);
			for (auto item : res)
				cout << item << " ";
			cout << endl;
		}
	}
};

class Solution23 {

public:
	bool VerifySquenceOfBST(vector<int> sequence) {
		if (sequence.size() <= 0)
			return false;

		return _VerifySquenceOfBST(sequence);
	}

	bool _VerifySquenceOfBST(vector<int> sequence) {
		if (sequence.size() <= 1)
			return true;

		int split_i = 0;
		int last = sequence.back();
		bool flag = false;
		for (int i = 0; i < sequence.size() - 1; i++) {
			if (sequence[i] >= last) {
				flag = true;
				split_i = i;
				break;
			}
		}

		if (!flag) {
			split_i = sequence.size() - 1;
		}

		for (int i = split_i; i < sequence.size() - 1; i++) {
			if (sequence[i] <= last) {
				return false;
			}
		}

		vector<int> prefix(sequence.begin(), sequence.begin() + split_i);
		vector<int> suffix(sequence.begin() + split_i, sequence.end() - 1);

		return _VerifySquenceOfBST(prefix) && _VerifySquenceOfBST(suffix);
	}

	void test() {
		vector<vector<int>> vec{ { 2, 4, 5, 6, 3 },{},{ 12, 32, 3, 4, 5, 6 },{ 1 },{ 1, 3 } };
		for (int i = 0; i != vec.size(); i++) {
			cout << boolalpha << VerifySquenceOfBST(vec[i]) << endl;
		}

	}
};

class Solution24 {
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	vector<vector<int> > FindPath(TreeNode* root, int expectNumber) {
		vector<vector<int> > res;
		vector<int> path;
		if (root == nullptr)
			return res;

		Search(root, expectNumber, res, path);
		return res;
	}

	void Search(TreeNode * root, int expectNumber, vector<vector<int> > & res, vector<int> & path) {
		if (root->left == nullptr && root->right == nullptr) {
			if (root->val == expectNumber) {
				path.push_back(root->val);
				res.push_back(path);
				path.pop_back();
			}

			return;
		}

		path.push_back(root->val);
		if (root->left)
			Search(root->left, expectNumber - root->val, res, path);
		if (root->right)
			Search(root->right, expectNumber - root->val, res, path);
		path.pop_back();
	}

	void test() {
		TreeNode a(3), b(2), c(5), d(4), f(6), g(8);
		a.left = &b, a.right = &c, c.left = &d, c.right = &f;

		vector<TreeNode *> arr{ &a, &g, nullptr };
		for (int i = 0; i != arr.size(); i++) {
			auto res = FindPath(arr[i], 5);
			for (auto items : res) {
				for (auto item : items) {
					cout << item << " ";
				}
				cout << endl;
			}
		}
	}
};


class Solution25 {

	struct RandomListNode {
		int label;
		struct RandomListNode *next, *random;
		RandomListNode(int x) :
			label(x), next(NULL), random(NULL) {
		}
	};

public:
	RandomListNode* Clone(RandomListNode* pHead)
	{		
		RandomListNode *p = pHead, *pnext = nullptr, *pnew = nullptr;

		while (p){
			pnext = p->next;
			pnew = new RandomListNode(p->label);
			p->next = pnew;
			pnew->next = pnext;
			p = pnext;
		}

		p = pHead;
		while (p){
			if (p->random)
				p->next->random = p->random->next;

			p = p->next->next;
		}

		p = pHead;
		RandomListNode myHead(0);
		RandomListNode * pstart = &myHead;
		while (p){
			pstart->next = p->next;
			pstart = pstart->next;
			pnext = p->next->next;
			p->next = pnext;
			p = pnext;
		}

		return myHead.next;
	}

	void display(RandomListNode * list){
		while (list){
			cout << list->label << "  ";
			list = list->next;
		}
		cout << endl;
	}

	void test(){
		RandomListNode a(1), b(2), c(3), d(4), e(5), f(6);
		a.next = &b, b.next = &c, c.next = &d, d.next = &e;
		vector<RandomListNode *> vec{ &a, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = Clone(vec[i]);
			display(res);
		}
	}
};

class Solution26 {

	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	TreeNode* Convert(TreeNode* pRootOfTree)
	{
		if (pRootOfTree == nullptr)
			return nullptr;

		TreeNode myhead(0);
		TreeNode * pstart = &myhead;
		inorder(pRootOfTree, pstart);
		myhead.right->left = nullptr;
		return myhead.right;
	}

	void inorder(TreeNode * root, TreeNode * & pstart){
		if (root){
			inorder(root->left, pstart);
			pstart->right = root;
			root->left = pstart;
			pstart = pstart->right;
			inorder(root->right, pstart);
		}
	}

	void display(TreeNode * root){
		while (root){
			cout << root->val << " ";
			root = root->right;
		}
		cout << endl;
	}

	void test(){
		TreeNode a(1), b(2), c(3), d(4), e(5), f(6), g(7), h(0);
		a.left = &b, a.right = &c, b.left = &d, b.right = &e, c.left = &f, c.right = &g;
		vector<TreeNode *> vec{ &a, &h, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = Convert(vec[i]);
			display(res);
		}
	}
};

class Solution27__ {
public:
	vector<string> Permutation(string str) {
		vector<string> vec;
		if (str == "")
			return vector<string>();
		
		search(str, 0, vec);
		sort(vec.begin(), vec.end());
		return vec;
	}

	void search(string str, int depth, vector<string> & vec){
		if (depth == str.size()){
			vec.push_back(str);
			return;
		}

		for (int i = depth; i != str.size(); i++){
			if (i != depth && str[i] == str[depth])
				continue;
			swap(str[i], str[depth]);
			search(str, depth + 1, vec);
			swap(str[i], str[depth]);
		}
	}

	void test(){
		vector<string> vec{ "abc", "aac", "", "a" };
		for (int i = 0; i != vec.size(); i++){
			auto res = Permutation(vec[i]);
			for (auto item : res){
				cout << item << "   ";
			}
			cout << endl;
		}
	}
};

class Solution28 {
public:
	int MoreThanHalfNum_Solution(vector<int> numbers) {
		if (numbers.size() <= 0)
			return 0;

		int most = numbers[0];
		int count = 1;
		for (int i = 1; i != numbers.size(); i++){
			if (most == numbers[i]){
				count++; 
			}
			else{
				if (count > 0)
					count--;
				else{
					count = 1;
					most = numbers[i];
				}
			}
		}

		count = 0;
		for (int i = 0; i != numbers.size(); i++){
			if (numbers[i] == most)
				count++;
		}

		return (count > numbers.size() / 2) ? most : 0;
	}

	void test(){
		vector<vector<int>> vec{ { 1, 2, 3, 2, 2, 2, 5, 4, 2 }, {}, { 1 }, {1, 2, 2, 1} };
		for (auto item : vec){
			cout << MoreThanHalfNum_Solution(item) << endl;
		}
	}
};

class Solution29___ {
public:
	vector<int> GetLeastNumbers_Solution(vector<int> input, int k) {
		vector<int> res;
		if (input.size() <= 0 || k <= 0 || k > input.size())
			return res;

		selectLeastNumbers(input, k, 0, input.size() - 1);
		return vector<int>(input.begin(), input.begin() + k);
	}

	void selectLeastNumbers(vector<int> & input, int k, int start, int stop){
		int part = partition(input, start, stop);
		if (k == part)
			return;

		if (k < part){
			stop = part - 1;
		}
		else{
			start = part + 1;
		}
		selectLeastNumbers(input, k, start, stop);
	}

	int partition(vector<int> & input, int start, int stop){
		if (input.size() <= 0)
			return 0;

		int privot = input[start];

		while (start < stop){
			while (start < stop && input[stop] >= privot){
				stop--;
				continue;
			}

			if (start < stop && input[stop] < privot)
				swap(input[stop], input[start]); 

			while (start < stop && input[start] <= privot){
				start++;
				continue;
			}

			if (start < stop && input[start] > privot)
				swap(input[start], input[stop]);
		}

		return start;
	}

	void test(){
		vector<vector<int>> vec{ { 4, 5, 1, 6, 2, 7, 3, 8 }, {}, { 1, 2, 3 }, { 4, 5, 1, 6, 2, 7, 3, 8 } };
		for (int i = 0; i != vec.size(); i++){
			auto res = GetLeastNumbers_Solution(vec[i], 1);
			for (auto item : res){
				cout << item << " ";
			}
			cout << endl;
		}
	}
};

class Solution30 {
public:
	int FindGreatestSumOfSubArray(vector<int> array) {
		if (array.size() <= 0)
			return 0;

		vector<long> arr(array.size(), INT_MIN);
		arr[0] = array[0];
		for (int i = 1; i != array.size(); i++){
			arr[i] = max(arr[i - 1] + array[i], (long)array[i]);
		}

		return *max_element(arr.begin(), arr.end());
	}

	void test(){
		vector<vector<int>> vec{ { 6, -3, -2, 7, -15, 1, 2, 2 }, {}, { 1 } };
		for (int i = 0; i != vec.size(); i++){
			cout << FindGreatestSumOfSubArray(vec[i]) << endl;
		}
	}
};

class Solution31_____ {
public:
	int NumberOf1Between1AndN_Solution(int n)
	{
		if (n == 0)
			return 0;

		int res = 0;
		int len = NumberLength(n);
		// 首位
		int first = n / pow(10, len - 1);
		if ( first == 1){
			res += n - pow(10, len - 1) + 1;
		}
		else{
			res += pow(10, len - 1);
		}

		// 中间位
		if (len >= 2)
			res += (first) * pow(10, len - 2) * (len - 1);

		res += NumberOf1Between1AndN_Solution(n - first * pow(10, len - 1));
		return res;
	}

	int NumberLength(int n){
		int count = 0;
		while (n){
			count++;
			n = n / 10;
		}
		return count;
	}

	void test(){
		for (int i = 0; i != 120; i++){
			cout << i << " ========= " << NumberOf1Between1AndN_Solution(i) << endl;
		}
	}
};

class Solution32 {
public:
	string PrintMinNumber(vector<int> numbers) {
		sort(numbers.begin(), numbers.end(), cmp);
		string res;
		for (auto item : numbers){
			res += to_string(item);
		}
		return res;
	}

	static bool cmp(const int & a, const int & b){
		double resab = a * pow(10, getLen(b)) + b;
		double resba = b * pow(10, getLen(a)) + a;
		return resab < resba;
	}

	static int getLen(int a){
		int count = 0;
		while (a){
			count++;
			a /= 10;
		}
		return count;
	}

	void test(){
		vector<vector<int>> numbers{ { 3, 32, 321 }, {}, { 1, 2, 3 } };
		for (auto item : numbers){
			cout << PrintMinNumber(item) << endl;
		}
	}
};

class Solution33 {
public:
	int GetUglyNumber_Solution(int index) {
		if (index <= 0)
			return 0;

		queue<int> a2, a3, a5;
		a2.push(2), a3.push(3), a5.push(5);

		int id = 1;
		int res = 1;
		while (id < index){
			res = min(a2.front(), min(a3.front(), a5.front()));
			a2.push(res * 2);
			a3.push(res * 3);
			a5.push(res * 5);
			if (a2.front() == res)
				a2.pop();
			if (a3.front() == res)
				a3.pop();
			if (a5.front() == res)
				a5.pop();

			id++;
		}

		return res;
	}

	void test(){
		for (int i = 0; i != 10; i++){
			cout << GetUglyNumber_Solution(i) << endl;
		}
	}
};

class Solution34 {
public:
	int FirstNotRepeatingChar(string str) {
		unordered_map<char, int> mymap;
		for (int i = 0; i != str.size(); i++){
			if (mymap.count(str[i]) > 0)
				mymap[str[i]] = -1;
			else
				mymap.insert(std::pair<char, int>(str[i], i));

		}

		int mymin = INT_MAX;
		bool flag = false;
		for (auto item : mymap){
			if (item.second >= 0){
				flag = true;
				mymin = min(mymin, item.second);
			}
		}

		return flag ? mymin : 0;
	}

	void test(){
		vector<string> strs{ "hello", "", "aasdssad", "asdad", "google", "gos" };
		for (int i = 0; i != strs.size(); i++){
			cout << FirstNotRepeatingChar(strs[i]) << endl;
		}
	}
};

class Solution35______ {
public:
	int InversePairs(vector<int> data) {
		if (data.size() <= 0)
			return 0;
		return _InversePairs(data);
	}

	long _InversePairs(vector<int> & data) {
		if (data.size() <= 1)
			return 0;

		vector<int> left(data.begin(), data.begin() + data.size() / 2);
		vector<int> right(data.begin() + data.size() / 2, data.end());

		long left_count = _InversePairs(left);
		long right_count = _InversePairs(right);

		// merge
		long mid = merge(left, right, data);
		return (left_count + right_count + mid) % 1000000007;
	}

	long merge(vector<int> & left, vector<int> & right, vector<int> & out){
		int pleft = left.size() - 1;
		int pright = right.size() - 1;
		int out_i = out.size() - 1;

		long count = 0;
		while (pleft >= 0 || pright >= 0){
			if (pleft < 0){
				while (pright >= 0){
					out[out_i--] = right[pright--];
				}
				break;
			}
			else if (pright < 0){
				while (pleft >= 0){
					out[out_i--] = left[pleft--];
				}
				break;
			}
			else{
				if (left[pleft] < right[pright]){
					out[out_i--] = right[pright--];
				}
				else {
					out[out_i--] = left[pleft--];
					count += pright + 1;
				}
			}
		}
		return count % 1000000007;
	}

	int InversePairs_n_2(vector<int> data) {
		vector<long> arr(data.size() + 1, 0);
		vector<long> min_arr(data.size() + 1, INT_MAX);
		min_arr[0] = -1;
		for (int i = 0; i != data.size(); i++){
			long sum = 0;
			for (int j = i - 1; j >= 0; j--){
				if (data[i] < min_arr[j]){
					sum += j + 1;
					break;
				}
				else{
					if (data[i] < data[j])
						sum++;
				}
			}

			arr[i + 1] = arr[i] + sum;
			min_arr[i + 1] = min(min_arr[i], (long)data[i]);
		}

		return arr[data.size()] % 1000000007;
	}

	void test(){
		vector<vector<int>> data{ { 1, 2, 3, 4, 5, 6, 7, 0 }, {}, { 1, 2 }, { 2, 1 } };
		for (int i = 0; i != data.size(); i++){
			cout << InversePairs(data[i]) << endl;
		}
	}
}; 

class Solution36 {
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};

public:
	ListNode* FindFirstCommonNode(ListNode *pHead1, ListNode *pHead2) {
		int left = getLen(pHead1);
		int right = getLen(pHead2);
		while (left < right){
			pHead2 = pHead2->next;
			right--;
		}
		while (left > right){
			pHead1 = pHead1->next;
			left--;
		}
		while (pHead1 != pHead2){
			pHead2 = pHead2->next;
			pHead1 = pHead1->next;
		}

		return pHead1;
	}

	int getLen(ListNode * p){
		int count = 0;
		while (p){
			count++;
			p = p->next;
		}
		return count;
	}

	void display(ListNode * p){
		while (p){
			cout << p->val << "  ";
			p = p->next;
		}
		cout << endl;
	}

	void test(){
		ListNode a(1), b(2), c(3), d(4), e(5), f(6), g(7);
		a.next = &b, b.next = &c, c.next = &d;
		e.next = &c;
		vector<ListNode *> left{ &a, &f, &g, nullptr };
		vector<ListNode *> right{ &e, &f, nullptr, &a };
		for (int i = 0; i != left.size(); i++){
			auto res = FindFirstCommonNode(left[i], right[i]);
			display(res);
		}
	}
};

class Solution37 {
public:
	int GetNumberOfK(vector<int> data, int k) {
		if (data.size() <= 0)
			return 0;

		int start = 0, stop = data.size() - 1, mid = 0;
		bool flag = false;
		while (start < stop){
			mid = start + ((stop - start) >> 1);
			if (data[mid] == k){
				flag = true;
				break;
			}
				

			if (data[mid] < k){
				start = mid + 1;
			}
			else{
				stop = mid - 1;
			}
		}

		// calc
		if (!flag){
			if (data[start] != k)
				return 0;
			else
				mid = start;
		}

		int count = 0;
		for (int i = mid; i < data.size(); i++){
			if (data[i] == k)
				count++;
			else
				break;
		}

		for (int i = mid - 1; i >= 0; i--){
			if (data[i] == k)
				count++;
			else
				break;
		}
		
		return count;
	}

	void test(){
		vector<vector<int>> arr{ { 1, 2, 3, 4, 5, 6, 7, 8 }, {}, { 1 } };
		for (int i = 0; i != arr.size(); i++){
			cout << GetNumberOfK(arr[i], 1) << endl;
		}
	}
};

class Solution38 {
	
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	int TreeDepth(TreeNode* pRoot)
	{
		if (pRoot == nullptr)
			return 0;

		int left = TreeDepth(pRoot->left);
		int right = TreeDepth(pRoot->right);
		return max(left, right) + 1;
	}

	void test(){
		TreeNode a(1), b(2), c(3), d(4), e(5), f(6), g(7);
		a.left = &b, a.right = &c, b.left = &d, b.right = &e;
		f.left = &g;
		vector<TreeNode *> vec{ &a, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			cout << TreeDepth(vec[i]) << endl;
		}
	}
};

class Solution39 {
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	bool IsBalanced_Solution(TreeNode* pRoot) {
		int height = 0;
		return IsBalanced_Solution(pRoot, height);
	}

	bool IsBalanced_Solution(TreeNode* pRoot, int & height){
		if (pRoot == nullptr){
			height = 0;
			return true;
		}

		int left_h = 0, right_h = 0;
		bool left = IsBalanced_Solution(pRoot->left, left_h);
		if (!left)
			return false;
		bool right = IsBalanced_Solution(pRoot->right, right_h);
		if (!right)
			return false;

		height = max(left_h, right_h) + 1;
		return abs(left_h - right_h) <= 1;
	}

	void test(){
		TreeNode a(1), b(2), c(3), d(4), e(5), f(6), g(7);
		a.left = &b, a.right = &c, b.left = &d, b.right = &e;
		f.left = &g;
		vector<TreeNode *> vec{ &a, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			cout << IsBalanced_Solution(vec[i]) << endl;
		}
	}
};

class Solution40 {
public:
	void FindNumsAppearOnce(vector<int> data, int* num1, int *num2) {
		int n = 0;
		for (auto item : data){
			n ^= item;
		}
		int split = n - (n & (n - 1));
		for (auto item : data){
			if ((item & split) == 0){
				*num1 ^= item;
			}
			else{
				*num2 ^= item;
			}
		}
	}

	void test(){
		vector<vector<int>> vec{ { 1, 1, 2, 2, 3, 4, 5, 5, 6, 6 }, {} };
		for (int i = 0; i != vec.size(); i++){
			int num1 = 0, num2 = 0;
			FindNumsAppearOnce(vec[i], &num1, &num2);
			cout << num1 << "    " << num2 << endl;
		}
	}
};

class Solution41 {
public:
	vector<vector<int> > FindContinuousSequence(int sum) {
		queue<int> myqueue;
		vector<vector<int> > res;
		int start = 1, stop = 2, cur = 3;
		myqueue.push(1);
		myqueue.push(2);
		while (stop <= (sum + 1) / 2){
			if (cur == sum){
				vector<int> temp;
				for (int i = start; i <= stop; i++){
					temp.push_back(i);
				}
				res.push_back(temp);
				cur = cur - start + stop + 1;
				start++;
				stop++;
			}
			else if (cur < sum){
				cur += stop + 1;
				stop++;
			}
			else{
				cur -= start;
				start++;
			}
		}
		return res;
	}

	void test(){
		vector<int> sum{ 1, 2, 3, 5, 10, 100 };
		for (int i = 0; i < sum.size(); i++){
			cout << "============"<<sum[i] << "========" << endl;
			auto res = FindContinuousSequence(sum[i]);
			for (auto item : res){
				for (auto subitem : item){
					cout << subitem << "  ";
				}
				cout << endl;
			}			
		}
	}
};

class Solution42 {
public:
	vector<int> FindNumbersWithSum(vector<int> array, int sum) {
		int start = 0, stop = array.size() - 1;
		while (start < stop){
			int cur = array[start] + array[stop];
			if (cur == sum){
				return vector<int>{array[start], array[stop]};
			}
			else if (cur < sum){
				start++;
			}
			else{
				stop--;
			}
		}
		return vector<int>();
	}

	void test(){
		vector<vector<int>> array{{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }, {}, { 1 }, {1, 2}};
		for (int i = 0; i < array.size(); i++){
			auto res = FindNumbersWithSum(array[i], 3);
			if (res.size() >= 2)
				cout << res[0] << "====" << res[1] << endl;
		}


	}
};

class Solution43 {
public:
	string LeftRotateString(string str, int n) {
		int len = str.size();
		if (len <= 0 || n % len == 0)
			return str;

		n = n % len;
		ReverseString(str, 0, n - 1);
		ReverseString(str, n, len - 1);
		ReverseString(str, 0, len - 1);
		return str;
	}

	void ReverseString(string & str, int start, int stop){
		while (start < stop){
			swap(str[start++], str[stop--]);
		}
	}

	void test(){
		vector<string> arr{ "abcXYZdef", "", "1" };
		for (int i = 0; i != arr.size(); i++){
			cout << arr[i] << " ============= " << LeftRotateString(arr[i], 3) << endl;
		}
	}
};

class Solution44 {
public:
	string ReverseSentence(string str) {
		int len = str.size();
		if (len <= 0)
			return str;

		int start = 0, stop = 0;
		for (int i = 0; i < len; i++){
			if (str[i] == ' ' && str[start] != ' '){
				stop = i;
				ReverseString(str, start, stop - 1);
				start = stop + 1;
			}
			else if (str[i] == ' '){
				start = i;
			}
		}
		ReverseString(str, start, len - 1);
		ReverseString(str, 0, len - 1);
		return str;
	}

	void ReverseString(string & str, int start, int stop){
		while (start < stop){
			swap(str[start++], str[stop--]);
		}
	}

	void test(){
		vector<string> arr{ "abcXYZdef", "", "1" , "student. a am I"};
		for (int i = 0; i != arr.size(); i++){
			cout << arr[i] << " ============= " << ReverseSentence(arr[i]) << endl;
		}
	}
};

class Solution45 {
public:
	bool IsContinuous(vector<int> numbers) {
		if (numbers.size() != 5)
			return false;

		int count_0 = 0;
		int num_max = INT_MIN, num_min = INT_MAX;
		bool duplicated = false;

		for (int i = 0; i != numbers.size(); i++){
			if (numbers[i] == 0){
				count_0++;
			}
			else {
				num_max = max(num_max, numbers[i]);
				num_min = min(num_min, numbers[i]);
				for (int j = i - 1; j >= 0; j--){
					if (numbers[j] == numbers[i]){
						duplicated = true;
						return false;
					}
				}
			}
		}

		if (num_max - num_min > 4)
			return false;

		return true;
	}

	void test(){
		vector<vector<int>> vec{ { 1, 2, 3, 4, 5 }, { 0, 0, 0, 0, 1 }, { 1, 3, 4, 6, 0 } };
		for (int i = 0; i != vec.size(); i++){
			cout << IsContinuous(vec[i]) << endl;
		}
	}
};

class Solution46_____ {
	struct ListNode{
		struct ListNode * next;
		unsigned int val;

		ListNode(unsigned int val) :val(val), next(nullptr){}
	};

public:
	int LastRemaining_Solution(unsigned int n, unsigned int m)
	{
		if (n <= 0 || m <= 0)
			return 0;

		ListNode myHead(0);
		ListNode * p = &myHead;
		for (unsigned int i = 0; i != n; i++){
			ListNode * pnew = new ListNode(i);
			p->next = pnew;
			p = pnew;
		}
		p->next = myHead.next;

		p = &myHead;
		while (p->next != p->next->next){
			for (unsigned int i = 0; i < m - 1; i++){
				p = p->next;
			}

			// delete
			p->next = p->next->next;
		}

		return p->val;
	}

	int LastRemaining_Solution1(unsigned int n, unsigned int m)
	{
		if (n <= 0 || m <= 0)
			return 0;

		list<unsigned int> mylist;
		for (unsigned int i = 0; i != n; i++){
			mylist.push_back(i);
		}

		list<unsigned int>::iterator iter = mylist.begin();
		for (unsigned int i = 0; i != m; ){
			if (mylist.size() == 1){
				break;
			}				

			if (iter == mylist.end()){
				iter = mylist.begin();
			}

			if (i == m - 1){
				list<unsigned int>::iterator next = ++iter;
				mylist.erase(--iter);
				iter = next;
				i = 0;
				continue;
			}
			else{
				iter++;
			}
			i++;
		}

		return mylist.front();
	}

	void test(){
		vector<unsigned int> n{ 4, 3, 6, 10, 11, 100, 1000,  0, INT_MAX };
		for (unsigned int i = 0; i != n.size(); i++){
			cout << LastRemaining_Solution(n[i], 3) << endl;
		}
	}
};


class Demo{
	static int count;
	static int sum;
public:
	Demo(){
		count++;
		sum += count;
	}

	int static getsum(){
		return sum;
	}

	static void reset(){
		count = 0;
		sum = 0;
	}
};

int Demo::count = 0;
int Demo::sum = 0;

class Solution47___ {
public:
	int Sum_Solution(int n) {
		Demo::reset();
		Demo* arr = new Demo[n];
		delete[] arr; 
		return Demo::getsum();
	}

	void test(){
		for (int i = 1; i != 10; i++){
			cout << Sum_Solution(i) << endl;
		}
	}
};

class Solution48__ {
public:
	int Add(int num1, int num2)
	{
		if (num1 < num2)
			return Add(num2, num1);

		if (num2 == 0)
			return num1;

		while (num2 != 0){
			int _and = num1 & num2;
			int _xor = num1 ^ num2;
			num2 = (_and << 1);
			num1 = _xor;
		}
		
		return num1;
	}

	void test(){
		for (int i = 0; i != 10; i++){
			cout << Add(i, 5) << endl;
		}
	}
};

class Solution49 {
public:
	int StrToInt(string str) {
		bool isStarted = false;
		int isPos = 1;
		int res = 0;
		for (int i = 0; i != str.size(); i++){
			if (str[i] == ' ' && !isStarted)
				continue;

			if (str[i] == '+' && !isStarted){
				isStarted = true;
				continue;
			}
			if (str[i] == '-' && !isStarted){
				isPos = -1;
				isStarted = true;
				continue;
			}

			if (str[i] >= '0' && str[i] <= '9'){
				res = 10 * res + str[i] - '0';
				continue;
			}

			return 0;
		}
		return isPos * res;
	}

	void test(){
		vector<string> vec{ "+323 ", " -23 123", " safsdf 32", "13 sfsd", "112e1wr", "", "32" };
		for (int i = 0; i != vec.size(); i++){
			cout << StrToInt(vec[i]) << endl;
		}
	}
};

class Solution50 {
public:
	// Parameters:
	//        numbers:     an array of integers
	//        length:      the length of array numbers
	//        duplication: (Output) the duplicated number in the array number
	// Return value:       true if the input is valid, and there are some duplications in the array number
	//                     otherwise false
	bool duplicate(int numbers[], int length, int* duplication) {
		unordered_set<int> myset;
		for (int i = 0; i != length; i++){
			if (myset.count(numbers[i]) > 0){
				*duplication = numbers[i];
				return true;
			}
			else{
				myset.insert(numbers[i]);
			}
		}
		return false;
	}

	void test(){
// 		int arr1[]{ 2, 3, 1, 0, 2, 5, 3 };
// 		int arr2[1];
// 		int arr3[]{1, 2};
// 		vector<int *> vec{ int[]{  }, int[]{}, int []{ 1, 2 } };
// 		for (int i = 0; i != vec.size(); i++){
// 			int num = 0;
// 			bool ret = duplicate(&vec[i][0], vec[i].size(), &num);
// 			cout << boolalpha << ret << "=====" << num << endl;
// 		}
	}
};

class Solution51 {
public:
	vector<int> multiply(const vector<int>& A) {
		vector<int> left(A.size() + 1, 1);
		vector<int> right(A.size() + 1, 1);

		for (int i = 0; i != A.size(); i++){
			left[i + 1] = left[i] * A[i];
		}
		for (int i = A.size() - 2; i >= 0; i--){
			right[i] = right[i + 1] * A[i + 1];
		}

		vector<int> res(A.size(), 0);
		for (int i = 0; i != A.size(); i++){
			res[i] = left[i] * right[i];
		}

		return res;
	}

	void test(){
		vector<vector<int>> vec{ { 1, 2, 3, 4 }, { 2, 3 }, {} };
		for (int i = 0; i != vec.size(); i++){
			auto res = multiply(vec[i]);
			for (auto item : res){
				cout << item << " ";
			}
			cout << endl;
		}
	}
};

class Solution52_____ {
public:
	bool match(char* str, char* pattern)
	{
		int len_s = strlen(str);
		int len_p = strlen(pattern);
		if (len_s < 0 || len_p < 0)
			return false;
		vector<vector<bool>> arr(len_s + 1, vector<bool>(len_p + 1, false));
		for (int i = 0; i != len_s + 1; i++){
			if (i == 0){
				arr[0][0] = true;
				for (int j = 1; j != len_p + 1; j++){
					if (pattern[j - 1] == '*')
						arr[0][j] = arr[0][j - 1] || (j >= 2 ? arr[0][j - 2] : false);
				}
				continue;
			}			
			for (int j = 0; j != len_p + 1; j++){
				if (j == 0){
					continue;
				}

				if (str[i - 1] == pattern[j - 1] || pattern[j - 1] == '.'){
					arr[i][j] = arr[i - 1][j - 1];
				}
				else if (pattern[j - 1] == '*'){
					if (j >= 2 && (pattern[j - 2] != str[i - 1] && pattern[j - 2] != '.')){
						arr[i][j] = arr[i][j - 1] || (j >= 2 ? arr[i][j - 2] : false);
					}
					else{
						arr[i][j] = arr[i - 1][j] ||
							arr[i][j - 1] || (j >= 2 ? arr[i][j - 2] : false);
					}
					
				}
				else
					arr[i][j] = false;
			}
		}

		return arr[len_s][len_p];
	}

	void test(){
		vector<string> str{ "aa", "aaa", "aaa", "abc", "aaa", "", "" };
		vector<string> pat{ ".*", "ab*a", "a.a", "adf", "ab*ac*a", ".*", "a" };
		for (int i = 0; i != str.size(); i++){
			cout << boolalpha << match(const_cast<char *>(str[i].c_str()), const_cast<char *>(pat[i].c_str())) << endl;
		}
	}
};

class Solution53_ {
public:
	bool isNumeric(char* string)
	{
		int len = strlen(string);
		if (len <= 0)
			return false;

		bool hasStarted = false;
		bool hasOper = false;
		bool hasNum = false;
		bool hasE = false;
		bool hasPoint = false;

		for (int i = 0; i < len; i++){
			if (!hasStarted && string[i] == ' '){
				continue;
			}

			if (string[i] == '+' || string[i] == '-'){
				if (hasOper || hasNum)
					return false;

				hasStarted = true;
				hasOper = true;
				continue;
			}

			if (string[i] == 'E' || string[i] == 'e'){
				if (!hasNum || hasE)
					return false;

				hasStarted = true;
				hasOper = false;
				hasNum = false;
				hasE = true;
				continue;
			}

			if (string[i] == '.'){
				if (hasPoint || hasE)
					return false;

				hasStarted = true;
				hasPoint = true;
				continue;
			}

			if (string[i] >= '0' && string[i] <= '9'){
				hasNum = true;
				hasStarted = true;
				continue;
			}

			return false;
		}

		return hasNum;
	}

	void test(){
		vector<string> arr{ "+100", "5e2", "-123", "3.1415", "-1E-16", "12e",
			"1a3.14", "1.2.2", "+-5", "12e+4.3" };
		for (int i = 0; i != arr.size(); i++){
			cout << boolalpha << isNumeric(const_cast<char*>(arr[i].c_str())) << endl;
		}
	}
};

class Solution54
{
public:
	//Insert one char from stringstream
	void Insert(char ch)
	{
		if (myset.count(ch) > 0)
			return;
		else{
			for (int i = 0; i != myvec.size(); i++){
				if (myvec[i] == ch){
					myset.insert(ch);
					myvec.erase(myvec.begin() + i);
					return;
				}
			}

			myvec.push_back(ch);
		}
	}
	//return the first appearence once char in current stringstream
	char FirstAppearingOnce()
	{
		if (myvec.size() == 0)
			return '#';
		else
			return myvec[0];
	}

private:
	unordered_set<int> myset;
	vector<char> myvec;

public:
	void test(){
		vector<string> arr{ "google", "hello", "as", "" };
		for (int i = 0; i != arr.size(); i++){
			myset.clear();
			myvec.clear();
			for (int j = 0; j != arr[i].size(); j++){
				Insert(arr[i][j]);
				cout << FirstAppearingOnce();
			}
			cout << endl;
		}
	}
};

class Solution55 {

	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};

public:
	ListNode* EntryNodeOfLoop(ListNode* pHead)
	{
		ListNode myHead(0);
		myHead.next = pHead;
		ListNode * p1 = &myHead;
		ListNode * p2 = &myHead;

		bool flag = false;
		while (p2 && p2->next){
			p2 = p2->next->next;
			p1 = p1->next;

			if (p1 == p2){
				flag = true;
				break;
			}
		}
		
		if (!flag)
			return nullptr;

		p2 = &myHead;
		while (p1 != p2){
			p1 = p1->next;
			p2 = p2->next;
		}

		return p1;
	}

	void display(ListNode * a){
		if (a)
			cout << a->val << endl;
	}

	void test(){
		ListNode a(0), b(1), c(2), d(3), e(5);
		a.next = &b, b.next = &c, c.next = &d, d.next = &b;
		e.next = &e;
		vector<ListNode *> arr{ &a, &e, nullptr };
		for (int i = 0; i != arr.size(); i++){
			auto res = EntryNodeOfLoop(arr[i]);
			display(res);
		}
	}
};

class Solution56___ {
	struct ListNode {
		int val;
		struct ListNode *next;
		ListNode(int x) :
			val(x), next(NULL) {
		}
	};	

public:
	ListNode* deleteDuplication(ListNode* pHead)
	{
		if (pHead == nullptr)
			return nullptr;

		ListNode myHead(0);
		ListNode * pre = &myHead;
		myHead.next = pHead;
		ListNode * cur = pre->next;

		while (cur){
			if (cur->next && cur->next->val != cur->val){
				pre = pre->next;
				cur = cur->next;
			}
			else if (cur->next == nullptr){
				return myHead.next;
			}
			else{
				int val = cur->val;
				bool flag = false;
				while (cur->next){
					if (cur->next->val != val){
						pre->next = cur->next;
						cur = cur->next;
						flag = true;
						break;
					}
					else{
						cur = cur->next;
					}
				}
				if (!flag && cur->next == nullptr){
					pre->next = nullptr;
					cur = cur->next;
				}
			}
		}
		return myHead.next;
	}

	void display(ListNode * res){
		while (res){
			cout << res->val << " ";
			res = res->next;
		}
		cout << endl;
	}

	void test(){
		ListNode a(1), b(2), c(3), d(3), e(4), f(4), g(5), h(6), i(6);
		a.next = &b, b.next = &c, c.next = &d, d.next = &e, e.next = &f, f.next = &g;
		h.next = &i;

		vector<ListNode *> arr{ &a, &h, nullptr };
		for (int i = 0; i != arr.size(); i++){
			auto res = deleteDuplication(arr[i]);
			display(res);
		}
	}
};

class Solution57 {

	struct TreeLinkNode {
		int val;
		struct TreeLinkNode *left;
		struct TreeLinkNode *right;
		struct TreeLinkNode *next;
		TreeLinkNode(int x) :val(x), left(NULL), right(NULL), next(NULL) {

		}
	};

public:
	TreeLinkNode* GetNext(TreeLinkNode* pNode)
	{
		if (pNode == nullptr)
			return nullptr;

		if (pNode->right){
			TreeLinkNode * root = pNode->right;
			while (root->left){
				root = root->left;
			}
			return root;
		}
		else{
			while (pNode->next && pNode->next->right == pNode){
				pNode = pNode->next;
			}
			return pNode->next;
		}
	}

	void test(){

	}
};

class Solution58_ {

	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};
	
public:
	bool isSymmetrical(TreeNode* pRoot)
	{
		if (pRoot == nullptr)
			return false;

		return _isSymmetrical(pRoot->left, pRoot->right);
	}

	bool _isSymmetrical(TreeNode * p1, TreeNode * p2){
		if (p1 == nullptr && p2 == nullptr)
			return true;

		if (p1 == nullptr && p2 ||
			p2 == nullptr && p1 || p1->val != p2->val)
			return false;

		return _isSymmetrical(p1->left, p2->right)
			&& _isSymmetrical(p1->right, p2->left);
	}

	void test(){
		TreeNode a(1), b(1), c(2), d(2), e(3), f(3), g(9);
		e.left = &a, e.right = &b, a.left = &c, b.right = &d;
		f.left = &g;
		vector<TreeNode *> vec{ &e, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			cout << isSymmetrical(vec[i]) << endl;
		}

	}

};

class Solution59__ {

	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	vector<vector<int> > Print(TreeNode* pRoot) {
		vector<vector<int> > vec;
		if (pRoot == nullptr)
			return vec;

		queue<TreeNode *> myqueue;
		myqueue.push(pRoot);
		int level_id = 0;
		while (!myqueue.empty()){
			int len = myqueue.size();
			vector<int> level;
			for (int i = 0; i != len; i++){
				TreeNode * front = myqueue.front();
				level.push_back(front->val);
				myqueue.pop();
				if (front->left)
					myqueue.push(front->left);
				if (front->right)
					myqueue.push(front->right);
			}	

			if (level_id % 2){
				vec.emplace_back(vector<int>(level.crbegin(), level.crend()));
			}
			else{
				vec.push_back(level);
			}
			level_id++;
		}
		return vec;
	}

	void display(vector<vector<int> > & a){
		for (auto item : a){
			for (auto subitem : item){
				cout << subitem << " ";
			}
			cout << endl;
		}
	}

	void test(){
		TreeNode a(1), b(4), c(2), d(8), e(3), f(3), g(9);
		e.left = &a, e.right = &b, a.left = &c, b.right = &d;
		f.left = &g;
		vector<TreeNode *> vec{ &e, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = Print(vec[i]);
			display(res);
		}
	}

};

class Solution60 {
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	vector<vector<int> > Print(TreeNode* pRoot) {
		vector<vector<int> > vec;
		if (pRoot == nullptr)
			return vec;

		queue<TreeNode *> myqueue;
		myqueue.push(pRoot);
		while (!myqueue.empty()){
			int len = myqueue.size();
			vector<int> level;
			for (int i = 0; i != len; i++){
				TreeNode * front = myqueue.front();
				level.push_back(front->val);
				myqueue.pop();
				if (front->left)
					myqueue.push(front->left);
				if (front->right)
					myqueue.push(front->right);
			}

			vec.push_back(level);
		}
		return vec;
	}

	void display(vector<vector<int> > & a){
		for (auto item : a){
			for (auto subitem : item){
				cout << subitem << " ";
			}
			cout << endl;
		}
	}

	void test(){
		TreeNode a(1), b(4), c(2), d(8), e(3), f(3), g(9);
		e.left = &a, e.right = &b, a.left = &c, b.right = &d;
		f.left = &g;
		vector<TreeNode *> vec{ &e, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = Print(vec[i]);
			display(res);
		}
	}

};

class Solution61__ {

	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	char* Serialize(TreeNode *root) {
		if (root == nullptr)
			return nullptr;

		string res;
		preorder(root, res);
		int len = res.size();
		char * buf = new char[len + 1];
		memset(buf, 0, len + 1);
		strcpy(buf, res.c_str());
		return buf;
	}

	void preorder(TreeNode * root, string & str){
		if (root == nullptr){
			str += "!#";
			return;
		}			

		str += to_string(root->val) + '#';
		preorder(root->left, str);
		preorder(root->right, str);
	}

	TreeNode* Deserialize(char *str) {
		if (str == nullptr)
			return nullptr;

		TreeNode * root = nullptr;
		string mystr(str);
		preorder_b(root, mystr);
		return root;
	}

	void preorder_b(TreeNode * &root, string & str){
		int pos = str.find('#');
		string sub = str.substr(0, pos);
		str = str.substr(pos + 1);
		if (sub == "!"){
			return;
		}

		root = new TreeNode(stoi(sub));
		preorder_b(root->left, str);
		preorder_b(root->right, str);
	}

// 	void display(TreeNode * a){
// 		for (auto item : a){
// 			for (auto subitem : item){
// 				cout << subitem << " ";
// 			}
// 			cout << endl;
// 		}
// 	}

	void test(){
		TreeNode a(1), b(4), c(2), d(8), e(3), f(3), g(9);
		e.left = &a, e.right = &b, a.left = &c, b.right = &d;
		f.left = &g;
		vector<TreeNode *> vec{ &e, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			char * str = Serialize(vec[i]);
			cout << str << endl;
			TreeNode * root = Deserialize(str);
			char * newstr = Serialize(root);
			cout << newstr << endl;
		}
	}
};

class Solution62 {

	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};

public:
	TreeNode* KthNode(TreeNode* pRoot, unsigned int k)
	{
		if (pRoot == nullptr)
			return nullptr;

		int left = getTreeNodeCount(pRoot->left);
		if (left + 1 == k)
			return pRoot;
		else if (left >= k){
			return KthNode(pRoot->left, k); 
		}
		else{
			return KthNode(pRoot->right, k - left - 1);
		}
	}

	int getTreeNodeCount(TreeNode * root){
		if (root == nullptr)
			return 0;

		return getTreeNodeCount(root->left) +
			getTreeNodeCount(root->right) + 1;
	}


	void test(){
		TreeNode a(1), b(4), c(2), d(8), e(3), f(3), g(9);
		e.left = &a, e.right = &b, a.left = &c, b.right = &d;
		f.left = &g;
		vector<TreeNode *> vec{ &e, &f, nullptr };
		for (int i = 0; i != vec.size(); i++){
			auto res = KthNode(vec[i], 3);
			if (res)
				cout << res->val << endl;
		}
	}
};

class Solution63 {
public:
	void Insert(int num)
	{
		for (int i = 0; i != myvec.size(); i++){
			if (myvec[i] >= num){
				myvec.insert(myvec.begin() + i, num);
				return;
			}
		}
		myvec.push_back(num);
	}

	double GetMedian()
	{
		int len = myvec.size();
		if (len % 2){
			return myvec[len / 2];
		}
		else{
			return (myvec[len / 2 - 1] + myvec[len / 2]) / 2.0;
		}
	}

private:
	vector<int> myvec;

public:
	void test(){
		vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		for (int i = 0; i != vec.size(); i++){
			Insert(vec[i]);
			cout << GetMedian() << endl;
		}
	}

};

class Solution64_ {
public:
	vector<int> maxInWindows(const vector<int>& num, unsigned int size)
	{
		vector<int> res;
		if (num.size() < size || size == 0)
			return res;

		int start = 0, stop = size;
		while (stop <= num.size()){
			int cur = *max_element(num.begin() + start, num.begin() + stop);
			start++;
			stop++;
			res.push_back(cur);
		}
		return res;
	}


	void test(){
		vector<vector<int>> num{ { 2, 3, 4, 2, 6, 2, 5, 1 }, {} };
		for (int i = 0; i != num.size(); i++){
			auto res = maxInWindows(num[i], 3);
			for (auto item : res){
				cout << item << " ";
			}
			cout << endl;
		}
	}
};

class Solution65 {
public:
	bool hasPath(char* matrix, int rows, int cols, char* str)
	{
		bool ret = false;
		if (matrix == nullptr)
			return false;

		for (int i = 0; i != rows * cols; i++){
			ret = search(matrix, rows, cols, str, i / cols, i - i / cols * cols);
			if (ret)
				return true;
		}

		return false;
	}

	bool search(char* matrix, int rows, int cols, char* str, int i, int j){
		if (*str == '\0')
			return true;

		if (i < 0 || i >= rows || j < 0 || j >= cols)
			return false;

		if (matrix[i * cols + j] != *str)
			return false;

		char tmp = *str;
		matrix[i * cols + j] = ' ';

		bool ret = search(matrix, rows, cols, str + 1, i + 1, j)
			|| search(matrix, rows, cols, str + 1, i - 1, j)
			|| search(matrix, rows, cols, str + 1, i, j - 1)
			|| search(matrix, rows, cols, str + 1, i, j + 1);
		matrix[i * cols + j] = tmp;
		return ret;
	}

	void test(){
		//char matrix[]{'a', 'b', 'c', 'e', 's', 'f', 'c', 's', 'a', 'd', 'e', 'e'};
		char matrix[]{"ABCEHJIGSFCSLOPQADEEMNOEADIDEJFMVCEIFGGS"};
		vector<char *> vec{ "SLHECCEIDEJFGGFIE", "bcced", "abcb", "" };
		for (int i = 0; i != vec.size(); i++){
			cout << hasPath(matrix, 5, 8, vec[i]) << endl;
		}
	}


};

class Solution66 {
public:
	int movingCount(int threshold, int rows, int cols)
	{
		vector<vector<int>> mymap(rows, vector<int>(cols, false));
		movingCount(threshold, rows, cols, mymap, 0, 0);
		int sum = 0;
		for (auto item : mymap){
			for (auto subitem : item){
				sum += subitem;
			}
		}
		return sum;
	}

	int getSum(int i){
		int count = 0;
		while (i){
			count += i % 10;
			i = i / 10;
		}
		return count;
	}

	void movingCount(int threshold, int rows, int cols, vector<vector<int>> & mymap, int i, int j){
		if (i >= rows || j >= cols || j < 0 || i < 0)
			return;

		if (mymap[i][j])
			return;

		int sum = getSum(i) + getSum(j);
		if (sum > threshold)
			return;

		mymap[i][j] = true;
		movingCount(threshold, rows, cols, mymap, i + 1, j);
		movingCount(threshold, rows, cols, mymap, i - 1, j);
		movingCount(threshold, rows, cols, mymap, i, j + 1);
		movingCount(threshold, rows, cols, mymap, i, j - 1);
	}

	void test(){
		cout << movingCount(2, 1, 1) << endl;
	}
};


int main(){
	Solution s;	
	s.test();

	system("pause");
	return 0;
}