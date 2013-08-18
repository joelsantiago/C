//
//  testbinarysearch.cpp
//  Binary Search
//
//  Created by Joel on 2/5/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <iostream>
#include "header.h"

using namespace std;

int main()
{
    SSortedList List;
    
    int choice;
    int size;
    
    while (choice != 3)
    {
        cout << "What type of array would you like to sort?" << endl;
        cout << "1.  Int" << endl;
        cout << "2.  String" << endl;
        cout << "3.  Exit" << endl << endl;
        cin >> choice;
    
        cout << endl;

        switch (choice)
        {
            case 1:
            {
                cout << "How many elements would you like in this array? ";
                cin >> size;
                cout << endl;
            
                cout << "Now please enter the array elements: " << endl << endl;
            
                int iArray[size];
            
                for (int i = 0; i < size; i++)
                {
                    cout << "* ";
                    cin >> iArray[i];
                    cout << endl;
                }
            
                List.insertSort(iArray, size);
            
                cout << endl;
            
                List.printArray(iArray, size);
            
                int c;
            
                cout << endl << "What value would you like to search for? ";
                cin >> c;
            
                List.BinarySearch(iArray, size, c);
            }
                break;
        
            case 2:
            {
                cout << "How many elements would you like in this array? ";
                cin >> size;
                cout << endl;
            
                cout << "Now please enter the array elements: " << endl << endl;
            
                char sArray[size][100];
            
                for (int i = 0; i < size; i++)
                {
                    cout << "* ";
                    cin >> sArray[i];
                    cout << endl;
                }
            
                List.insertSort(sArray, size);
            
                cout << endl;
            
                List.printArray(sArray, size);
                
                string c;
                
                cout << endl << "What value would you like to search for? ";
                cin >> c;
                
                //List.BinarySearch(sArray, size, c);
            }
                break;
        
            case 3:
                break;
            
            default:
                cout << "Please enter a valid selection from the available options!" << endl;
                break;
        }
    }
}





