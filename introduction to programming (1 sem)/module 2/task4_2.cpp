/*
 
    Дана последовательность целых чисел a1...an и натуральное число k, такое что для 
    любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. 
    Последовательность может быть очень длинной. Время работы O(n * log(k)). Доп. память O(k). 
    Использовать слияние.
 
 */



#include <assert.h>
#include <iostream>
#include <limits.h>
#include <vector>



using namespace std;


/*
 Local variables:
    >>> subSeqBegin      - list's subsequence index of the begining 
    >>> subSeqEnd        - list's subsequence index of the ending
    >>> subSeqMiddle     - middle of the subsequnce
    >>> buffer           - local buffer, which contains merged subsequences
    >>> bufferIndex      - current index of empty space in buffer
    >>> bufferSize       - maximum size of buffer
    >>> leftSubSeqIndex  - index for sorted list[subSeqBegin : subSeqMiddle]
    >>> rightSubSeqIndex - index for sorted list[subSeqMiddle + 1 : subSeqEnd]
 
    After dividing into two parts by index of subSeqMiddle, executing 
 sort for list[subSeqBegin : subSeqMiddle] and list[subSeqMiddle + 1 : subSeqEnd].
 
    Then merge list[subSeqBegin : subSeqMiddle] and list[subSeqMiddle + 1 : subSeqEnd]
 into one array.
 */
template<class TClass, class TCompare> 
inline void merge(vector<TClass>& list, const TCompare& cmp, 
                  int subSeqBegin, int subSeqEnd, int subSeqMiddle)
{
    int bufferIndex = 0;
    int bufferSize = subSeqEnd - subSeqBegin + 1;
    int leftSubSeqIndex = subSeqBegin;
    int rightSubSeqIndex = subSeqMiddle + 1;
    vector<TClass> buffer(bufferSize);
    for (; leftSubSeqIndex <= subSeqMiddle && rightSubSeqIndex <= subSeqEnd; bufferIndex++)
    {
        if (cmp(list[leftSubSeqIndex], list[rightSubSeqIndex]) == 1)
        {
            buffer[bufferIndex] = list[rightSubSeqIndex++];
        }
        else if (cmp(list[leftSubSeqIndex], list[rightSubSeqIndex]) == -1)
        {
            buffer[bufferIndex] = list[leftSubSeqIndex++];
        }
        else
        {
            buffer[bufferIndex] = list[rightSubSeqIndex++];
        }
    }
    while (leftSubSeqIndex <= subSeqMiddle)
    {
        buffer[bufferIndex++] = list[leftSubSeqIndex++];
    }
    while (rightSubSeqIndex <= subSeqEnd)
    {
        buffer[bufferIndex++] = list[rightSubSeqIndex++];
    }
    for (int index = subSeqBegin; index <= subSeqEnd; index++)
    {
        list[index] = buffer[index - subSeqBegin];
    }
    buffer.clear();
}

/*
 Local variables:
    >>> subSeqBegin  - list's subsequence index of the begining 
    >>> subSeqEnd    - list's subsequence index of the ending
    >>> subSeqMiddle - middle of the subsequnce
 
    After dividing into two parts by index of subSeqMiddle, run sort for 
 list[subSeqBegin : subSeqMiddle] and list[subSeqMiddle + 1 : subSeqEnd].
 
    Then merge list[subSeqBegin : subSeqMiddle] and list[subSeqMiddle + 1 : subSeqEnd]
 into one array.
 */
template<class TClass, class TCompare> 
inline void mergeSortSubSequence(vector<TClass>& list, const TCompare& cmp, 
                                 int subSeqBegin, int subSeqEnd)
{
    if (subSeqBegin >= subSeqEnd)
    {
        return;
    }
    else if (subSeqEnd == (1 + subSeqBegin)) 
    {
        if (cmp(list[subSeqBegin], list[subSeqEnd]) == 1)
        {
            swap(list[subSeqBegin], list[subSeqEnd]);
        }
        return;
    }
    else
    {
        int subSeqMiddle = (subSeqBegin + subSeqEnd) / 2;
        if (subSeqMiddle < subSeqBegin || subSeqMiddle > subSeqEnd)
        {
            return;
        }
        mergeSortSubSequence(list, cmp, subSeqBegin, subSeqMiddle);
        mergeSortSubSequence(list, cmp, subSeqMiddle + 1, subSeqEnd);
        merge(list, cmp, subSeqBegin, subSeqEnd, (subSeqBegin + subSeqEnd) / 2);
    }
    return;
}

template<class TClass, class TCompare> 
inline void mergeSort(vector<TClass>& list, const TCompare& cmp)
{
    return mergeSortSubSequence(list, cmp, 0, list.size() - 1);
}



//  1 - lhs > rhs
//  0 - lhs == rhs
// -1 - lhs < rhs
int compare(const int& lhs, const int& rhs)
{
    if (lhs > rhs)
    {
        return 1;
    }
    else if (lhs == rhs)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//  Asymptotics of the algorithm:
//  >>>  M(2 * k) ~ M(k)
//  >>>  T(n * log(k))
inline void scanAndSort(vector<int>& list, int n, int k)
{
    int inputedNumbers = 0;
    int outputedNumbers = 0;
    // read first 2*k elements
    for (int index = 0; index < k * 2; index++)
    {
        if (inputedNumbers >= n)
        {
            // if n % k != 0 then push
            // INT_MAX values in the end of list
            // and sort with ones
            list[index] = INT_MAX;
        }
        else
        {
            assert(cin >> list[index]);
            inputedNumbers++;
        }
    }
    // sort first (2*k)-elements, so first k-elements
    // will be a sorted subsequnce of n-elements sequence
    mergeSort(list, compare);
    for (int index = 0; index < k; index++)
    {
        cout << list[index] << " ";
        outputedNumbers++;
    }
    
    while (outputedNumbers < n)
    {
        for (int index = 0; index < k; index++)
        {
            if (inputedNumbers >= n)
            {
                list[index] = INT_MAX;
            }
            else
            {
                assert(cin >> list[index]);
                inputedNumbers++;
            }
        }
        mergeSort(list, compare);
        for (int index = 0; index < k; index++)
        {  
            // if all n-elements are sorted, so
            // return function and terminate program
            if(outputedNumbers >= n)
            {
                break;
            }
            cout << list[index] << " ";
            outputedNumbers++;
        }
    }
    return;
}



int main ()
{
    ios_base::sync_with_stdio(false);
    int n = 0, k = 0;
    assert(cin >> n >> k);
    assert(n > 0 && k > 0);
    vector<int> list(k * 2);
    scanAndSort(list, n, k);
    list.clear();
    return 0;
}