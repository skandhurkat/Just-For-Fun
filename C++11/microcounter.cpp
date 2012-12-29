// microcounter.cpp
// (c) Skand Hurkat, 2012

//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
