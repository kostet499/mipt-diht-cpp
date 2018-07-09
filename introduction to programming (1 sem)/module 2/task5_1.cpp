/*
 
    5 1. MSD для строк.
    Дан массив строк. Количество строк не больше 100000. 
    Отсортировать массив методом поразрядной сортировки MSD 
    по символам. Размер алфавита - 256 символов. Последний 
    символ строки = ‘0’.
 
 */



#include <assert.h>
#include <iostream>
#include <string>
#include <vector>



using namespace std;



inline int readLines(vector<string>& list);
inline void printLines(vector<string>& list);



static const int ALPHABET_SIZE = 256;

// if character at "position" is not exist in str -> return 0
inline unsigned int charAt(const string& str, int position)
{
    if (position >= str.length())
        return 0;
    else
        return (int)str[position];
}


/*
 Local variables:
    >>> newList     - contains sorted list[subSeqBegin:subSeqEnd]
    >>> maxLength   - max length of all strings
    >>> position    - number of digit for sort
    >>> subSeqBegin - list's subsequence index of the begining 
    >>> subSeqEnd   - list's subsequence index of the ending
    >>> counter     - contains the amount for every character
 */
void countingSort(vector<string>& list, int maxLength, 
                  vector<string>& newList, int subSeqBegin, int subSeqEnd, int position)
{
    vector<int> counter(ALPHABET_SIZE, 0);
    if (position >= maxLength || subSeqEnd <= subSeqBegin)
        return;
    int asciiCode = -1;
    
    // Firstly, we precalc the amount for every character of the alphabet 
    // in each string of the list at the "position".
    for (int index = subSeqBegin; index <= subSeqEnd; index++)
    {
        asciiCode = charAt(list[index], position);
        counter[asciiCode]++;
    }
    for (int index = 1; index < ALPHABET_SIZE; index++)
        counter[index] += counter[index - 1];
    
    // Sort by reshuffling at "position"
    for (int index = subSeqEnd; index >= subSeqBegin; index--)
    {
        asciiCode = charAt(list[index], position);
        newList[subSeqBegin - 1 + counter[asciiCode]--] = list[index];
    }
    
    // Copy sorted subsequence
    for (int index = subSeqBegin; index <= subSeqEnd; index++)
        list[index] = newList[index];
    
    // Recursively calls sort at less significant digits
    for (int index = 1; index < ALPHABET_SIZE - 1; index++)
    {
        if (counter[index + 1] - 1 - counter[index] >= 1)
        {
            countingSort(list, maxLength, newList, subSeqBegin + counter[index], 
                         subSeqBegin + counter[index + 1] - 1, position + 1);
        }
    }
    counter.clear();
}


/*
 Local variables:
    >>> maxLength   - max length of all strings
    >>> position    - start msdSort from the most significant digit
    >>> subSeqBegin - list's subsequence index of the begining 
    >>> subSeqEnd   - list's subsequence index of the ending
 */
void msdSort(vector<string>& list, int maxLength)
{
    vector<string> newList(list.size());
    int subSeqBegin = 0, subSeqEnd = (int)list.size() - 1, position = 0;
    countingSort(list, maxLength, newList, subSeqBegin, subSeqEnd, position);
    newList.clear();
}



int main ()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    vector<string> list;
    int maxLength = readLines(list);
    msdSort(list, maxLength); 
    printLines(list);
    list.clear();
    return 0;
}



inline int readLines(vector<string>& list)
{
    int maxLength = 0;
    string tmp = "";
    while(cin >> tmp)
    {
        if (cin.fail() || cin.eof())
        {
            break;
        }
        if (tmp.length() > maxLength)
        {
            maxLength = (int)tmp.length();
        }
        list.push_back(tmp);
    }
    return maxLength;
}

inline void printLines(vector<string>& list)
{
    for (int index = 0; index < list.size(); index++)
    {
        cout << list[index] << endl;
    }
}