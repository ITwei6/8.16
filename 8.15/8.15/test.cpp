
#include <iostream>

using namespace std;

namespace key
{
	template <class K>
	struct BSTreeNode
	{
		BSTreeNode<K>* left;
		BSTreeNode<K>* right;
		K _key;

		BSTreeNode(const K& key)
			:_key(key)
			, left(nullptr)
			, right(nullptr)
		{}
	};

	template <class K>
	struct BSTree
	{
		typedef BSTreeNode<K> Node;

		BSTree()
			:_root(nullptr)
		{}

		BSTree(BSTree<K>& t)
			:_root(nullptr)
		{
			_root = _Copy(t._root);//前序递归拷贝
		}
		BSTree<K>* operator=(BSTree<K> t)
		{
			swap(_root, t._root);
			return this;
		}
		void Inoder()
		{
			_Inoder(_root);
		}
		Node* FindR(const K& key)
		{
			return _FindR(_root, key);
		}
		bool insert(const K& key)
		{
			if (_root == nullptr)
			{
				_root = new Node(key);
			}
			else
			{
				Node* cur = _root;
				Node* parent = nullptr;
				while (cur)
				{
					if (cur->_key < key)
					{
						parent = cur;
						cur = cur->right;
					}
					else if (cur->_key > key)
					{
						parent = cur;
						cur = cur->left;
					}
					else
					{
						return false;
					}
				}

				if (parent->_key < key)
				{
					parent->right = new Node(key);
				}
				if (parent->_key > key)
				{
					parent->left = new Node(key);
				}


			}
			return true;
		}
		bool erase(const K& key)
		{
			//首先需要找到要删除的结点，这个过程需要记录父节点
			Node* cur = _root;
			Node* parent = nullptr;
			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->right;
				}
				else if (cur->_key > key)
				{
					parent = cur;
					cur = cur->left;
				}
				else//找到要删除的结点了--cur就是要删除的结点
				{
					//1.右子树为空,左子树为孤结点，需要托孤给父节点
					//特殊情况：当删除结点为根节点时

					if (cur->right == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->left;
						}
						else
						{
							if (parent->left == cur)
							{
								parent->left = cur->left;
							}
							else
							{
								parent->left = cur->left;
							}
						}

					}
					//2.左子树为空，右子树为孤结点，需要托孤给父节点
					else if (cur->left == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->right;
						}
						else
						{
							if (parent->left == cur)
							{
								parent->left = cur->right;
							}
							else
							{
								parent->right = cur->right;
							}
						}
					}
					//3.左右子树都存在，找保姆
					else
					{
						//保姆：当前要删除结点的最右结点
						Node* leftMax = cur->left;
						Node* parent = cur;
						//这里不能给nullptr
						while (leftMax->right)
						{
							parent = leftMax;
							leftMax = leftMax->right;
						}
						//找到保姆后交换
						std::swap(cur->_key, leftMax->_key);

						//转化为上面问题--因为最右结点的右结点肯定为空
						//那左节点就是孤结点，需要托孤
						if (parent->right == leftMax)
						{
							parent->right = leftMax->left;
						}
						if (parent->left == leftMax)
						{
							parent->left = leftMax->left;
						}

						cur = leftMax;
					}

