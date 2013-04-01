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
  skiplist_node(): top_level = 0;
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
skiplist_node::MAX_LEVEL = 0;

template <typename T>
class skiplist_priority_queue
{
private:
  skiplist_node* head;
  skiplist_node* tail;

public:
  skiplist_priority_queue() = delete;
  skiplist_priority_queue(size_t MAX_LEVEL): head(nullptr), tail(nullptr)
  {
    skiplist_node::MAX_LEVEL = MAX_LEVEL;
  }
  void push(T& data)
  {
    skiplist_node* node = new skiplist_node();
    node->data = data;
    node->top_level = rand() % skiplist_node::MAX_LEVEL;
    node* curr = head;
    node* next = head;
    if(head == nullptr)
    {
      head = node;
      for(size_t lvl = 0; lvl < node->top_level; lvl++)
      {
        node->preds = nullptr;
        node->succs = nullptr;
      }
    }
    else
    {
      while(node->data < curr->data)
      {
        curr = next;
        next = curr->succs+curr->top_level;
        for(; next < curr->succs; next--)
        {
          if(node->data < next->data)
            break;
        }
      }
      // At this point, we know where we must insert the node. Question is:
      // how do we insert the node?
    }
  }
  T& top()
  {
  }
  void pop()
  {
  }
};
