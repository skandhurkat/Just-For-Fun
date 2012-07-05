//KeyMaker.h
//Contains keyMaker class for checking license key.

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