					delete cur;
					return true;
				}
			}
			return false;
		}
		bool insertR(const K& key)//每次递归都需要要改变root的状态，所以必须要传root过来，这里使用一个子函数
		{
			return _insertR(_root, key);
		}

		bool eraseR(const K& key)
		{
			return _eraseR(_root, key);
		}
		~BSTree()
		{
			Destroy(_root);
		}
	private:
		Node* _FindR(Node* root, const K& key)//key是无法被修改的，value是可以被修改的，所以要传Node* 来修该value
		{
			if (root == nullptr)
			{
				return nullptr;
			}
			if (root->_key < key)
			{
				FindR(root->right, key);
			}
			else if(root->_key>key)
			{
				FindR(root->left, key);
			}
			else
			{
				return root;
			}
		}
		Node* _Copy(Node* root)//前序递归拷贝
		{
			if (root == nullptr)
				return nullptr;
			//根  左子树  右子树
			Node* newnode = new Node(root->_key);

			newnode->left = _Copy(root->left);//递归拷贝
			newnode->right = _Copy(root->right);
			//递归时，拷贝创建结点，返回时，链接起来

			return newnode;
		}
		void Destroy(Node* root)
		{
			//析构走后序遍历
			if (root == nullptr)
				return;

			Destroy(root->left);

			Destroy(root->right);
			delete root;
			root = nullptr;
		}
		bool _eraseR(Node*& root, const K& key)
		{

			//首先还是要先找到要删除的结点
			if (root == nullptr)
				return false;

			if (root->_key < key)
			{
				return _eraseR(root->right, key);
			}
			else if (root->_key > key)
			{
				return _eraseR(root->left,key);
			}
			else//找到了
			{
				Node* del = root;
				//引用的魅力：root就是父节点左子树或者右子树的引用！，不需要找父节点了
				//1.左子树为空
				if (root->left == nullptr)
				{
					root = root->right;
				}
				//2.右子树为空
				else if (root->right == nullptr)
				{
					root = root->left;
				}
				//3.左右子树都不为空
				else
				{

					//首先找保姆
					Node* leftMax = root->left;
					while (leftMax->right)
					{
						leftMax = leftMax->right;
					}
					std::swap(del->_key, leftMax->_key);

					//转化为子问题
					//交换完后就不是一个搜索树了，结构被破坏了，但左子树没有，并且这时要删除的结点只有一个结点或者没有结点
					return _eraseR(root->left, key);

				}
				delete del;
				return true;
			}

		}
		bool _insertR(Node*& root, const K& key)
		{
			//root是父节点左子树或者右子树的别名

			if (root == nullptr)
			{
				root = new Node(key);
			}

			if (root->_key < key)
			{
				return _insertR(root->right, key);
			}
			else if (root->_key > key)
			{
				return _insertR(root->left, key);
			}
			else
			{
				return false;
			}
			return true;

		}
		void _Inoder(Node* _root)
		{
			if (_root == nullptr)
				return;

			_Inoder(_root->left);
			cout << _root->_key << " ";
			_Inoder(_root->right);

		}
		Node* _root;
	};

}

namespace key_value
{
	template <class K,class V>
	struct BSTreeNode
	{
		BSTreeNode<K,V>* left;
		BSTreeNode<K,V>* right;
		K _key;
		V _value;
		BSTreeNode(const K& key,const V& value)
			:_key(key)
			,_value(value)
			, left(nullptr)
			, right(nullptr)
		{}
	};

	template <class K,class V>
	struct BSTree
	{
		typedef BSTreeNode<K,V> Node;

		BSTree()
			:_root(nullptr)
		{}

		BSTree(BSTree<K,V>& t)
			:_root(nullptr)
		{
			_root = _Copy(t._root);//前序递归拷贝
		}
		BSTree<K,V>* operator=(BSTree<K,V> t)
		{
			swap(_root, t._root);
			return this;
		}
		void Inoder()
		{
			_Inoder(_root);
		}

		
		bool insertR(const K& key,const V& value)//每次递归都需要要改变root的状态，所以必须要传root过来，这里使用一个子函数
		{
			return _insertR(_root, key,value);
		}

