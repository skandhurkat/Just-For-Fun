//msConGra.cpp
//Contains function definitions for msConGra.cpp

#include "msConGra.h"

//////////////////////////methods for class graphics///////////////////////////
//-----------------------------------------------------------------------------
graphics::graphics()
{
	console_size.X = 80;
	console_size.Y = 25;
	hConsole = CreateFile("CONOUT$", GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
	SetConsoleScreenBufferSize(hConsole, console_size);
	SetConsoleTextAttribute( hConsole, (WORD)((0 << 4) | 15) );

	clear_screen();
}
//-----------------------------------------------------------------------------
graphics::graphics(COORD c_size)
{
	console_size = c_size;
	hConsole = CreateFile("CONOUT$", GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
	SetConsoleScreenBufferSize(hConsole, console_size);
	SetConsoleTextAttribute( hConsole, (WORD)((0 << 4) | 15) );

	clear_screen();
}
//-----------------------------------------------------------------------------
graphics::graphics(int MAX_X, int MAX_Y)
{
	console_size.X = MAX_X;
	console_size.Y = MAX_Y;
	hConsole = CreateFile("CONOUT$", GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
	SetConsoleScreenBufferSize(hConsole, console_size);
	SetConsoleTextAttribute( hConsole, (WORD)((0 << 4) | 15) );

	clear_screen();
}
//-----------------------------------------------------------------------------
char* graphics::asc_date(tm* lcTime)
{
	const int SIZE = 30;
	static char memBuff[SIZE];
	ostrstream oMemBuff(memBuff, SIZE);
	oMemBuff << dayOfWeek[lcTime->tm_wday] << ", " << lcTime->tm_mday << ' '
		<< months[lcTime->tm_mon] << ' ' << lcTime->tm_year + 1900 << '.';
	oMemBuff << '\0';
	return memBuff;
}
//-----------------------------------------------------------------------------
void graphics::clear_line()
{
	for(int i = 0; i < console_size.X; i++)
		cout << ' ';
}
//-----------------------------------------------------------------------------
void graphics::clear_screen()
{
	set_cursor_pos(1, console_size.Y);
	for (int i = 0; i < console_size.Y; i++)
		cout << '\n';
	set_cursor_pos(1, 1);
}
//-----------------------------------------------------------------------------
int graphics::days_elapsed(tm* time1, tm* time2)
{
	int day_diff = 0;
	int current_year;
	if(time1->tm_year == time2->tm_year)
		day_diff = abs(time2->tm_yday - time1->tm_yday);
	else if(time1->tm_year < time2->tm_year)
	{
		current_year = time1->tm_year + 1900;
		while(current_year < (time2->tm_year+1900))
		{
			if(is_leap_year(current_year))
				day_diff += 366;
			else day_diff += 365;
			current_year++;
		}
		day_diff += time2->tm_yday - time1->tm_yday;
	}
	else
	{
		current_year = time2->tm_year + 1900;
		while(current_year < (time1->tm_year+1900))
		{
			if(is_leap_year(current_year))
				day_diff += 366;
			else day_diff += 365;
			current_year++;
		}
		day_diff += time1->tm_yday - time2->tm_yday;
	}
	return day_diff;
}
//-----------------------------------------------------------------------------
void graphics::draw_box(int xCo, int yCo, int width, int height)
{
	int i;
	set_cursor_pos(xCo, yCo);
	cout << TOP_LEFT_CORNER;
	for(i = 1; i < width; i++)
		cout << HORIZ_LINE;
	cout << TOP_RIGHT_CORNER;
	for(i = 1; i < height; i++)
	{
		set_cursor_pos(xCo, yCo + i);
		cout << VERT_LINE;
		set_cursor_pos(xCo + width, yCo + i);
		cout << VERT_LINE;
	}
	set_cursor_pos(xCo, yCo + height);
	cout << BOTTOM_LEFT_CORNER;
	for(i = 1; i < width; i++)
		cout << HORIZ_LINE;
	cout << BOTTOM_RIGHT_CORNER;
}
//-----------------------------------------------------------------------------
int graphics::get_max_x() const
{
	return console_size.X;
}
//-----------------------------------------------------------------------------
int graphics::get_max_y() const
{
	return console_size.Y;
}
//-----------------------------------------------------------------------------
tm* graphics::get_time() const
{
	time_t sysTime;
	tm* lcTime;
	sysTime = time(NULL);
	lcTime = localtime(&sysTime);
	return lcTime;
}
//-----------------------------------------------------------------------------
bool graphics::is_leap_year(tm timeStruct)
{
	return is_leap_year(timeStruct.tm_year + 1900);
}
//-----------------------------------------------------------------------------
bool graphics::is_leap_year(int year)
{
	if(year % 4)
		if(year % 100)
			if(year % 400)
				return true;
			else return false;
		else return true;
	else return false;
}
//-----------------------------------------------------------------------------
int graphics::months_elapsed(const tm* time1, const tm* time2)
{
	int month_diff = 0;
	int current_year;
	if(time1->tm_year == time2->tm_year)
		month_diff = abs(time2->tm_mon - time2->tm_mon);
	else if(time1->tm_year < time2->tm_year)
	{
		current_year = time1->tm_year;
		while(current_year < time2->tm_year)
		{
			 month_diff += 12;
			 current_year++;
        }
		month_diff += time2->tm_mon - time1->tm_mon;
	}
	else
	{
		current_year = time2->tm_year;
		while(current_year < time1->tm_year)
		{
			 month_diff += 12;
			 current_year++;
		}
		month_diff += time1->tm_mon - time2->tm_mon;
	}
	return month_diff;
}
//-----------------------------------------------------------------------------
void graphics::set_cursor_pos(int xCo, int yCo)
{
	COORD cursor_pos = {xCo-1, yCo-1};
	SetConsoleCursorPosition(hConsole, cursor_pos);
}
//-----------------------------------------------------------------------------
void graphics::show_time(const tm* lcTime)
{
	cout << dayOfWeek[lcTime->tm_wday] << ", " << lcTime->tm_mday << ' '
		<< months[lcTime->tm_mon] << ' ' << lcTime->tm_year + 1900 << ". ";
	if(lcTime->tm_hour < 10)
		cout << '0' << lcTime->tm_hour << " : ";
	else
		cout << lcTime->tm_hour << " : ";
	if(lcTime->tm_min < 10)
		cout << '0' << lcTime->tm_min << " : ";
	else
		cout << lcTime->tm_min << " : ";
	if(lcTime->tm_sec < 10)
		cout << '0' << lcTime->tm_sec;
	else
		cout << lcTime->tm_sec;
}
//-----------------------------------------------------------------------------
void graphics::show_time(const tm* lcTime, int line_no)
{
	const int SIZE = 50;
	int length;
	int xCo;
	char memBuff[SIZE];
	ostrstream oMemBuff(memBuff, SIZE);
	oMemBuff << dayOfWeek[lcTime->tm_wday] << ", " << lcTime->tm_mday << ' '
		<< months[lcTime->tm_mon] << ' ' << lcTime->tm_year + 1900 << ". ";
	if(lcTime->tm_hour < 10)
		oMemBuff << '0' << lcTime->tm_hour << " : ";
	else
		oMemBuff << lcTime->tm_hour << " : ";
	if(lcTime->tm_min < 10)
		oMemBuff << '0' << lcTime->tm_min << " : ";
	else
		oMemBuff << lcTime->tm_min << " : ";
	if(lcTime->tm_sec < 10)
		oMemBuff << '0' << lcTime->tm_sec;
	else
		oMemBuff << lcTime->tm_sec;
	oMemBuff << '\0';
	length = strlen(memBuff);
	xCo = (console_size.X - length) / 2;
	set_cursor_pos(1, line_no);
	clear_line();
	set_cursor_pos(xCo, line_no);
	cout << memBuff;
}
//-----------------------------------------------------------------------------
void graphics::wait(int milliseconds)
{
	Sleep(milliseconds);
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////end of file msConGra.cpp////////////////////////////