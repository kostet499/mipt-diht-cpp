/*
 
 Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..10^9]
 размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое 
 бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
 Напишите нерекурсивный алгоритм.
 Требования к дополнительной памяти: O(n).
 Требуемое среднее время работы: O(n).
 Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
 Описание для случая прохода от начала массива к концу:
 Выбирается опорный элемент.
 Опорный элемент меняется с последним элементом массива.
 Во время работы Partition в начале массива содержатся элементы, не бОльшие 
 опорного.Затем располагаются элементы, строго бОльшие опорного. В конце массива 
 лежат нерассмотренные элементы. Последним элементом лежит опорный.
 Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
 Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
 Шаг алгоритма. Рассматривается элемент, на который указывает j. 
 Если он больше опорного, то сдвигаем j. Если он не больше опорного, то меняем a[i] и a[j] местами, 
 сдвигаем i и сдвигаем j.
 В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
 
 
 3_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
 Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
 
 */



#include <assert.h>
#include <iostream>
#include <vector>



using namespace std;



inline void readElements(vector<int>& list);



/*
 Return index of the pivot
    (method: median of triple: begin, middle, end)
 
 Local variables:
    >>> list         - sequence for sort
    >>> subSeqBegin  - list's subsequence index of the begining 
    >>> subSeqEnd    - list's subsequence index of the ending
    >>> subSeqMiddle - middle index of the subsequnce
    >>> chooseMiddle - boolean variable, which shows that:
        min(list[subSeqEnd], list[subSeqBegin]) <= list[subSeqMiddle] <= max(list[subSeqEnd], list[subSeqBegin]) 
    >>> chooseEnd    - boolean variable, which shows that:
        min(list[subSeqMiddle], list[subSeqBegin]) <= list[subSeqEnd] <= max(list[subSeqMiddle], list[subSeqBegin]) 

 If chooseMiddle is true -> return subSeqMiddle as pivot's index.
 Else if chooseEnd is true -> return subSeqEnd as pivot's index.
 Else -> return subSeqBegin as pivot's index.
 */
template<class TClass, class TCompare>
int getPivotIndex(vector<TClass>& list, const TCompare& cmp, int subSeqBegin, int subSeqEnd)
{
    if (subSeqEnd - subSeqBegin < 2)
    {
        return subSeqBegin;
    }
    else
    {
        int subSeqMiddle = (subSeqBegin + subSeqEnd + 1) / 2;
        
        bool chooseMiddle = (cmp(list[subSeqMiddle], list[subSeqBegin]) >= 0) &
        					(cmp(list[subSeqMiddle], list[subSeqEnd]) <= 0);
        chooseMiddle |= (cmp(list[subSeqMiddle], list[subSeqEnd]) >= 0) &
        				(cmp(list[subSeqMiddle], list[subSeqBegin]) <= 0);
        
        bool chooseEnd = (cmp(list[subSeqEnd], list[subSeqBegin]) >= 0) &
        				 (cmp(list[subSeqEnd], list[subSeqMiddle]) <= 0);
        chooseEnd |= (cmp(list[subSeqEnd], list[subSeqMiddle]) >= 0) & 
        			 (cmp(list[subSeqEnd], list[subSeqBegin]) <= 0);
        
        if (chooseMiddle)
        {
            return subSeqMiddle;
        }
        else if (chooseEnd)
        {
            return subSeqEnd;
        }
        else
        {
            return subSeqBegin;
        }
    }
}

/*
 Local variables:
    >>> list        - sequence for sort
    >>> subSeqBegin - list's subsequence index of the begining 
    >>> subSeqEnd   - list's subsequence index of the ending
    >>> pivotIndex  - index of pivot in list
 */
template<class TClass, class TCompare> 
int partition(vector<TClass>& list, const TCompare& cmp, int subSeqBegin, int subSeqEnd)
{
    int i = -1, j = -1, pivotIndex = getPivotIndex(list, cmp, subSeqBegin, subSeqEnd);
    swap(list[subSeqEnd], list[pivotIndex]);
    
    for (i = subSeqBegin, j = subSeqBegin; j != subSeqEnd; j++)
    {
        // if j-th element is less than pivot
        if (cmp(list[j], list[subSeqEnd]) != 1)
        {
            swap(list[i], list[j]);
            i++;
        }
    }
    
    swap(list[i], list[subSeqEnd]); 
    return i; 
}

/*
 Local variables:
    >>> list        - sequence for sort
    >>> subSeqBegin - list's subsequence index of the begining 
    >>> subSeqEnd   - list's subsequence index of the ending
    >>> pivotIndex  - index of pivot in list
 */
template<class TClass, class TCompare> 
int getKthStatistics(vector<TClass>& list, const TCompare& cmp, int k)
{
    int subSeqBegin = 0, subSeqEnd = (int)list.size() - 1;
    int pivotIndex = partition(list, cmp, subSeqBegin, subSeqEnd);
    // if k == pivotIndex -> list[pivotIndex] is a KthStatistics
    if (pivotIndex == k)
    {
        return pivotIndex;
    }
    else
    {
        while (pivotIndex != k) 
        {
            // if pivotIndex < k -> KthStatistics is on the right side from pivotIndex
            if (pivotIndex < k)
            {
                subSeqBegin = pivotIndex + 1;
                pivotIndex = partition(list, cmp, subSeqBegin, subSeqEnd);
            }
            // if pivotIndex > k -> KthStatistics is on the left side from pivotIndex
            else
            {
                subSeqEnd = pivotIndex;
                pivotIndex = partition(list, cmp, subSeqBegin, subSeqEnd);
            }
        }
        return pivotIndex;
    }
}


//  1 - lhs > rhs
//  0 - lhs == rhs
// -1 - lhs < rhs
int compare(const int& lhs, const int& rhs)
{
    
    if (lhs == rhs)
    {
        return 0;
    }
    if (lhs > rhs)
    {
        return 1;
    }
    else
    {
        return -1;
    }
    
}




int main ()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n = 0, k = 0;
    assert(cin >> n >> k);
    assert(n >= 0 && k >= 0);
    vector<int> list(n);
    readElements(list);
    int answer = list[getKthStatistics(list, compare, k)];
    cout << answer << endl;
    list.clear();
    return 0;
}




inline void readElements(vector<int>& list)
{
    for (int i = 0; i < list.size(); i++)
    {
        assert(cin >> list[i]);
    }
    return;
}