/*
 *
 *
 * Реализуйте структуру данных типа “множество строк” на основе
 * динамической хеш-таблицы с открытой адресацией. Хранимые
 * строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления
 * значения многочлена методом Горнера. Начальный размер таблицы
 * должен быть равным 8-ми. Перехеширование выполняйте при добавлении
 * элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество,
 * удаления строки из множества и проверки принадлежности данной строки
 * множеству.
 * Для разрешения коллизий используйте двойное хеширование.
 *
 *
 */



#include <assert.h>
#include <iostream>
#include <vector>



using namespace std;



template<class T, class Hashing>
class HashTable
{
public:

  static const int resizefactor = 2;
  static const int hashprime = 1000000007;
  static const int addhash1 = 1073676287;

private:

  T EMPTY_CELL, DELETED_CELL;
  float loadfactor;
  int buffersize;
  int usedsize;
  vector<T> buffer;
  Hashing hashfunc;

public:

  HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE);
  HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE, \
    int newbuffersize);
  HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE, \
    int newbuffersize, float newloadfactor);
  ~HashTable();
  bool Add(T object);
  bool Delete(T object);
  bool Has(T object);

private:

  bool isDeleted(int index);
  bool isEmpty(int index);
  bool isOverflowed();
  int getHash(T object, int hashprm = hashprime);
  void rehash();
};



template<class T, class Hashing> inline
string UnitTest(HashTable<T, Hashing> *hashtable, string cmd, T object);

class StringHash
{
public:
  StringHash() = default;
  int operator()(string str, int hashprm, int modulo);
};



int main()
{
  StringHash hashfunc;
  HashTable<string, StringHash> *hashtable = \
    new HashTable<string, StringHash>(hashfunc, "EMPTY", "DELETED");
  string cmd = "", value = "";
  while (cin >> cmd >> value)
  {
    if (cin.fail() || cin.eof())
      break;
    else
      cout << UnitTest(hashtable, cmd, value) << endl;
  }
  delete hashtable;
  return 0;
}



template<class T, class Hashing> inline
string UnitTest(HashTable<T, Hashing> *hashtable, string cmd, T object)
{
  if (cmd == "+")
    return hashtable->Add(object) ? "OK" : "FAIL";
  else if (cmd == "-")
    return hashtable->Delete(object) ? "OK" : "FAIL";
  else if (cmd == "?")
    return (hashtable->Has(object)) ? "OK" : "FAIL";
  else
    return "Error: invalid input";
}

int StringHash::operator()(string str, int hashprm, int modulo)
{
  long long hash = 0LL;
  for (int i = 0; i < str.size(); i++)
    hash += (hash * hashprm + (int) str[i]) % modulo;
  return (int) (hash % modulo);
}



template<class T, class Hashing>
HashTable<T, Hashing>::HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE)
{
  buffersize = 8;
  loadfactor = 0.75f;
  usedsize = 0;
  EMPTY_CELL = EMPTY_TYPE;
  DELETED_CELL = DELETED_TYPE;
  assert(loadfactor >= 0.5f && loadfactor <= 1.0f);
  assert(buffersize >= 1);
  buffer.assign(buffersize, EMPTY_CELL);
}

template<class T, class Hashing>
HashTable<T, Hashing>::HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE, \
  int newbuffersize)
{
  buffersize = newbuffersize;
  loadfactor = 0.75f;
  usedsize = 0;
  EMPTY_CELL = EMPTY_TYPE;
  DELETED_CELL = DELETED_TYPE;
  assert(loadfactor >= 0.5f && loadfactor <= 1.0f);
  assert(buffersize >= 1);
  buffer.assign(buffersize, EMPTY_CELL);
}

template<class T, class Hashing>
HashTable<T, Hashing>::HashTable(Hashing hashfunc, T EMPTY_TYPE, T DELETED_TYPE, \
  int newbuffersize, float newloadfactor)
{
  buffersize = newbuffersize;
  loadfactor = newloadfactor;
  usedsize = 0;
  EMPTY_CELL = EMPTY_TYPE;
  DELETED_CELL = DELETED_TYPE;
  assert(loadfactor >= 0.5f && loadfactor <= 1.0f);
  assert(buffersize >= 1);
  buffer.assign(buffersize, EMPTY_CELL);
}

