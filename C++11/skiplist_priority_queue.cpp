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
    head = new skiplist_node();
    tail = new skiplist_node();
    head->is_start_node = true;
    tail->is_end_node = true;
    head->top_level = skiplist_node::MAX_LEVEL;
    tail->top_level = skiplist_node::MAX_LEVEL;
    for(node* pred = *head->preds; pred < *head->preds+head->top_level;
        pred++)
      pred = nullptr;
    for(node* succ = *head->succs; succ < *head->succs+head->top_level;
        succ++)
      succ = tail;
    for(node* pred = *tail->preds; pred < *tail->preds+tail->top_level;
        pred++)
      pred = head;
    for(node* succ = *tail->succs; succ < *tail->succs+tail->top_level;
        succ++)
      succ = nullptr;
  }
  ~skiplist_priority_queue()
  {
    node* curr;
    while(head != nullptr)
    {
      curr = head->succs[0];
      delete head;
      head = curr;
    }
  }
  void push(T& data)
  {
    skiplist_node* node = new skiplist_node();
    node->data = data;
    node->lock_node.lock();
    node->top_level = rand() % skiplist_node::MAX_LEVEL;
    int curr_level = node->top_level-1;
    while(curr_level >= 0)
    {
      node->preds[curr_level] = head;
      node->succs[curr_level] = head->succs[curr_level];
      curr_level--;
    }
    node* curr = head;
    curr->lock_node.lock();
    node* next = head->succs[0];
    next->lock_node.lock();
    while(not next->is_end_node and node->data < next->data)
    {
      curr->lock_node.unlock();
      curr = next;
      next = *curr->succs+curr->top_level-1;
      next->lock_node.lock();
      curr_level = curr->top_level-1;
      // TODO: Fix order of locks, and locking mechanism
      while(next >= *curr->succs)
      {
        if(not next->is_end_node and node->data < next->data)
          break;
        else
        {
          node->preds[curr_level] = curr;
          node->succs[curr_level] = next;
          curr_level--;
        }
        next--;
      }
      while(curr_level >= 0)
      {
        node->preds[curr_level] = curr;
        node->succs[curr_level] = curr->succs[curr_level];
        curr_level--;
      }
    }
  }
  T& top()
  {
    return head->succs[0]->data;
  }
  void pop()
  {
    head->lock_node.lock();
    node* temp = head->succs[0];
    temp->lock_node.lock();
    for(size_t i = 0; i < temp->top_level; i++)
    {
      temp->succs[i]->lock_node.lock();
      head->succs[i] = temp->succs[i];
      head->succs[i]->preds[i] = head;
    }
    head->lock_node.unlock();
    for(size_t i = 0; i < temp->top_level; i++)
    {
      temp->succs[i]->lock_node.unlock();
    }
    temp->lock_node.unlock();
    delete temp;
  }
};
