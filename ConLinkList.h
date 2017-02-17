// CPSC 4600 / 5600 - Concurrent Linked List

#include <iostream>
#include <climits>
using namespace std;

#include "tbb/tbb.h"
using namespace tbb;
typedef tbb::mutex MyMutex;

class ConLinkList {

private:

  const int LOW_SENTINEL = -1;
  const int HIGH_SENTINEL = INT_MAX;

#ifdef LAZY

  // TODO: Complete node definition and private variables and methods
  // for lazy synchronization
  struct Node {
      Node *next;
      int key;
      int data;
      bool marked;
      MyMutex lock;
  };
    
bool validate(Node *prev, Node *curr);
    
#else

  // TODO: Complete node definition and private variables and methods
  // for coarse-grained lock synchronization

  struct Node {
      Node *next;
      int key;
      int data;
  };
    

#endif

  // TODO: Place private variables and methods common to both techniques here
    Node *head;



  // NOTE: You are not allowed to modify the public section.
public:

  ConLinkList();
  bool add(int key, int data);
  bool remove(int key);
  bool contains(int key);
  int count();

};
