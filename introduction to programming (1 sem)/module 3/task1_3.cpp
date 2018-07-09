/*

Дано число N ≤ 10^4 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке post-order (снизу вверх).

*/




#include <assert.h>
#include <iostream>
#include <stack>




template<class TKey, class TValue>
class Node
{
private:
	Node<TKey, TValue>* right;
	Node<TKey, TValue>* left;
	TKey key;
	TValue value;
public:
	Node(const TKey& key, const TValue& value);
	void GetInfo() const;
	Node<TKey, TValue>*& GetRight();
	Node<TKey, TValue>*& GetLeft();
	TKey GetKey();
	TValue GetValue();
};




template<class TKey, class TValue>
class BSTree
{
private:
	Node<TKey, TValue> *root;
public:
	BSTree();
	~BSTree();
	void Insert(const TKey& key);
	void Insert(const TKey& key, const TValue& value);
	void PostOrderTraversal();
private:
	void insert_(const TKey& key, const TValue& value);
};




int main()
{
	int tmpvalue = 0, n = 0;
	BSTree<int, int>* bst = new BSTree<int, int>();
	std::cin >> n;
	for (int i = 0; i < n; i++)
	{
		std::cin >> tmpvalue;
		bst->Insert(tmpvalue);
	}
	bst->PostOrderTraversal();
	delete bst;
	return 0;
}




template<class TKey, class TValue>
Node<TKey, TValue>::Node(const TKey& key, const TValue& value)
{
	this->key = key;
	this->value = value;
	this->left = nullptr;
	this->right = nullptr;
}

template<class TKey, class TValue>
void Node<TKey, TValue>::GetInfo() const
{
	std::cout << value << " ";
}

template<class TKey, class TValue>
Node<TKey, TValue>*& Node<TKey, TValue>::GetRight()
{
	return right;
}

template<class TKey, class TValue>
Node<TKey, TValue>*& Node<TKey, TValue>::GetLeft()
{
	return left;
}

template<class TKey, class TValue>
TKey Node<TKey, TValue>::GetKey()
{
	return key;
}

template<class TKey, class TValue>
TValue Node<TKey, TValue>::GetValue()
{
	return value;
}




template<class TKey, class TValue>
BSTree<TKey, TValue>::BSTree()
{
	root = nullptr;
}

template<class TKey, class TValue>
BSTree<TKey, TValue>::~BSTree()
{
	if (root == nullptr)
		return;
	std::stack<Node<TKey, TValue>*>* extendedrecursion = \
		new std::stack<Node<TKey, TValue>*>();
	std::stack<Node<TKey, TValue>*>* nodesforremoval = \
		new std::stack<Node<TKey, TValue>*>();
	extendedrecursion->push(root);
	Node<TKey, TValue>* tmpnode = nullptr;
	while (!extendedrecursion->empty())
	{
		tmpnode = extendedrecursion->top();
		extendedrecursion->pop();
		nodesforremoval->push(tmpnode);
		if (tmpnode->GetLeft() != nullptr)
			extendedrecursion->push(tmpnode->GetLeft());
		if (tmpnode->GetRight() != nullptr)
			extendedrecursion->push(tmpnode->GetRight());
	}
	while (!nodesforremoval->empty())
	{
		tmpnode = nodesforremoval->top();
		nodesforremoval->pop();
		delete tmpnode;
	}
	delete extendedrecursion;
	delete nodesforremoval;
}

template<class TKey, class TValue>
void BSTree<TKey, TValue>::Insert(const TKey& key)
{
	insert_(key, key);
}

template<class TKey, class TValue>
void BSTree<TKey, TValue>::Insert(const TKey& key, const TValue& value)
{
	insert_(key, value);
}

template<class TKey, class TValue>
void BSTree<TKey, TValue>::insert_(const TKey& key, const TValue& value)
{
	if (root == NULL)
	{
		root = new Node<TKey, TValue>(key, value);
		return;
	}
	Node<TKey, TValue>* tmpnode = root;
	while (true)
	{
		if (key < tmpnode->GetKey())
		{
			if (tmpnode->GetLeft() == NULL)
			{
				tmpnode->GetLeft() = new Node<TKey, TValue>(key, value);
				break;
			}
			else
				tmpnode = tmpnode->GetLeft();
		}
		else
		{
			if (tmpnode->GetRight() == NULL)
			{
				tmpnode->GetRight() = new Node<TKey, TValue>(key, value);
				break;
			}
			else
				tmpnode = tmpnode->GetRight();
		}
	}
}

template<class TKey, class TValue>
void BSTree<TKey, TValue>::PostOrderTraversal()
{
	if (root == nullptr)
		return;
	std::stack<Node<TKey, TValue>*>* extendedrecursion = \
		new std::stack<Node<TKey, TValue>*>();
	std::stack<Node<TKey, TValue>*>* treeinfo = \
		new std::stack<Node<TKey, TValue>*>();
	extendedrecursion->push(root);
	Node<TKey, TValue>* tmpnode = nullptr;
	while (!extendedrecursion->empty())
	{
		tmpnode = extendedrecursion->top();
		extendedrecursion->pop();
		if (tmpnode->GetLeft() != nullptr)
			extendedrecursion->push(tmpnode->GetLeft());
		if (tmpnode->GetRight() != nullptr)
			extendedrecursion->push(tmpnode->GetRight());
		treeinfo->push(tmpnode);
	}
	while (!treeinfo->empty())
	{
		treeinfo->top()->GetInfo();
		treeinfo->pop();
	}
	delete extendedrecursion;
	delete treeinfo;
}