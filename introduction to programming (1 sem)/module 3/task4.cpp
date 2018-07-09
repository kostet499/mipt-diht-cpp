#include <assert.h>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <stack>

#include "Huffman.h"


using namespace std;


typedef char byte;



class HNode
{

private:

  bool leaf;
  byte letter;
  int frequency;
  HNode *left;
  HNode *right;

public:

  HNode(byte letter, int frequency, bool leaf, HNode *left, HNode *right);
  HNode(const HNode *&hnode);
  HNode &operator=(const HNode *&hnode);
  HNode *&GetLeft();
  HNode *&GetRight();
  bool IsLeaf();
  byte &GetLetter();
  int GetFrequency();

};


struct HNodeComparator
{
  bool operator()(HNode *arg0, HNode *arg1)
  {
    return arg0->GetFrequency() > arg1->GetFrequency();
  }
};


class HuffmanTree
{

public:

  vector<byte> dataforzip;
  vector<bool> packedtree;

private:

  HNode *root;
  map<byte, int> freqcounter;
  priority_queue<HNode *, vector<HNode *>, HNodeComparator> heap;

public:

  HuffmanTree(const string &inputstreamdata);
  ~HuffmanTree();
  void CompleteCodeTable(map<byte, vector<bool> > &encodetable, vector<bool> &path);
  void GetByte(const vector<bool> &code, byte &letter);
  void GetPackedTree(vector<bool> &buffer);

private:

  void calculateFrequency_();
  void buildTree_();
  void clear_(HNode *&hnode);
  void completeCodeTable_(map<byte, vector<bool> > &encodetable, \
    vector<bool> &path, HNode *&hnode);
  void getByte_(const vector<bool> &code, byte &letter, HNode *&hnode, int bit = 0);
  void packByte_(byte letter);
  void getPackedTreeSize_(vector<bool> &buffer);
  void packTree_(HNode *&hnode);
};


class HuffmanZipper
{

private:

  HuffmanTree *htree;
  map<byte, vector<bool> > encodetable;

public:

  HuffmanZipper(const string &asciiinput);
  ~HuffmanZipper();
  void GetCode(vector<bool> &buffer, const byte &letter);
  void PutCompressedTree(vector<byte> &compresseddata);
  void PutCompressedText(vector<byte> &compresseddata);
};


class HuffmanUnzipper
{

private:

  HNode* root;
  stack<HNode*> nodes;
  unsigned int treesize = 0;

public:

  HuffmanUnzipper() = default;
  ~HuffmanUnzipper();
  string GetUncompressedText(const string &bynarystring);

private:

  string decodeText_(const vector<bool> &bynaryarray);
  void toBinaryArray_(const string &str, vector<bool> &array);
  void decodeTree_(const vector<bool> &bynaryarray);
  void deleteNode_(HNode *currentnode);
};






void Encode(IInputStream &original, IOutputStream &compressed)
{
  string asciiinput = "";
  byte inputvalue = 0;
  while (original.Read(inputvalue))
    asciiinput.push_back(inputvalue);
  HuffmanZipper *zipper = new HuffmanZipper(asciiinput);
  asciiinput.clear();
  vector<byte> outputstreamdata;
  zipper->PutCompressedTree(outputstreamdata);
  zipper->PutCompressedText(outputstreamdata);
  for (unsigned char letter : outputstreamdata)
    compressed.Write(letter);
  delete zipper;
  outputstreamdata.clear();
}

void Decode(IInputStream &compressed, IOutputStream &original)
{
  string bynaryinput = "";
  byte inputvalue = 0;
  while (compressed.Read(inputvalue))
    bynaryinput.push_back(inputvalue);
  HuffmanUnzipper *unzipper = new HuffmanUnzipper();
  for (unsigned char letter : unzipper->GetUncompressedText(bynaryinput))
    original.Write(letter);
  delete unzipper;
  bynaryinput.clear();
}






/******************** Methods of class HNode ********************/

HNode::HNode(byte letter, int frequency, bool leaf, HNode *left, HNode *right)
{
  this->letter = letter;
  this->frequency = frequency;
  this->leaf = leaf;
  this->left = left;
  this->right = right;
}

HNode::HNode(const HNode *&hnode)
{
  this->letter = hnode->letter;
  this->frequency = hnode->frequency;
  this->leaf = hnode->leaf;
  this->left = hnode->left;
  this->right = hnode->right;
}

HNode &HNode::operator=(const HNode *&hnode)
{
  this->letter = hnode->letter;
  this->frequency = hnode->frequency;
  this->leaf = hnode->leaf;
  this->left = hnode->left;
  this->right = hnode->right;
  return *this;
}

bool HNode::IsLeaf()
{
  return this->leaf;
}

byte &HNode::GetLetter()
{
  return this->letter;
}

int HNode::GetFrequency()
{
  return this->frequency;
}

