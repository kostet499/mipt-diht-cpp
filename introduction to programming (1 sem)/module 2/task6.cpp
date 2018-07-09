/*
 
 Дан массив целых чисел в диапазоне [0..10^9]. Размер массива кратен 10 и 
 ограничен сверху значением 2.5 * 10^7 элементов. Все значения массива являются 
 элементами псевдо-рандомной последовательности. Необходимо отсортировать элементы 
 массива за минимально время и вывести каждый десятый элемент отсортированной последовательности.
 Реализуйте сортировку, основанную на QuickSort.
 Минимальный набор оптимизаций, который необходимо реализовать:
 1. Оптимизация ввода/вывода
 2. Оптимизация выбора опорного элемента
 3. Оптимизация Partition
 4. Оптимизация рекурсии
 5. Оптимизация концевой рекурсии
 
 
 */



#include <assert.h>
#include <iostream>
#include <vector>



using namespace std;



inline void readElements(vector<int>& list);
inline void printElements(vector<int>& list);



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
	return (subSeqBegin + subSeqEnd + 1) / 2;
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
    if ((subSeqEnd - subSeqBegin) <= 1) return subSeqBegin;
    
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

template<class TClass, class TCompare> 
void QuickSort(vector<TClass>& list, const TCompare& cmp, int subSeqBegin, int subSeqEnd)
{
    if (subSeqBegin >= subSeqEnd) return;
    if (subSeqEnd - subSeqBegin == 1)
    {
        if (cmp(list[subSeqBegin], list[subSeqEnd]) == 1)
            swap(list[subSeqBegin], list[subSeqEnd]);
        return;
    }
    int part = partition(list, cmp, subSeqBegin, subSeqEnd);
    if (part > 0) QuickSort(list, cmp, subSeqBegin, part - 1);
    if (part + 1 < subSeqEnd) QuickSort(list, cmp, part + 1, subSeqEnd);
}

template<class TClass, class TCompare> 
void quickSort(vector<TClass>& list, const TCompare& cmp)
{
    return QuickSort(list, cmp, 0, list.size() - 1);
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
    vector<int> list;
    readElements(list);
    quickSort(list, compare); 
    printElements(list);
    list.clear();
    return 0;
}




inline void readElements(vector<int>& list)
{
    int tmp = 0;
    while(cin >> tmp)
    {
        if (cin.fail() || cin.eof())
        {
            break;
        }
        list.push_back(tmp);
    }
}

inline void printElements(vector<int>& list)
{
    for (int i = 9; i < list.size(); i += 10)
    {
        cout << list.at(i) << " ";
    }
}