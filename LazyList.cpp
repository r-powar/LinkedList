// CPSC 4600 / 5600 - Concurrent Linked List
// Lazy Synchronization

#include "tbb/tbb.h"
using namespace tbb;

#include "ConLinkList.h"

ConLinkList::ConLinkList() 
{
    Node *low = new Node;
    Node *high = new Node;
    high -> key = HIGH_SENTINEL;
    high->data = 0;
    high->next = NULL;
    high -> marked = false;
    low -> key = LOW_SENTINEL;
    low -> data = 0;
    low -> next = high;
    low -> marked = false;
    head = low;
    
}

bool ConLinkList::validate(Node *prev, Node* curr){
    return (!prev->marked && !curr->marked && prev->next == curr);
}

bool ConLinkList::add(int key, int data)
{
    while (true) {
        Node *prev = head;
        Node *curr = head -> next;
        while(curr -> key < key){
            prev = curr;
            curr = curr -> next;
        }
        MyMutex::scoped_lock lock(prev->lock);
        MyMutex::scoped_lock secondlock(curr->lock);
        
        if(validate(prev, curr)){
            if(curr->key == key){
                return false;
            }else{
                Node *node = new Node;
                node->key = key;
                node->data = data;
                node->next = curr;
                prev->next = node;
                return true;
            }
        }
        
        
    }
    
}

bool ConLinkList::remove(int key)
{
    while (true) {
        Node *prev = head;
        Node *curr = head -> next;
        while (curr->key < key) {
            prev = curr;
            curr = curr -> next;
        }
        MyMutex::scoped_lock lock(prev->lock);
        MyMutex::scoped_lock secondlock(curr->lock);
        if(validate(prev, curr)){
            if(curr->key != key){
                return false;
            }else{
                curr->marked = true;
                prev->next = curr->next;
                return true;
            }
        }
    }
}

bool ConLinkList::contains(int key)
{
    Node *curr = head;
    while (curr->key < key) {
        curr = curr -> next;
    }
    return curr->key == key && !curr->marked;
}

// Unsafe to run concurrently, used for debugging only
int ConLinkList::count()
{
    Node *temp = head;
    int count = 0;
    while(temp!=NULL){
        count++;
        temp = temp->next;
    }
    return count - 2;
}
