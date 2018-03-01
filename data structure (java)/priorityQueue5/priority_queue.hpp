/****************************************************************************
*****************************************************************************
*****************************************************************************
**********                                                         **********
**********           Heap (Priority Queue Representation)          **********
**********                                                         **********
**********                  C++ Implementation                     **********
**********                                                         **********
**********                                                         **********
*****************************************************************************
*****************************************************************************
*****************************************************************************/

#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

class PriorityQueue {
public:
  // Can be seen externally as PriorityQueue::DataType
  typedef int DataType;

private:
  friend class PriorityQueueTest;

  // Sequential representation of the priority queue.
  DataType* heap_;

  // Total number of elements that the priority queue can store.
  unsigned int capacity_;
  
  // Current number of elements in the priority queue.
  unsigned int size_;
  
  // Override copy constructor and assignment operator in private so we can't
  // use them.
  PriorityQueue(const PriorityQueue& other) {}
  PriorityQueue& operator=(const PriorityQueue& other) {}

  
public:
  // Constructor initializes heap_ to an array of (capacity_ + 1) size, so 
  // that there are at most capacity_ elements in the priority queue. 
  PriorityQueue(unsigned int capacity);
  // Destructor of the class PriorityQueue. It deallocates the memory space 
  // allocated for the priority queue. 
  ~PriorityQueue();
  
  // Returns the number of elements in the priority queue.
  unsigned int size() const;
  // Returns true if the priority queue is empty, and false otherwise.
  bool empty() const;
  // Returns true if the priority queue is full, and false otherwise.
  bool full() const;
  // Prints the contents of the priority queue.
  void print() const;
  // Returns the max element of the priority queue, but does not remove it.
  DataType max() const;

  
  // Inserts value into the priority queue. Returns true if successful, and 
  // false otherwise. Assume no duplicate entries will be entered.
  bool enqueue(DataType val);
  // Removes the top element with the maximum value (priority) and rearranges 
  // the resulting heap. Returns true if successful, and false otherwise.
  bool dequeue();  
};
#endif 