HNode *&HNode::GetLeft()
{
  return this->left;
}

HNode *&HNode::GetRight()
{
  return this->right;
}




/******************** Methods of class HuffmanTree ********************/

HuffmanTree::HuffmanTree(const string &inputstreamdata)
{
  dataforzip = vector<byte>(inputstreamdata.begin(), inputstreamdata.end());
  this->calculateFrequency_();
  this->buildTree_();
  root = heap.top();
}

HuffmanTree::~HuffmanTree()
{
  if (!dataforzip.empty())
    dataforzip.clear();
  if (!freqcounter.empty())
    freqcounter.clear();
  while (!heap.empty())
    heap.pop();
  clear_(root);
}

void HuffmanTree::CompleteCodeTable(map<byte, vector<bool> > &encodetable, \
 vector<bool> &path)
{
  completeCodeTable_(encodetable, path, root);
}

void HuffmanTree::GetByte(const vector<bool> &code, byte &letter)
{
  getByte_(code, letter, this->root);
}

void HuffmanTree::GetPackedTree(vector<bool> &buffer)
{
  if (packedtree.size() == 0)
    packTree_(root);
  getPackedTreeSize_(buffer);
  for (int i = 0; i < packedtree.size(); i++)
    buffer.push_back(packedtree[i]);
}

void HuffmanTree::calculateFrequency_()
{
  for (int i = 0; i < dataforzip.size(); i++)
    freqcounter[dataforzip[i]]++;
}

void HuffmanTree::buildTree_()
{
  HNode *prevnode = nullptr;
  for (map<byte, int>::iterator it = freqcounter.begin(); it != freqcounter.end(); it++)
  {
    prevnode = new HNode(it->first, it->second, true, nullptr, nullptr);
    heap.push(prevnode);
  }
  HNode *currnode = nullptr;
  while (heap.size() > 1)
  {
    prevnode = heap.top();
    heap.pop();
    currnode = new HNode('\0', prevnode->GetFrequency() + \
      heap.top()->GetFrequency(), false, prevnode, heap.top());
    heap.pop();
    heap.push(currnode);
  }
}

void HuffmanTree::clear_(HNode *&hnode)
{
  if (hnode == nullptr)
    return;
  clear_(hnode->GetLeft());
  clear_(hnode->GetRight());
  delete hnode;
}

void HuffmanTree::completeCodeTable_(map<byte, vector<bool> > &encodetable, \
 vector<bool> &path, HNode *&hnode)
{
  if (hnode == nullptr)
    path.pop_back();
  else
  {
    if (hnode->IsLeaf())
    {
      encodetable[hnode->GetLetter()] = vector<bool>(path.begin(), path.end());
      path.pop_back();
    }
    else
    {
      path.push_back(false);
      completeCodeTable_(encodetable, path, hnode->GetLeft());
      path.push_back(true);
      completeCodeTable_(encodetable, path, hnode->GetRight());
      if (path.size() == 0)
        return;
      else
        path.pop_back();
    }
  }
}

void HuffmanTree::getByte_(const vector<bool> &code, byte &letter, \
 HNode *&hnode, int bit)
{
  if (bit == code.size())
  {
    assert(hnode->IsLeaf());
    letter = hnode->GetLetter();
  }
  else
  {
    if (!code[bit])
      getByte_(code, letter, hnode->GetLeft(), bit + 1);
    else
      getByte_(code, letter, hnode->GetRight(), bit + 1);
    return;
  }
}

void HuffmanTree::packByte_(byte letter)
{
  for (int i = 7; i >= 0; i--)
    packedtree.push_back((letter >> i) & 1);
}

void HuffmanTree::getPackedTreeSize_(vector<bool> &buffer)
{
  int packedsize = packedtree.size();
  for (int i = 31; i >= 0; i--)
    buffer.push_back((packedsize >> i) & 1);
}

void HuffmanTree::packTree_(HNode *&hnode)
{
  if (!hnode) return;
  packTree_(hnode->GetLeft());
  packTree_(hnode->GetRight());
  if (!hnode->IsLeaf())
    packedtree.push_back(true);
  else
    packedtree.push_back(false), packByte_(hnode->GetLetter());
}




/******************** Methods of class HuffmanZipper ********************/

HuffmanZipper::HuffmanZipper(const string &asciiinput)
{
  htree = new HuffmanTree(asciiinput);
  vector<bool> path;
  htree->CompleteCodeTable(encodetable, path);
  if (!path.empty())
    path.clear();
}

HuffmanZipper::~HuffmanZipper()
{
  delete htree;
  if (!encodetable.empty())
    encodetable.clear();
}

void HuffmanZipper::GetCode(vector<bool> &buffer, const byte &letter)
{
  buffer.clear();
  buffer = vector<bool>(encodetable[letter].begin(), encodetable[letter].end());
}

