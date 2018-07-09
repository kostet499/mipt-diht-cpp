/*

Дано число N < 10^6 и последовательность пар целых чисел из [-2^31, 2^31] длиной N.
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного
на лекции:
При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
Новый узел вставьте на место узла P. Построить также наивное дерево поиска по ключам Xi.
Равные ключи добавляйте в правое поддерево. Вычислить количество узлов в самом широком слое
декартового дерева и количество узлов в самом широком слое наивного дерева поиска. Вывести их
разницу. Разница может быть отрицательна.

*/




#include <algorithm>
#include <assert.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <queue>




template<class TKey, class TValue>
class Node
{
private:
	Node<TKey, TValue>* right;
	Node<TKey, TValue>* left;
	TKey key;
	TValue value;
public:
	Node(TKey key, TValue value);
	Node(TKey key, TValue value, Node<TKey, TValue>*& leftCNode, \
		Node<TKey, TValue>*& rightCNode);
	Node(const Node<TKey, TValue>*& cnode);
	Node<TKey, TValue>& operator=(const Node<TKey, TValue>*& cnode);
	inline Node<TKey, TValue>*& GetRight();
	inline Node<TKey, TValue>*& GetLeft();
	inline TKey GetKey();
	inline TValue GetValue();
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
	inline int GetWidth();
private:
	void insert_(const TKey& key, const TValue& value);
};




template<class TKey, class TValue>
class Treap
{
private:
	Node<TKey, TValue>* root;
public:
	Treap();
	~Treap();
	void Insert(Node<TKey, TValue>* newnode);
	inline int GetWidth();
private:
	Node<TKey, TValue>*& insert_(Node<TKey, TValue>*& subroot, \
		Node<TKey, TValue>* newnode);
	void split_(Node<TKey, TValue>* subroot, Node<TKey, TValue>*& left, \
		Node<TKey, TValue>*& right, int key);
	void clear_(Node<TKey, TValue>*& node);
};




int main()
{
	BSTree<int, int>* naivetree = new BSTree<int, int>();
	Treap<int, int>* cartesiantree = new Treap<int, int>();
	int n = 0, x = 0, y = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d %d", &x, &y);
		naivetree->Insert(x);
		cartesiantree->Insert(new Node<int, int>(x, y));
	}
	printf("%d", cartesiantree->GetWidth() - naivetree->GetWidth());
	delete naivetree;
	delete cartesiantree;
	return 0;
}




template<class TKey, class TValue>
Node<TKey, TValue>::Node(TKey key, TValue value)
{
	this->key = key;
	this->value = value;
	this->left = NULL;
	this->right = NULL;
}

template<class TKey, class TValue>
Node<TKey, TValue>::Node(TKey key, TValue value, \
	Node<TKey, TValue>*& leftNode,\
	Node<TKey, TValue>*& rightNode)
{
	this->key = key;
	this->value = value;
	this->left = leftNode;
	this->right = rightNode;
}

template<class TKey, class TValue>
Node<TKey, TValue>::Node(const Node<TKey, TValue>*& node)
{
	this->key = node->key;
	this->value = node->value;
	this->left = node->left;
	this->right = node->right;
}

template<class TKey, class TValue>
Node<TKey, TValue>& Node<TKey, TValue>::operator=(const Node<TKey, TValue>*& node)
{
	this->key = node->key;
	this->value = node->value;
	this->left = node->left;
	this->right = node->right;
	return *this;
}

template<class TKey, class TValue>
TKey Node<TKey, TValue>::GetKey()
{
	return this->key;
}

template<class TKey, class TValue>
TValue Node<TKey, TValue>::GetValue()
{
	return this->value;
}

template<class TKey, class TValue>
Node<TKey, TValue>*& Node<TKey, TValue>::GetLeft()
{
	return this->left;
}

template<class TKey, class TValue>
Node<TKey, TValue>*& Node<TKey, TValue>::GetRight()
{
	return this->right;
}




template<class TKey, class TValue>
BSTree<TKey, TValue>::BSTree()
{
	root = NULL;
}

