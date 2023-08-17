
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
			_root = _Copy(t._root);//ǰ��ݹ鿽��
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
			//������Ҫ�ҵ�Ҫɾ���Ľ�㣬���������Ҫ��¼���ڵ�
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
				else//�ҵ�Ҫɾ���Ľ����--cur����Ҫɾ���Ľ��
				{
					//1.������Ϊ��,������Ϊ�½�㣬��Ҫ�й¸����ڵ�
					//�����������ɾ�����Ϊ���ڵ�ʱ

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
					//2.������Ϊ�գ�������Ϊ�½�㣬��Ҫ�й¸����ڵ�
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
					//3.�������������ڣ��ұ�ķ
					else
					{
						//��ķ����ǰҪɾ���������ҽ��
						Node* leftMax = cur->left;
						Node* parent = cur;
						//���ﲻ�ܸ�nullptr
						while (leftMax->right)
						{
							parent = leftMax;
							leftMax = leftMax->right;
						}
						//�ҵ���ķ�󽻻�
						std::swap(cur->_key, leftMax->_key);

						//ת��Ϊ��������--��Ϊ���ҽ����ҽ��϶�Ϊ��
						//����ڵ���ǹ½�㣬��Ҫ�й�
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
		bool insertR(const K& key)//ÿ�εݹ鶼��ҪҪ�ı�root��״̬�����Ա���Ҫ��root����������ʹ��һ���Ӻ���
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
		Node* _FindR(Node* root, const K& key)//key���޷����޸ĵģ�value�ǿ��Ա��޸ĵģ�����Ҫ��Node* ���޸�value
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
		Node* _Copy(Node* root)//ǰ��ݹ鿽��
		{
			if (root == nullptr)
				return nullptr;
			//��  ������  ������
			Node* newnode = new Node(root->_key);

			newnode->left = _Copy(root->left);//�ݹ鿽��
			newnode->right = _Copy(root->right);
			//�ݹ�ʱ������������㣬����ʱ����������

			return newnode;
		}
		void Destroy(Node* root)
		{
			//�����ߺ������
			if (root == nullptr)
				return;

			Destroy(root->left);

			Destroy(root->right);
			delete root;
			root = nullptr;
		}
		bool _eraseR(Node*& root, const K& key)
		{

			//���Ȼ���Ҫ���ҵ�Ҫɾ���Ľ��
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
			else//�ҵ���
			{
				Node* del = root;
				//���õ�������root���Ǹ��ڵ����������������������ã�������Ҫ�Ҹ��ڵ���
				//1.������Ϊ��
				if (root->left == nullptr)
				{
					root = root->right;
				}
				//2.������Ϊ��
				else if (root->right == nullptr)
				{
					root = root->left;
				}
				//3.������������Ϊ��
				else
				{

					//�����ұ�ķ
					Node* leftMax = root->left;
					while (leftMax->right)
					{
						leftMax = leftMax->right;
					}
					std::swap(del->_key, leftMax->_key);

					//ת��Ϊ������
					//�������Ͳ���һ���������ˣ��ṹ���ƻ��ˣ���������û�У�������ʱҪɾ���Ľ��ֻ��һ��������û�н��
					return _eraseR(root->left, key);

				}
				delete del;
				return true;
			}

		}
		bool _insertR(Node*& root, const K& key)
		{
			//root�Ǹ��ڵ������������������ı���

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
			_root = _Copy(t._root);//ǰ��ݹ鿽��
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

		
		bool insertR(const K& key,const V& value)//ÿ�εݹ鶼��ҪҪ�ı�root��״̬�����Ա���Ҫ��root����������ʹ��һ���Ӻ���
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
		Node* _FindR(Node* root, const K& key)//key���޷����޸ĵģ�value�ǿ��Ա��޸ĵģ�����Ҫ��Node* ���޸�value
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
		Node* _Copy(Node* root)//ǰ��ݹ鿽��
		{
			if (root == nullptr)
				return nullptr;
			//��  ������  ������
			Node* newnode = new Node(root->_key,root->_value,root->_value);

			newnode->left = _Copy(root->left);//�ݹ鿽��
			newnode->right = _Copy(root->right);
			//�ݹ�ʱ������������㣬����ʱ����������

			return newnode;
		}
		void Destroy(Node* root)
		{
			//�����ߺ������
			if (root == nullptr)
				return;

			Destroy(root->left);

			Destroy(root->right);
			delete root;
			root = nullptr;
		}
		bool _eraseR(Node*& root, const K& key)
		{

			//���Ȼ���Ҫ���ҵ�Ҫɾ���Ľ��
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
			else//�ҵ���
			{
				Node* del = root;
				//���õ�������root���Ǹ��ڵ����������������������ã�������Ҫ�Ҹ��ڵ���
				//1.������Ϊ��
				if (root->left == nullptr)
				{
					root = root->right;
				}
				//2.������Ϊ��
				else if (root->right == nullptr)
				{
					root = root->left;
				}
				//3.������������Ϊ��
				else
				{

					//�����ұ�ķ
					Node* leftMax = root->left;
					while (leftMax->right)
					{
						leftMax = leftMax->right;
					}
					std::swap(del->_key, leftMax->_key);

					//ת��Ϊ������
					//�������Ͳ���һ���������ˣ��ṹ���ƻ��ˣ���������û�У�������ʱҪɾ���Ľ��ֻ��һ��������û�н��
					return _eraseR(root->left, key);

				}
				delete del;
				return true;
			}

		}
		bool _insertR(Node*& root, const K& key,const V& value)
		{
			//root�Ǹ��ڵ������������������ı���

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
	string a[] = { "����","�㽶","������","����","����","����","ƻ��","�㽶","������" };
	 
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
	key_value::BSTree<string, string> dic;//���ֵ�  key_valueģ��
	dic.insertR("insert", "����");
	dic.insertR("delete", "ɾ��");
	dic.insertR("love", "ϲ��");
	dic.insertR("print", "��ӡ");
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
			cout << "������" << endl;
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