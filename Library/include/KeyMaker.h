// KeyMaker.h
// Contains keyMaker class for checking license key.
// Copyright 2008, 2012 Skand Hurkat

// This file is part of Library.

// Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Library.  If not, see <http://www.gnu.org/licenses/>.

#if !defined _INC_KEYMAKER
#define _INC_KEYMAKER

#include "msConGra.h"
#include <iostream>
#include <conio.h>
#include <ctype.h>
using namespace std;

/////////////////////////////////class keyMaker////////////////////////////////

class keyMaker
{
private:
	char name[31];
	char password[11];
	char card_no[5];
	int key[4];
	char inKey[4][5];
	void make_key();
	bool authenticate();
	void draw_screen();

public:
	keyMaker();
	~keyMaker()
	{ /*Empty*/ }
	bool interact();
};

/////////////////////////////end of class keyMaker/////////////////////////////
#endif
////////////////////////////end of file KeyMaker.h/////////////////////////////