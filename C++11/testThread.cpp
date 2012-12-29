#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex m;

void t_call(int* sharedCounter)
{
  m.lock();
  (*sharedCounter)++;
  m.unlock();
}

int main(int argc, char** argv)
{
  vector<thread> t;
  int sharedCounter = 0;
  for(int i = 0; i < 8; i++)
    t.push_back(thread(t_call, &sharedCounter));
  m.lock();
  sharedCounter++;
  m.unlock();
  auto titr = t.begin();
  while(titr != t.end())
  {
    titr->join();
    titr++;
  }
  cout << sharedCounter << endl;
  return 0;
}
