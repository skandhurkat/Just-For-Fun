// MakeKey.cpp
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

#include <conio.h>
#include "KeyMaker.h"

int main()
{   
	keyMaker theUserInterface;
	theUserInterface.interact();
	getch();
	return 0;
}
