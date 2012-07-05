//KeyMaker.cpp
//Contains function definitions for KeyMaker.h

#include "KeyMaker.h"

//////////////////////////methods for class keymaker///////////////////////////
//-----------------------------------------------------------------------------
keyMaker::keyMaker()
{
	name[0] = '\0';
	password[0] = '\0';
	card_no[0] = '\0';
	for(int i = 0; i < 4; i++)
	{
		key[i] = -1;
		inKey[0][i] = '\0';
	}
}
//-----------------------------------------------------------------------------
bool keyMaker::authenticate()
{
	int i;
	make_key();
	for(i = 0; i < 4; i++)
	{
		if(strlen(inKey[i]) != 4)
			return false;
		if(key[i] != atoi(inKey[i]))
			return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
void keyMaker::draw_screen()
{
	const int MID_X = console.get_max_x() / 2;
	const int MID_Y = console.get_max_y() / 2;
	console.clear_screen();
	console.draw_box(MID_X - 25, MID_Y - 7, 50, 16);
	console.set_cursor_pos(MID_X - 20, MID_Y - 5);
	cout << "NAME:";
	console.set_cursor_pos(MID_X - 20, MID_Y - 3);
	cout << "PASSWORD:";
	console.set_cursor_pos(MID_X - 20, MID_Y - 1);
	cout << "LAST 4 DIGITS OF";
	console.set_cursor_pos(MID_X - 20, MID_Y);
	cout << "CREDIT CARD:";
	console.set_cursor_pos(MID_X - 20, MID_Y + 2);
	cout << "SERIAL NO FROM E-MAIL:";
	console.draw_box(MID_X - 20, MID_Y + 3, 5, 2);
	console.draw_box(MID_X - 9, MID_Y + 3, 5, 2);
	console.draw_box(MID_X + 2, MID_Y + 3, 5, 2);
	console.draw_box(MID_X + 13, MID_Y + 3, 5, 2);
	console.set_cursor_pos(MID_X - 20, MID_Y + 6);
	cout << "Press ENTER to go to the next field";
	console.set_cursor_pos(MID_X - 20, MID_Y + 7);
	cout << "Press ESCAPE to reset all fields";
}
//-----------------------------------------------------------------------------
bool keyMaker::interact()
{
	bool redraw;
	int i, j;
	char ch;
	const int MID_X = console.get_max_x() / 2;
	const int MID_Y = console.get_max_y() / 2;
	do
	{
		redraw = false;
		draw_screen();
		console.set_cursor_pos(MID_X - 13, MID_Y - 5);
		i = 0;
		do
		{
			if(redraw) break;
			ch = getch();
			if(ch >= 'A' && ch <= 'Z' && i < 31)
			{
				name[i++] = ch;
				cout << ch;
			}
			else if(ch >= 'a' && ch <= 'z' && i < 31)
			{
				name[i++] = toupper(ch);
				cout << static_cast<char>(toupper(ch));
			}
			else if(ch == SPACE)
			{
				name[i++] = ch;
				cout << ch;
			}
			else if(i >= 31)
				cout << '\a';
			else if(ch == '\b')
				if(i > 0)
				{
					cout << "\b \b";
					i--;
				}
				else
					cout << '\a';
			else if(ch == ENTER)
				if(i)
				{
					name[i] = '\0';
					break;
				}
				else
					cout << '\a';
			else if(ch == ESC)
			{
				redraw = true;
				break;
			}
			else cout << '\a';
		}while(true);
		console.set_cursor_pos(MID_X - 10, MID_Y - 3);
		i = 0;
		do
		{
			if(redraw) break;
			ch = getch();
			if(ch >= 'A' && ch <= 'Z' && i < 11)
			{
				password[i++] = ch;
				cout << '*';
			}
			else if(ch >= 'a' && ch <= 'z' && i < 11)
			{
				password[i++] = toupper(ch);
				cout << '*';
			}
			else if(i >= 11)
				cout << '\a';
			else if(ch == '\b')
				if(i > 0)
				{
					cout << "\b \b";
					i--;
				}
				else
					cout << '\a';
			else if(ch == ENTER)
				if(i)
				{
					password[i] = '\0';
					break;
				}
				else
					cout << '\a';
			else if(ch == ESC)
			{
				redraw = true;
				break;
			}
			else cout << '\a';
		}while(true);
		console.set_cursor_pos(MID_X - 2, MID_Y);
		i = 0;
		do
		{
			if(redraw) break;
			ch = getch();
			if(ch >= '0' && ch <= '9' && i < 5)
			{
				card_no[i++] = ch;
				cout << ch;
				if(i >= 4)
					cout << '\a';
			}
			else if(ch == '\b')
				if(i > 0)
				{
					cout << "\b \b";
					i--;
				}
				else
					cout << '\a';
			else if(ch == ENTER)
				if(i == 4)
				{
					card_no[i] = '\0';
					break;
				}
				else
					cout << '\a';
			else if(ch == ESC)
			{
				redraw = true;
				break;
			}
			else cout << '\a';
		}while(true);
		i = j =0;
		do
		{
			if(redraw) break;
			if(i != 4 && j != 4)
				console.set_cursor_pos(MID_X-19+i*11+j, MID_Y+4);
			ch = getch();
			if(ch >= '0' && ch <= '9' && i < 4)
			{
				cout << ch;
				inKey[i][j++] = ch;
				if(j == 4)
				{
					inKey[i++][j] = '\0';
					j = 0;
				}
			}
			else if(ch == '\b')
			{
				if(j == 0 && i == 0)
					cout << '\a';
				else if(j == 0)
				{
					j = 3;
					i--;
					console.set_cursor_pos(MID_X-19+i*11+j, MID_Y+4);
					cout << " \b";
				}
				else
				{
					j--;
					cout << "\b \b";
				}
			}
			else if(ch == ESC)
			{
				redraw = true;
				break;
			}
			else if(ch == ENTER)
				if(i == 4 && j == 0)
					break;
				else cout << '\a';
		}while(true);
	}while(redraw);
	return authenticate();

	return true;
}
//-----------------------------------------------------------------------------
void keyMaker::make_key()
{
	int len1, len2, len;
	int rand_no;
	int i;
	len1 = strlen(name);
	len2 = strlen(password);
	len = len1 + len2;
	srand(len);
	for(i = 0; i < len1; i++)
		rand_no = rand();
	srand(rand_no);
	for(i = 0; i < len2; i++)
		rand_no = rand();
	srand(static_cast<int>(atoi(card_no) % rand_no));
	rand_no = rand() % len1;
	srand(static_cast<int>(name[rand_no]));
	for(i = 0; i < len2; i++)
		rand_no = rand() % len2;
	srand(static_cast<int>(password[rand_no]));
	for(i = 0; i < (atoi(card_no) % len); i++)
		rand_no = rand();
	srand(rand_no);
	for(i = 0; i < 4; i++)
		key[i] = (rand() % 10000);
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////end of file KeyMaker.cpp////////////////////////////