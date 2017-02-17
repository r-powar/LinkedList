// CPSC 4600 / 5600 - Concurrent Linked List
// Course-grained Lock Synchronization

#include "tbb/tbb.h"
using namespace tbb;

#include "ConLinkList.h"
typedef tbb::mutex FreeLisMutexType;
FreeLisMutexType mymutex;


ConLinkList::ConLinkList() 
{
    Node *low = new Node;
    Node *high = new Node;
    high -> key = HIGH_SENTINEL;
    high -> data = 0;
    high->next = NULL;
    low -> key = LOW_SENTINEL;
    low -> data = 0;
    low->next = high;
    head = low;
}

bool ConLinkList::add(int key, int data)
{
    Node *prev, *curr;
    FreeLisMutexType::scoped_lock lock(mymutex);
    
    prev = head;
    curr = prev -> next;
    while(curr -> key < key){
        prev = curr;
        curr = curr->next;
    }
    if(key == curr->key){
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

bool ConLinkList::remove(int key)
{
    Node *prev, *curr;
    FreeLisMutexType::scoped_lock lock(mymutex);

    prev = head;
    curr = prev->next;
    while(curr->key < key){
        prev = curr;
        curr = curr -> next;
    }
    
    if(key == curr -> key){
        prev -> next = curr -> next;
        return true;
    }else{
        return false;
    }
}

bool ConLinkList::contains(int key)
{
    Node *prev, *curr;
    FreeLisMutexType::scoped_lock lock(mymutex);
    
    prev = head;
    curr = prev->next;
    while(curr->key < key){
        prev = curr;
        curr = curr->next;
    }
    
    return (key == curr->key);

}

// Unsafe to run concurrently, used for debugging only
int ConLinkList::count()
{
    Node *temp = head;
    int count = 0;
    while(temp != NULL){
        count++;
        temp = temp->next;
    }
    return count -2;
        
}
