#include <assert.h>
#include <iostream>
#include <vector>

using namespace std;




template<class TClass, class TCompare>
inline int getPivotIndex(TClass* list, const TCompare& cmp, \
    int subSeqBegin, int subSeqEnd)
{
    return (subSeqBegin + subSeqEnd + 1) / 2;
}

template<class TClass, class TCompare>
int partition(TClass* list, const TCompare& cmp, \
    int subSeqBegin, int subSeqEnd)
{
    if ((subSeqEnd - subSeqBegin) <= 1)
        return subSeqBegin;
    int i = -1, j = -1;
    int pivotIndex = getPivotIndex(list, cmp, subSeqBegin, subSeqEnd);
    swap(list[subSeqEnd], list[pivotIndex]);
    for (i = subSeqBegin, j = subSeqBegin; j != subSeqEnd; j++)
    {
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
void quickSort(TClass* list, const TCompare& cmp, \
int subSeqBegin, int subSeqEnd)
{
    if (subSeqBegin >= subSeqEnd)
        return;
    if (subSeqEnd - subSeqBegin == 1)
    {
        if (cmp(list[subSeqBegin], list[subSeqEnd]) == 1)
            swap(list[subSeqBegin], list[subSeqEnd]);
        return;
    }
    int part = partition(list, cmp, subSeqBegin, subSeqEnd);
    if (part > 0)
        quickSort(list, cmp, subSeqBegin, part - 1);
    if (part + 1 < subSeqEnd)
        quickSort(list, cmp, part + 1, subSeqEnd);
}

template<class TClass, class TCompare>
void QuickSort(TClass* list, int size, const TCompare& cmp)
{
    return quickSort(list, cmp, 0, size);
}




int GetMaxHeight(int amount, pair<int, int>* sportsmen)
{
    // Firstly, sort all athletes by weight,
    // in the way that athlete with less weight will be
    // in the front of more one,
    // if ones' weights are equal choise less powerful
    QuickSort(sportsmen, amount - 1, \
    [&](pair<int, int> athlete0, pair<int, int> athlete1){
        if (athlete0.first == athlete1.first)
            return athlete0.second > athlete1.second;
        else
            return athlete0.first > athlete1.first;
    });
    // Then iterate sequence with greedy algorithm,
    // so the i-th sportsman will be in the tower of human
    // if he/she can lift others, who are already in the tower
    int totalweight = 0, totalheight = 0;
    for (int current = 0; current < amount; current++)
    {
        if (sportsmen[current].first >= totalweight)
        {
            totalweight += sportsmen[current].second;
            totalheight++;
        }
    }
    return totalheight;
}


int main()
{
    // athlete.first  -> athlete's strength
    // athlete.second -> athlete's weight
    pair<int, int> athlete = make_pair(0, 0);
    vector<pair<int, int> > sportsmen;
    while (cin >> athlete.second >> athlete.first)
    {
        if (cin.fail() || cin.eof())
            break;
        sportsmen.push_back( make_pair(athlete.first, athlete.second) );
    }
    assert(!sportsmen.empty());
    cout << GetMaxHeight(sportsmen.size(), &sportsmen[0]) << endl;
    sportsmen.clear();
    return 0;
}