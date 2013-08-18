//
//  list.cpp
//  Linked List
//
//  Created by Joel on 2/12/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include "iostream"
using namespace std;

class Node
{
public:
    int val;
    Node* next;
    
    void searchNode(int n);
    void display();
    void addtoend(int addVal);
    void insert(Node** head, int n, Node* newPos);
    int length();
    int deleteNode(int n);
};
Node* head = NULL;
Node* mid = NULL;
Node* tail = NULL;




Node* searchNode(int n)
{
    if (head == NULL)
        return head;
    Node* cur = head;
    Node* prev = head;
    while (cur)
    {
        if (cur->val == n)
            return cur;
        prev = cur;
        cur = cur->next;
    }
    return cur;
}


void Node::display()
{
    Node* list = head;
    while (list)
    {
        cout << list->val << " ";
        list = list->next;
    }
    cout << endl;
}


void Node::addtoend(int addVal)
{
    Node* newNode = new Node;
    newNode->val = addVal;
    newNode->next = NULL;
    
    if (head == NULL)
    {
        head = newNode;
        return;
    }
    
    Node* cur = head;
    while (cur)
    {
        if (cur->next == NULL)
        {
            cur->next = newNode;
            return;
        }
        cur = cur->next;
    }
}


void Node::insert(Node** head, int n, Node* newPos)
{
    newPos = new Node;
    newPos->val = n;
    newPos->next = *head;
    *head = newPos;
}


int length()
{
    int count = 0;
    Node* cur = head;
    
    while (cur != NULL)
    {
        count++;
        cur = cur->next;
    }
    return count;
}


int deleteNode(int n)
{
    Node* ptr = searchNode(n);
    if (ptr == NULL)
        cout << "There is no node in the list that contains the value " << n << endl << endl;
    if (ptr == head)
    {
        head = ptr->next;
        return head->val;
    }
    Node* cur = head;
    Node* prev = head;
    
    while (cur)
    {
        if (cur == ptr)
        {
            prev->next = cur->next;
            return head->val;
        }
        prev = cur;
        cur = cur->next;
    }
    return head->val;
}


int main()
{
    Node a,b,c;
    
    a.addtoend(3);
    cout << "List contents of A: \t\t\t";
    a.display();
    
    b.addtoend(8);
    cout << "List contents of A & B: \t\t";
    b.display();
    
    c.addtoend(10);
    cout << "List contents of A, B, & C: \t";
    c.display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
    
    cout << "Value in head: " << deleteNode(3)<< endl << endl;
    
    a.display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
    
    a.insert(&head, 4, head);
    
    a.display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
    
    /*
    addtoend(3);
    cout << "List contents: ";
    display();
    
    addtoend(8);
    cout << "List contents: ";
    display();
    
    addtoend(10);
    cout << "List contents: ";
    display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
    
    cout << "Value in head: " << deleteNode(8)<< endl << endl;
    
    display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
    
    insert(&head, 4, head);
    
    display();
    
    cout << endl << "Length of list: " << length() << endl << endl;
     */
}


