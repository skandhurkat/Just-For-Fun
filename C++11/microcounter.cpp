#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex m;

void t_work(int& sharedCounter)
{
	m.lock();
	sharedCounter++;
	m.unlock();
}

int main(int argc, char** argv)
{
	int sharedCounter = 0;
	vector<thread> t;
	for(int i = 0; i < 8; i++)
		t.push_back(thread(t_work, ref(sharedCounter)));
	m.lock();
	sharedCounter++;
	m.unlock();
	for(thread& th : t)
		th.join();
	cout << sharedCounter << endl;
}