		bool eraseR(const K& key)
		{
			return _eraseR(_root, key);
		}
		~BSTree()
		{
			Destroy(_root);
		}
		Node* FindR(const K& key)
		{
			return _FindR(_root, key);
		}
	private:
		Node* _FindR(Node* root, const K& key)//key是无法被修改的，value是可以被修改的，所以要传Node* 来修该value
		{
			if (root == nullptr)
			{
				return nullptr;
			}
			if (root->_key < key)
			{
				_FindR(root->right, key);
			}
			else if (root->_key > key)
			{
				_FindR(root->left, key);
			}
			else
			{
				return root;
			}
		}
		Node* _Copy(Node* root)//前序递归拷贝
		{
			if (root == nullptr)
				return nullptr;
			//根  左子树  右子树
			Node* newnode = new Node(root->_key,root->_value,root->_value);

			newnode->left = _Copy(root->left);//递归拷贝
			newnode->right = _Copy(root->right);
			//递归时，拷贝创建结点，返回时，链接起来

			return newnode;
		}
		void Destroy(Node* root)
		{
			//析构走后序遍历
			if (root == nullptr)
				return;

			Destroy(root->left);

			Destroy(root->right);
			delete root;
			root = nullptr;
		}
		bool _eraseR(Node*& root, const K& key)
		{

			//首先还是要先找到要删除的结点
			if (root == nullptr)
				return false;

			if (root->_key < key)
			{
				return _eraseR(root->right, key);
			}
			else if (root->_key > key)
			{
				return _eraseR(root->left, key);
			}
			else//找到了
			{
				Node* del = root;
				//引用的魅力：root就是父节点左子树或者右子树的引用！，不需要找父节点了
				//1.左子树为空
				if (root->left == nullptr)
				{
					root = root->right;
				}
				//2.右子树为空
				else if (root->right == nullptr)
				{
					root = root->left;
				}
				//3.左右子树都不为空
				else
				{

					//首先找保姆
					Node* leftMax = root->left;
					while (leftMax->right)
					{
						leftMax = leftMax->right;
					}
					std::swap(del->_key, leftMax->_key);

					//转化为子问题
					//交换完后就不是一个搜索树了，结构被破坏了，但左子树没有，并且这时要删除的结点只有一个结点或者没有结点
					return _eraseR(root->left, key);

				}
				delete del;
				return true;
			}

		}
		bool _insertR(Node*& root, const K& key,const V& value)
		{
			//root是父节点左子树或者右子树的别名

			if (root == nullptr)
			{
				root = new Node(key,value);
			}

			if (root->_key < key)
			{
				return _insertR(root->right,key,value);
			}
			else if (root->_key > key)
			{
				return _insertR(root->left, key,value);
			}
			else
			{
				return false;
			}
			return true;

		}
		void _Inoder(Node* _root)
		{
			if (_root == nullptr)
				return;

			_Inoder(_root->left);
			cout << _root->_key << ":"<<_root->_value<<endl;
			_Inoder(_root->right);

		}
		Node* _root;
	};

}

void test2()
{
	key_value::BSTree<string, int> couttree;
	string a[] = { "西瓜","香蕉","火龙果","橘子","梨子","西瓜","苹果","香蕉","火龙果" };
	 
	for (auto& e : a)
	{
		auto ret = couttree.FindR(e);
		if (ret == nullptr)
		{
			couttree.insertR(e, 1);
		}
		else
		{
			ret->_value++;
		}

	}
	couttree.Inoder();
}
void test1()
{
	key_value::BSTree<string, string> dic;//查字典  key_value模型
	dic.insertR("insert", "插入");
	dic.insertR("delete", "删除");
	dic.insertR("love", "喜欢");
	dic.insertR("print", "打印");
	dic.Inoder();
	string name;
	while (cin >> name)
	{
		auto ret = dic.FindR(name);
		if (ret != nullptr)
		{
			cout << ret->_value << endl;
		}
		else
		{
			cout << "不存在" << endl;
		}
	}
	
}
int main()
{
	test2();
	
}
//int main()
//{
//	int a[] = { 8, 3, 1, 10, 6, 4, 7, 14, 13 };
//	int sz = sizeof(a) / sizeof(a[0]);
//	BSTree<int> t;
//	for (int i = 0; i < sz; i++)
//	{
//		t.insertR(a[i]);
//	}
//	t.Inoder();
//	cout << endl;
//	int b[] = { 1,2,34,5,6,7,8,9 };
//	int sz1 = sizeof(b) / sizeof(b[0]);
//	BSTree<int> t1;
//	for (int i = 0; i < sz1; i++)
//	{
//		t1.insertR(b[i]);
//	}
//	t1.Inoder();
//	cout << endl;
//	t = t1;
//	t.Inoder();
//	cout << endl;
//	/*for (auto& e : a)
//	{
//		t.eraseR(e);
//	}
//
//	cout << endl;*/
//
//
//}