void HuffmanZipper::PutCompressedTree(vector<byte> &compresseddata)
{
  vector<bool> buffer;
  htree->GetPackedTree(buffer);
  byte outputvalue = 0;
  for (int i = 0; i < buffer.size(); i++)
  {
    outputvalue |= buffer[i] << (7 - (i % 8));
    if (i % 8 == 7)
    {
      compresseddata.push_back(outputvalue);
      outputvalue = 0;
    }
  }
  if (outputvalue != 0)
    compresseddata.push_back(outputvalue);
  buffer.clear();
}

void HuffmanZipper::PutCompressedText(vector<byte> &compresseddata)
{
  unsigned char bitindex = 0;
  vector<bool> buffer;
  byte outputvalue = 0;
  int unusedcount = 0;
  // Read count of unused bits, putted with PutCompressedTree,
  // because tree ends with 1
  for (int i = 0; i < 8; i++)
    if (!(compresseddata[compresseddata.size() - 1] & (1 << i)))
      unusedcount++;
    else break;
  int unusedcopy = unusedcount;
  int treesize = compresseddata.size();
  // Put empty char to insert count of unused bits there later
  compresseddata.push_back(0);
  for (int i = 0; i < htree->dataforzip.size(); i++)
  {
    GetCode(buffer, htree->dataforzip[i]);
    for (int j = 0; j < buffer.size(); j++)
    {
      outputvalue = (outputvalue << 1) | buffer[j];
      bitindex++;
      if (unusedcount != 0 && bitindex == unusedcount)
      {
        bitindex = 0;
        for (int z = 0; z < unusedcount; z++)
          compresseddata[compresseddata.size() - 1] |= outputvalue & (1 << z);
        outputvalue = 0;
        unusedcount = 0;
      }
      if (bitindex == 8)
      {
        bitindex = 0;
        compresseddata.push_back(outputvalue);
        outputvalue = 0;
      }
    }
    buffer.clear();
  }
  // Put count of unused bits
  compresseddata[treesize - 1] |= (8 - bitindex) >> (8 - unusedcopy);
  compresseddata[treesize] |= (8 - bitindex) << unusedcopy;
  if (bitindex != 0)
    compresseddata.push_back(outputvalue << (8 - bitindex));
}




/******************** Methods of class HuffmanUnzipper ********************/

string HuffmanUnzipper::GetUncompressedText(const string &bynarystring)
{
  byte tmp = 0;
  stack<HNode *> nodes;
  vector<bool> bynaryarray;
  toBinaryArray_(bynarystring, bynaryarray);
  decodeTree_(bynaryarray);
  return decodeText_(bynaryarray);
}

HuffmanUnzipper::~HuffmanUnzipper()
{
  deleteNode_(root);
}

void HuffmanUnzipper::toBinaryArray_(const string &str, vector<bool> &array)
{
  for (int i = 0; i < str.size(); i++)
    for (int j = 7; j >= 0; j--)
      array.push_back((str[i] >> j) & 1);
}

void HuffmanUnzipper::decodeTree_(const vector<bool> &bynaryarray)
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++)
    {
      treesize <<= 1;
      treesize |= bynaryarray[i * 8 + j] ? 1 : 0;
    }
  int scannedbits = 0;
  int symbolcode = 0;
  for (int i = 32; i < treesize + 32; i++)
  {
    if (scannedbits != 9)
    {
      if (bynaryarray[i] && scannedbits == 0)
      {
        HNode *right = nodes.top();
        nodes.pop();
        HNode *left = nodes.top();
        nodes.pop();
        HNode *node = new HNode(0, 0, false, left, right);
        nodes.push(node);
      }
      else
      {
        if (scannedbits != 0)
          symbolcode |= (bynaryarray[i] ? 1 : 0) << (8 - scannedbits);
        scannedbits++;
      }
    }
    else
    {
      scannedbits = 0;
      nodes.push(new HNode(symbolcode, 0, true, nullptr, nullptr));
      symbolcode = 0;
      i--;
    }
  }
  root = nodes.top();
  nodes.pop();
}

string HuffmanUnzipper::decodeText_(const vector<bool> &bynaryarray)
{
  byte fakebits = 0;
  string result = "";
  for (int i = treesize + 32; i < treesize + 40; i++)
    fakebits |= bynaryarray[i] << (7 - (i - treesize));
  HNode *candidate = root;
  for (int i = treesize + 40; i < bynaryarray.size() - fakebits; i++)
  {
    if (!bynaryarray[i])
      candidate = candidate->GetLeft();
    else
      candidate = candidate->GetRight();
    if (candidate && candidate->IsLeaf())
    {
      result += candidate->GetLetter();
      candidate = root;
    }
  }
  return result;
}

void HuffmanUnzipper::deleteNode_(HNode *currentnode)
{
  if (!currentnode)
    return;
  deleteNode_(currentnode->GetRight());
  deleteNode_(currentnode->GetLeft());
  delete currentnode;
}