//msConGra.h
//Contains the graphic routines.

#if !defined _INC_MSCONGRA
#define _INC_MSCONGRA

#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <strstream>
#include <windows.h>
#include <conio.h>
using namespace std;

///////////////////////////////////global data/////////////////////////////////

const char UP_KEY = 72;
const char DOWN_KEY = 80;
const char ENTER = 13;
const char ESC = 27;
const char SPACE = 32;
const char BACK_SPACE = 8;
const char POINTER = '\x10';
const char TOP_LEFT_CORNER = '\xC9';
const char BOTTOM_LEFT_CORNER = '\xC8';
const char TOP_RIGHT_CORNER = '\xBB';
const char BOTTOM_RIGHT_CORNER = '\xBC';
const char HORIZ_LINE = '\xCD';
const char VERT_LINE = '\xBA';


static const char* dayOfWeek[7] =
{
		"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
};
static const char* months[12] =
{
		"January", "Febuary", "March", "April", "May",
			"June", "July", "August", "September",
			"October", "November", "December"
};

///////////////////////////////////////////////////////////////////////////////

////////////////////////////////class graphics/////////////////////////////////
class graphics
{
private:
	HANDLE hConsole;
	COORD console_size;
	
public:
	graphics();
	graphics(COORD console_size);
	graphics(int MAX_X, int MAX_Y);
	~graphics()
	{ /* Empty */ }
	char* asc_date(tm* lcTime);
	void clear_line();
	void clear_screen();
	int days_elapsed(tm* time1, tm*time2);
	void draw_box(int xCo, int yCo, int width, int height);
	int get_max_x() const;
	int get_max_y() const;
	tm* get_time() const;
	bool is_leap_year(const tm timeStruct);
	bool is_leap_year(const int year);
	int months_elapsed(const tm* time1, const tm* time2);
	void set_cursor_pos(int xCo, int yCo);
	void show_time(const tm* lcTime);
	void show_time(const tm* lcTime, int line_no);
	void wait(int milliseconds);
};

/////////////////////////////end of class graphics/////////////////////////////

///////////////////////////////////////////////////////////////////////////////
static graphics console;
///////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////end of file msConGra.h/////////////////////////////