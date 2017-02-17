// CPSC 4600 / 5600 - Concurrent Linked List
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "tbb/tbb.h"
using namespace tbb;

#include "ConLinkList.h"

const int NUM_THREADS = 8;

void removeVal(int *src, int start, int end, ConLinkList clist){
    for(int i = start; i < end; i++){
        int x = src[i];
        clist.remove(x);
    }
}

void checkVal(int *src, int start, int end, ConLinkList clist, int count){
    for(int i = start; i < end; i++){
        int x = src[i];
        bool check = clist.contains(x);
        if(check){
            count++;
        }
    }
}

void addVal(int *src, int start, int end, ConLinkList clist){
    for(int i = start; i < end; i++){
        int x = src[i];
        clist.add(x, x*2);
    }
}



int
main(int argc, char *argv[])
{
  if (argc != 2) {
    cerr << "Invalid command line - usage: <input file>" << endl;
    exit(-1);
  }

  // Extract parameters
  ifstream ifile(argv[1]);

  // Set the number of threads
  task_scheduler_init init(NUM_THREADS);

  // Declare the list
  ConLinkList clist;

  // Read in the file
  int size;
  ifile >> size;
  int *data = new int[size];
  for (int i = 0; i < size; i++) {
    ifile >> data[i];
  }
   
  tbb::atomic<int> count = 0;
  tick_count start_time, end_time;
  double seq_time;

  // Test 1: Parallel adds
  start_time = tick_count::now();

  // TODO: Add test code here
    parallel_for(blocked_range<int>(0,size),
                 [&](const blocked_range<int> &r){
                     for(int i = r.begin(); i < r.end(); i++){
                         int x = data[i];
                         clist.add(x, x*2);
                     }
                 });
  
  end_time = tick_count::now();
  seq_time = (end_time - start_time).seconds();
  cout << "Test 1 results: " << endl;
  cout << "Time: " << seq_time << endl;
  cout << "Size (correct): " << size << endl;
  cout << "Size (actual):  " << clist.count() << endl;

  // Test 2: Parallel contains
  start_time = tick_count::now();

  // TODO: Add test code here
  parallel_for(blocked_range<int>(0, size),
               [&](const blocked_range<int> &r){
                   for(int i = r.begin(); i < r.end(); i++){
                       int x = data[i];
                       bool check = clist.contains(x);
                       if(check){
                           count++;
                       }
                   }
               });
  end_time = tick_count::now();
  seq_time = (end_time - start_time).seconds();
  cout << "Test 2 results: " << endl;
  cout << "Time: " << seq_time << endl;
  cout << "Count (correct): " << size << endl;
  cout << "Count (actual):  " << count << endl;

  // Test 3: Parallel removes (1st half of data) / parallel contains (2nd half of data)
  start_time = tick_count::now();
    
 
  // TODO: Add test code here
    parallel_invoke([&]{removeVal(data, 0, size/2, clist);},
                    [&]{checkVal(data, size/2, size, clist, count);});
    

  
  end_time = tick_count::now();
  seq_time = (end_time - start_time).seconds();
  cout << "Test 3 results: " << endl;
  cout << "Time: " << seq_time << endl;
  cout << "Size  (correct): " << size / 2 << endl;
  cout << "Size  (actual):  " << clist.count() << endl;
  cout << "Count (correct): " << size / 2 << endl;
  cout << "Count (actual):  " << count << endl;

  // Test 4: Parallel adds (1st half of data) / parallel removes (2nd half of data)
  start_time = tick_count::now();

  // TODO: Add test code here
    parallel_invoke([&]{addVal(data, 0, size/2, clist);},
                    [&]{removeVal(data, size/2, size, clist);});
    
  end_time = tick_count::now();
  seq_time = (end_time - start_time).seconds();
  cout << "Test 4 results: " << endl;
  cout << "Time: " << seq_time << endl;
  cout << "Size (correct): " << size / 2 << endl;
  cout << "Size (actual):  " << clist.count() << endl;

  return 0;
}
