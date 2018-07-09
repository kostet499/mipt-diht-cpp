/*
 
    Напишите программу, печатающую набор строк в лексикографическом порядке. 
    Строки разделяются символом перевода строки ’∖n’. Если последний символ 
    в потоке ввода ’∖n’, считать, что после него нет пустой строки. Максимальная 
    длина строки 255 символов. Написать свою функцию сравнения строк.

 */


#include <assert.h>
#include <iostream>
#include <string>
#include <vector>


using namespace std;



inline void readLines(vector<string>& list)
{
    for (int i = 0; i < list.size(); i++)
    {
        cin >> list[i];
    }
}

inline void printLines(vector<string>& list)
{
    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i] << endl;
    }
}




template<class TClass, class TCompare> 
inline void insertionSort(vector<TClass>& list, TCompare tcompare)
{
    for (int index = 1; index < list.size(); index++)
    {
        TClass newElement = list.at(index);
        int previousIndex = index - 1;
        while (previousIndex >= 0 && (tcompare(list[previousIndex], newElement) == 1))
        {
            list[previousIndex + 1] = list[previousIndex];
            previousIndex--;
        }
        list[previousIndex + 1] = newElement;
    }
}

//  1 - lhs > rhs
//  0 - lhs == rhs
// -1 - lhs < rhs
inline int compare(const string& lhs, const string& rhs)
{
    for (size_t i = 0; i < min(lhs.size(), rhs.size()); i++)
    {
        if (lhs.at(i) > rhs.at(i))
        {
            return 1;
        }
        else if (lhs.at(i) < rhs.at(i))
        {
            return -1;
        }
        else
        {
            continue;
        }
    }
    if (lhs.size() > rhs.size())
    {
        return 1;
    }
    else if (lhs.size() < rhs.size())
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


int main ()
{
    int n = 0;
    cin >> n;
    vector<string> list(n, "");
    readLines(list);
    insertionSort(list, compare);
    printLines(list);
    list.clear();
    return 0;
}