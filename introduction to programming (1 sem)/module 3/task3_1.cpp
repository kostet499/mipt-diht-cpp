/*
 *
 * В одной военной части решили построить в одну шеренгу по росту.
 * Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
 * а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
 * Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены
 * по росту – сначала самые высокие, а в конце – самые низкие. За расстановку
 * солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
 * в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику
 * правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
 * перед каким солдатом в строе он должен становится.
 * Требуемая скорость выполнения команды - O(log n).
 *
 */




#include <iostream>


class Node
{

public:

  int key;
  int height;
  int value;
  Node *left;
  Node *right;

  Node(int nodekey, int nodevalue)
  {
    key = nodekey;
    height = 1;
    value = nodevalue;
    left = nullptr;
    right = nullptr;
  }
};


class AVL
{

private:

  Node *root;

public:

  AVL();
  ~AVL();
  void Insert(int key, int value, int &position);
  void Remove(int position);

private:

  void deleteTree_(Node *&node);
  void updateHeight_(Node *node);
  void updateValue_(Node *node);
  void updateNodeInfo_(Node *node);
  int getHeight_(Node *node);
  int getValue_(Node *node);
  int getBalance_(Node *node);
  Node *insert_(Node *node, int key, int value, int &position);
  Node *remove_(Node *node, int position);
  Node *rotateLeft_(Node *node);
  Node *rotateRight_(Node *node);
  Node *balance_(Node *node);
  Node *findMax_(Node *node);
  Node *removeMax_(Node *node);
};


void HandleRequest(AVL *tree);


int main()
{
  AVL *tree = new AVL();
  int n = 0;
  std::cin >> n;
  for (int i = 0; i < n; i++)
    HandleRequest(tree);
  delete tree;
  return 0;
}


void HandleRequest(AVL *tree)
{
  int command = 0, height = 0, index = 0;
  std::cin >> command;
  switch (command)
  {
    case 1:
      std::cin >> height;
      index = 0;
      tree->Insert(height, 1, index);
      std::cout << index << std::endl;
      break;
    case 2:
      std::cin >> index;
      index++;
      tree->Remove(index);
      break;
  }
}


AVL::AVL()
{
  this->root = nullptr;
}

AVL::~AVL()
{
  deleteTree_(root);
}

void AVL::Insert(int key, int value, int &position)
{
  root = insert_(root, key, value, position);
}

void AVL::Remove(int position)
{
  root = remove_(root, position);
}

void AVL::deleteTree_(Node *&node)
{
  if (node == nullptr)
    return;
  deleteTree_(node->left);
  deleteTree_(node->right);
  delete node;
}

Node *AVL::insert_(Node *node, int key, int value, int &position)
{
  if (node == nullptr)
    return new Node(key, value);
  else if (key > node->key)
    node->right = insert_(node->right, key, value, position);
  else
  {
    if (key < node->key)
    {
      position += getValue_(node->right) + 1;
      node->left = insert_(node->left, key, value, position);
    }
  }
  return balance_(node);
}

Node *AVL::remove_(Node *node, int position)
{
  if (node == nullptr || position > getValue_(node))
    return nullptr;
  int currentpos = getValue_(node->right) + 1;
  if (position == currentpos)
  {
    Node *left = node->left;
    Node *right = node->right;
    delete node;
    if (left == nullptr)
      return right;
    Node *max = findMax_(left);
    max->left = removeMax_(left);
    max->right = right;
    return balance_(max);
  }
  else if (position < currentpos)
    node->right = remove_(node->right, position);
  else
    node->left = remove_(node->left, position - currentpos);
  return balance_(node);
}

int AVL::getHeight_(Node *node)
{
  return node ? node->height : 0;
}

int AVL::getValue_(Node *node)
{
  return node ? node->value : 0;
}

void AVL::updateHeight_(Node *node)
{
  node->height = \
            std::max(getHeight_(node->left), getHeight_(node->right)) + 1;
}

void AVL::updateValue_(Node *node)
{
  node->value = \
            getValue_(node->left) + getValue_(node->right) + 1;
}

void AVL::updateNodeInfo_(Node *node)
{
  updateHeight_(node);
  updateValue_(node);
}

Node *AVL::rotateLeft_(Node *node)
{
  Node *right = node->right;
  node->right = right->left;
  right->left = node;
  updateNodeInfo_(node);
  updateNodeInfo_(right);
  return right;
}

Node *AVL::rotateRight_(Node *node)
{
  Node *left = node->left;
  node->left = left->right;
  left->right = node;
  updateNodeInfo_(node);
  updateNodeInfo_(left);
  return left;
}

int AVL::getBalance_(Node *node)
{
  return getHeight_(node->right) - getHeight_(node->left);
}

Node *AVL::balance_(Node *node)
{
  if (node == nullptr)
    return nullptr;
  updateNodeInfo_(node);
  int balance = getBalance_(node);
  if (balance == -2)
  {
    if (getBalance_(node->left) == 1)
      node->left = rotateLeft_(node->left);
    return rotateRight_(node);
  }
  else if (balance == 2)
  {
    if (getBalance_(node->right) == -1)
      node->right = rotateRight_(node->right);
    return rotateLeft_(node);
  }
  return node;
}

Node *AVL::findMax_(Node *node)
{
  if (node->right == nullptr)
    return node;
  else
    return findMax_(node->right);
}

Node *AVL::removeMax_(Node *node)
{
  if (node->right == nullptr)
    return node->left;
  node->right = removeMax_(node->right);
  return balance_(node);
}