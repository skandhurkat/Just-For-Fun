// skiplist_priority_queue.cpp
// Copyright 2013 Skand Hurkat

//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the
//  Free Software Foundation, either version 3 of the License, or (at your
//  option) any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <queue>
#include <mutex>
#include <cstdlib>

using namespace std;

template <typename T>
struct skiplist_node
{
  static size_t MAX_LEVEL;
  T data;
  size_t top_level;
  skiplist_node** succs;
  skiplist_node** preds;
  skiplist_node** succs_alt;
  skiplist_node** preds_alt;
  mutex lock_node;
  bool is_start_node;
  bool is_end_node;
  skiplist_node(): top_level(0), is_start_node(false), is_end_node(false)
  {
    if(MAX_LEVEL==0)
      throw std::bad_alloc();
    succs = new skiplist_node*[MAX_LEVEL];
    preds = new skiplist_node*[MAX_LEVEL];
    succs_alt = new skiplist_node*[MAX_LEVEL];
    preds_alt = new skiplist_node*[MAX_LEVEL];
  }
  ~skiplist_node()
  {
    delete[] succs;
    delete[] preds;
    delete[] succs_alt;
    delete[] preds_alt;
  }
};
template <typename T>
size_t skiplist_node<T>::MAX_LEVEL = 0;

template <typename T>
class skiplist_priority_queue
{
private:
  skiplist_node<T>* head;
  skiplist_node<T>* tail;

public:
  skiplist_priority_queue() = delete;
  skiplist_priority_queue(size_t MAX_LEVEL): head(nullptr), tail(nullptr)
  {
    skiplist_node<T>::MAX_LEVEL = MAX_LEVEL;
    head = new skiplist_node<T>();
    tail = new skiplist_node<T>();
    head->is_start_node = true;
    tail->is_end_node = true;
    head->top_level = MAX_LEVEL;
    tail->top_level = MAX_LEVEL;
    for(size_t i = 0; i < MAX_LEVEL; i++)
      head->preds[i] = nullptr;
    for(size_t i = 0; i < MAX_LEVEL; i++)
      head->succs[i] = tail;
    for(size_t i = 0; i < MAX_LEVEL; i++)
      tail->preds[i] = head;
    for(size_t i = 0; i < MAX_LEVEL; i++)
      tail->succs[i] = nullptr;
  }
  ~skiplist_priority_queue()
  {
    skiplist_node<T>* curr;
    while(head != nullptr)
    {
      curr = head->succs[0];
      delete head;
      head = curr;
    }
  }
  void push(const T& data)
  {
    //TODO: This algorithm does not implement locks yet.
    skiplist_node<T>* node = new skiplist_node<T>();
    node->data = data;
    node->top_level = (rand()%(skiplist_node<T>::MAX_LEVEL-1))+1;
    skiplist_node<T>* curr = head;
    skiplist_node<T>* next = nullptr;
    for(int curr_level = skiplist_node<T>::MAX_LEVEL-1;
            curr_level >= 0;
            curr_level--)
    {
      while(true)
      {
        next = curr->succs[curr_level];
        if(next->is_end_node or node->data > next->data)
        {
          if(curr_level < node->top_level)
          {
            node->preds[curr_level] = curr;
            node->succs[curr_level] = next;
          }
          break;
        }
        else
        {
          curr = next;
        }
      }
    }
    int curr_level = node->top_level;
    while(curr_level > 0)
    {
      curr_level--;
      curr->succs[curr_level] = node;
      curr->succs[curr_level]->preds[curr_level] = node;
    }
  }
  T& top()
  {
    return head->succs[0]->data;
  }
  void pop()
  {
    if(head->succs[0]->is_end_node)
      return;
//    head->lock_node.lock();
    skiplist_node<T>* temp = head->succs[0];
//    temp->lock_node.lock();
    for(size_t i = 0; i < temp->top_level; i++)
    {
//      temp->succs[i]->lock_node.lock();
      head->succs[i] = temp->succs[i];
      temp->succs[i]->preds[i] = head;
    }
//    head->lock_node.unlock();
//    for(size_t i = 0; i < temp->top_level; i++)
//    {
//      temp->succs[i]->lock_node.unlock();
//    }
//    temp->lock_node.unlock();
    delete temp;
  }
  bool empty()
  {
    return head->succs[0]->is_end_node;
  }
};

int main(int argc, char** argv)
{
  skiplist_priority_queue<int> pqueue(50);
  priority_queue<int> pqueue_bl;

  chrono::steady_clock::time_point start, end;
  chrono::duration<double> time_span_cumulative_sl =
    chrono::duration<double>::zero();
  chrono::duration<double> time_span_cumulative_pq =
    chrono::duration<double>::zero();

  int array[100000];
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 100000; j++)
    {
      array[j] = rand();
    }
    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++)
    {
      pqueue.push(array[j]);
    }
    end = chrono::steady_clock::now();
    time_span_cumulative_sl +=
      chrono::duration_cast<chrono::duration<double>>(end-start);

    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++)
    {
      pqueue_bl.push(array[j]);
    }
    end = chrono::steady_clock::now();
    time_span_cumulative_pq +=
      chrono::duration_cast<chrono::duration<double>>(end-start);

    for(int j = 0; j < 100000; j++)
    {
      if(pqueue.empty())
      {
        cerr << "Something went wrong. pqueue empty at " << j << endl;
        return EXIT_FAILURE;
      }
      if(pqueue_bl.empty())
      {
        cerr << "Something went wrong. pqueue_bl empty at " << j << endl;
        return EXIT_FAILURE;
      }
      if(pqueue.top() != pqueue_bl.top())
      {
        cerr << "skiplist implementation failed." << endl;
        cerr << pqueue.top() << " != " << pqueue_bl.top() << endl;
        return EXIT_FAILURE;
      }
      pqueue.pop();
      pqueue_bl.pop();
    }
  }

  cout << "My skiplist implementation took "
       << time_span_cumulative_sl.count()/10 << " seconds" << endl
       << "The STL implementation took "
       << time_span_cumulative_pq.count()/10 << " seconds" << endl;

  return 0;
}