template<class T, class Hashing>
HashTable<T, Hashing>::~HashTable()
{
  buffer.clear();
  buffersize = 0;
  usedsize = 0;
}

template<class T, class Hashing>
bool HashTable<T, Hashing>::Add(T object)
{
  if (isOverflowed())
    rehash();
  int position = 0;
  int deletedposition = -1;
  long long firsthash = getHash(object);
  long long secondhash = 2LL * getHash(object, addhash1) + 1LL;
  for (int i = 0; i < buffersize; i++)
  {
    position = (firsthash + i * secondhash) % buffersize;
    if (isEmpty(position))
    {
      if (deletedposition == -1)
        buffer[position] = object;
      else
        buffer[deletedposition] = object;
      usedsize++;
      return true;
    }
    if (isDeleted(position) && deletedposition == -1)
      deletedposition = position;
    if (object == buffer[position])
      return false;
  }
  if (deletedposition != -1)
  {
    buffer[deletedposition] = object;
    usedsize++;
    return true;
  }
  return false;
}

template<class T, class Hashing>
bool HashTable<T, Hashing>::Delete(T object)
{
  int position = 0;
  long long firsthash = getHash(object);
  long long secondhash = 2LL * getHash(object, addhash1) + 1LL;
  for (int i = 0; i < buffersize; i++)
  {
    position = (firsthash + i * secondhash) % buffersize;
    if (buffer[position] == object)
    {
      buffer[position] = DELETED_CELL;
      usedsize--;
      return true;
    }
    if (isEmpty(position))
      return false;
  }
  return false;
}

template<class T, class Hashing>
bool HashTable<T, Hashing>::Has(T object)
{
  int position = 0;
  int deletedposition = -1;
  long long firsthash = getHash(object);
  long long secondhash = 2LL * getHash(object, addhash1) + 1LL;
  for (int i = 0; i < buffersize; i++)
  {
    position = (int) ((firsthash + i * secondhash) % buffersize);
    if (isEmpty(position))
      return false;
    if (buffer[position] == object)
    {
      if (deletedposition != -1)
        swap(buffer[position], buffer[deletedposition]);
      return true;
    }
    else if (isEmpty(position))
      return false;
    else if (isDeleted(position) && deletedposition == -1)
      deletedposition = position;
    else
      continue;
  }
  return false;
}


template<class T, class Hashing>
bool HashTable<T, Hashing>::isDeleted(int index)
{
  return buffer[index] == DELETED_CELL;
}

template<class T, class Hashing>
bool HashTable<T, Hashing>::isEmpty(int index)
{
  return buffer[index] == EMPTY_CELL;
}

template<class T, class Hashing>
bool HashTable<T, Hashing>::isOverflowed()
{
  return ((1.0f * usedsize) / (1.0f * buffersize)) >= loadfactor;
}

template<class T, class Hashing>
int HashTable<T, Hashing>::getHash(T object, int hashprm)
{
  return hashfunc(object, hashprm, buffersize);
}

template<class T, class Hashing>
void HashTable<T, Hashing>::rehash()
{
  buffersize *= resizefactor;
  vector<T> newbuffer(buffersize, EMPTY_CELL);
  int position = 0;
  for (int i = 0; i < (buffersize / resizefactor); i++)
  {
    if (!isEmpty(i) && !isDeleted(i))
    {
      long long firsthash = getHash(buffer[i]);
      long long secondhash = 2LL * getHash(buffer[i], addhash1) + 1LL;
      for (int j = 0; j < buffersize; j++)
      {
        position = (firsthash + j * secondhash) % buffersize;
        assert(buffer[i] != newbuffer[position]);
        if (newbuffer[position] == EMPTY_CELL)
        {
          newbuffer[position] = buffer[i];
          break;
        }
      }
    }
  }
  buffer = vector<T>(newbuffer.begin(), newbuffer.end());
  newbuffer.clear();
}