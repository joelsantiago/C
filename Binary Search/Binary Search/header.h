//
//  header.h
//  Binary Search
//
//  Created by Joel on 2/5/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#ifndef Binary_Search_header_h
#define Binary_Search_header_h

using namespace std;

class SSortedList
{
public:
    
    //SSortedList(int n, T start_array);
    
    template <class T>
    void insertSort(T array[], int n);
    
    template <class T>
    void printArray(T array[], int n);
    
    template <class T>
    int BinarySearch(T array[], int n, int c);
};

/*template <class BinSearch>
SSortedList<BinSearch>::SSortedList(int n, BinSearch start_array)
{
    SSortedList::insertSort(start_array, n);
}*/

template <class BinSearch>
void SSortedList::insertSort(BinSearch array[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j > 0; j--)
        {
            if (array[j] < array[j - 1])
                swap(array[j], array[j - 1]);
        }
    }
}

template <class BinSearch>
void SSortedList::printArray(BinSearch array[], int n)
{
    cout << "Sorted List" << endl;
    for (int i = 0; i < n; i++)
        cout << "address: " << &array[i] << " value: " << array[i] << endl;
    cout << endl;
}

template <class BinSearch>
int SSortedList::BinarySearch(BinSearch array[], int n, int c)
{
    cout << endl << "c value: " << c << endl << endl;
    
    int first = 0;
    int last = n - 1;
    
    cout << "last value: " << last << endl << endl;
    
    while (first <= last)
    {
        cout << "while loop" << endl << endl;
        
        int mid = (first + last) / 2;
        cout << "mid value in loop: " << mid << endl << endl;
        
        if (c > array[mid])
        {
            first = mid + 1;
            cout << "if             address: " << &array[first] << "    first value: " << array[first] << endl << endl;
        }
        else if (c < array[mid])
        {
            last = mid - 1;
            cout << "else if        address: " << &array[last] << "     last value: " << array[last] << endl << endl;
        }
        else
            return mid;
            cout << "FOUND " << mid << endl;
            cout << "else           address: " << &array[mid] << "      mid value: " << array[mid] << endl << endl;
    }
    return -(first + 1);
    cout << "return" << endl << endl;
}

#endif
