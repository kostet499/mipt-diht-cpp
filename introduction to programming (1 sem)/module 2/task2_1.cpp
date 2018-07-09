/*
 
    В супермаркете решили оптимизировать показ рекламы. Известно расписание 
    прихода и ухода покупателей (два целых числа). Каждому покупателю необходимо 
    показать минимум 2 рекламы. Рекламу можно транслировать только в целочисленные 
    моменты времени. Покупатель может видеть рекламу от момента прихода до момента 
    ухода из магазина. В каждый момент времени может показываться только одна 
    реклама. Считается, что реклама показывается мгновенно. Если реклама 
    показывается в момент ухода или прихода, то считается, что посетитель успел её 
    посмотреть. Требуется определить минимальное число показов рекламы.
 
 */



#include <assert.h>
#include <iostream>
#include <vector>



using namespace std;



inline void readPairs(vector<pair<int, int> >& list);



template<class TClass, class TCompare> 
void siftDown(vector<TClass>& list, int index, int heapSize, const TCompare& cmp)
{
    int currentNode = index;
    int leftChild = index * 2 + 1;
    int rightChild = index * 2 + 2;
    if (leftChild < heapSize && (cmp(list[leftChild], list[currentNode]) == 1))
    {
        currentNode = leftChild;
    }
    if (rightChild < heapSize && (cmp(list[rightChild], list[currentNode]) == 1))
    {
        currentNode = rightChild;
    }
    if (currentNode != index)
    {
        swap(list[index], list[currentNode]);
        siftDown(list, currentNode, heapSize, cmp);
    }
    else
    {
        return;
    }
}

template<class TClass, class TCompare> 
void buildHeap(vector<TClass>& list, const TCompare& cmp)
{
    for(int i = (list.size() / 2) - 1; i >= 0; i--)
    {
        siftDown(list, i, list.size(), cmp);
    }
    return;
}

template<class TClass, class TCompare> 
void heapSort(vector<TClass>& list, const TCompare& cmp)
{
    buildHeap(list, cmp);
    for(int i = list.size() - 1; i >= 0; i--)
    {
        swap(list[0], list[i]);
        siftDown(list, 0, i, cmp);
    }
    return;
}



//  1 - lhs > rhs
//  0 - lhs == rhs
// -1 - lhs < rhs
int compare(const pair<int, int>& lhs, const pair<int, int>& rhs)
{
    if (lhs.second == rhs.second)
    {
        if (lhs.first == rhs.first)
        {
            return 0;
        }
        if (lhs.first > rhs.first)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    if (lhs.second > rhs.second)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}


int getMinAmountOfAdds(vector<pair<int, int> >& list)
{
    // if list contains less than 2 items
    if (list.size() < 2)
    {
        return 2;
    }
    // sort list by departure time in priority,
    // than in priority of arrival
    heapSort(list, compare);
    
    int answer = 2;
    // penultimate advertising is shown in a moment before current customer exits
    int penultAdd = list[0].second - 1;         
    // last advertising is shown when current customer exits
    int lastAdd = list[0].second;
    for (int i = 1; i < list.size(); i++)
    {
        // if current can not see the last add
        if (list[i].first > lastAdd)
        {
            answer += 2;
            penultAdd = list[i].second - 1;
            lastAdd = list[i].second;
        }
        // if current customer can see the penultimate and last adds
        else if (list[i].first <= penultAdd)
        {
            continue;
        }
        // if current can see only the last add
        else
        {
            answer++;
            penultAdd = lastAdd;
            lastAdd = list[i].second;
        }
    }
    return answer;
}



int main ()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n = 0;
    assert(cin >> n);
    assert(n >= 0);
    vector<pair<int, int> > list(n);
    readPairs(list);
    cout << getMinAmountOfAdds(list) << endl; 
    list.clear();
    return 0;
}



inline void readPairs(vector<pair<int, int> >& list)
{
    for (int i = 0; i < list.size(); i++)
    {
        assert(cin >> list[i].first >> list[i].second);
    }
    return;
}