template<class TKey, class TValue>
BSTree<TKey, TValue>::~BSTree()
{
	if (root == NULL)
		return;
	std::stack<Node<TKey, TValue>*>* extendedrecursion = \
		new std::stack<Node<TKey, TValue>*>();
	std::stack<Node<TKey, TValue>*>* nodesforremoval = \
		new std::stack<Node<TKey, TValue>*>();
	extendedrecursion->push(root);
	Node<TKey, TValue>* tmpnode = NULL;
	while (!extendedrecursion->empty())
	{
		tmpnode = extendedrecursion->top();
		extendedrecursion->pop();
		nodesforremoval->push(tmpnode);
		if (tmpnode->GetLeft() != NULL)
			extendedrecursion->push(tmpnode->GetLeft());
		if (tmpnode->GetRight() != NULL)
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
int BSTree<TKey, TValue>::GetWidth()
{
	int localwidth = 0, maxwidth = 0;
	std::queue<Node<TKey, TValue>*>* nodesqueue = \
		new std::queue<Node<TKey, TValue>*>();
	nodesqueue->push(root);
	nodesqueue->push(NULL);
	while (!nodesqueue->empty())
	{
		Node<TKey, TValue>* currnode = nodesqueue->front();
		nodesqueue->pop();
		if (currnode == NULL)
		{
			maxwidth = std::max(maxwidth, localwidth);
			localwidth = 0;
			if (nodesqueue->size() != 0)
				nodesqueue->push(NULL);
		}
		else
		{
			localwidth++;
			if (currnode->GetLeft() != NULL)
				nodesqueue->push(currnode->GetLeft());
			if (currnode->GetRight() != NULL)
				nodesqueue->push(currnode->GetRight());
		}
	}
	delete nodesqueue;
	return maxwidth;
}




template<class TKey, class TValue>
Treap<TKey, TValue>::Treap() : root(NULL) {}

template<class TKey, class TValue>
Treap<TKey, TValue>::~Treap()
{
	clear_(this->root);
}

template<class TKey, class TValue>
void Treap<TKey, TValue>::Insert(Node<TKey, TValue>* newnode)
{
	insert_(root, newnode);
}

template<class TKey, class TValue>
int Treap<TKey, TValue>::GetWidth()
{
	int localwidth = 0, maxwidth = 0;
	std::queue<Node<TKey, TValue>*>* nodesqueue = \
		new std::queue<Node<TKey, TValue>*>();
	nodesqueue->push(root);
	nodesqueue->push(NULL);
	while (!nodesqueue->empty())
	{
		Node<TKey, TValue>* currnode = nodesqueue->front();
		nodesqueue->pop();
		if (currnode == NULL)
		{
			maxwidth = std::max(maxwidth, localwidth);
			localwidth = 0;
			if (nodesqueue->size() != 0)
				nodesqueue->push(NULL);
		}
		else
		{
			localwidth++;
			if (currnode->GetLeft() != NULL)
				nodesqueue->push(currnode->GetLeft());
			if (currnode->GetRight() != NULL)
				nodesqueue->push(currnode->GetRight());
		}
	}
	delete nodesqueue;
	return maxwidth;
}

template<class TKey, class TValue>
Node<TKey, TValue>*& Treap<TKey, TValue>::insert_(Node<TKey, TValue>*& subroot, \
	Node<TKey, TValue>* newnode)
{
	if (subroot == NULL)
		return subroot = newnode;
	else
	{
		if (subroot->GetValue() > newnode->GetValue())
		{
			if (subroot->GetKey() > newnode->GetKey())
				insert_(subroot->GetLeft(), newnode);
			else
				insert_(subroot->GetRight(), newnode);
			return subroot;
		}
	}
	split_(subroot, newnode->GetLeft(), newnode->GetRight(), newnode->GetKey());
	return subroot = newnode;
}

template<class TKey, class TValue>
void Treap<TKey, TValue>::split_(Node<TKey, TValue>* subroot, Node<TKey, TValue>*& left, \
	Node<TKey, TValue>*& right, int key)
{
	if (subroot == NULL)
	{
		left = NULL;
		right = NULL;
	}
	else if (subroot->GetKey() < key)
	{
		split_(subroot->GetRight(), subroot->GetRight(), right, key);
		left = subroot;
	}
	else
	{
		split_(subroot->GetLeft(), left, subroot->GetLeft(), key);
		right = subroot;
	}
}

template<class TKey, class TValue>
void Treap<TKey, TValue>::clear_(Node<TKey, TValue>*& node)
{
	if (node == NULL)
		return;
	clear_(node->GetLeft());
	clear_(node->GetRight());
	delete node;
}
