#if !defined _INC_LIBRARY
#define _INC_LIBRARY

#include <iostream>
#include <fstream>
#include <process.h>
#include <conio.h>
#include "msConGra.h"
#include "KeyMaker.h"
using namespace std;

const int MIN_DEPOSIT = 200;
const int MONTHLY_CHARGES = 200;

//////////////////////////////graphics subsystem////////////////////////////////

const int MID_X = console.get_max_x() / 2;
const int MID_Y = console.get_max_y() / 2;

////////////////////////////////global methods//////////////////////////////////

void getString(char* str, int size);
int getInt();
bool getYN();

////////////////////////////global file stream objects//////////////////////////

static fstream bookFile;
static fstream memberFile;

//////////////////////////////////class book////////////////////////////////////

class book
{
private:
	int accCode;
	char name[80];
	char author[80];
	int totalCopies;
	int copiesOnShelf;
	int price;
public:
	book();
	~book()
	{ /*Empty*/ }
	
	void read(int);
	void write();

	bool issue();
	bool deposit();
	void setData(int, char*, char*, int, int);
	void displayData();
	bool removeBook();
	int getAccCode() const
	{
		return accCode;
	}
	char* getName()
	{
		return name;
	}
	char* getAuthor()
	{
		return author;
	}
	int getPrice() const
	{
		return price;
	}
    int getTotalNoCopies() const
    {
		return totalCopies;
	}

};

///////////////////////////////end of class book////////////////////////////////

/////////////////////////////////class member///////////////////////////////////

class member
{
private:
	int accountNo;
	char name[80];
	char telNo[11];
	int deposit;
	int bookIssued;
	tm dateBookIssued;
public:
	class bookNotIssued{ };
	class accessionCodeMismatch{ };
	class couldNotIssueBook{ };
	class bookAlreadyIssued{ };			//exception classes

	member();
	~member()
	{ /*Empty*/ }

	void read(int);
	void write();

	int issueBook(book*);
	int depositBook(book*);
	void setData(int, char*, char*);
	void input(int,char* ,char* );
	void displayData();
	int removeMember();
	int getAccountNo() const
	{
		return accountNo;
	}
	char* getName()
	{
		return name;
	}
	int getDeposit() const
	{
		 return deposit;
	}
	int getBookIssued() const
	{
		return bookIssued;
	}
};

//////////////////////////////end of class member///////////////////////////////

/////////////////////////////class bookInputScreen//////////////////////////////

class bookInputScreen
{
private:
	book* ptrBook;
	int aCode;
	char nameOfBook[80];
	char nameOfAuthor[80];
	int copiesReceived;
	int priceOfBook;
public:
	bookInputScreen();
	~bookInputScreen();
	void interact();
};

//////////////////////////end of class bookInputScreen//////////////////////////

/////////////////////////////class bookDeleteScreen/////////////////////////////

class bookDeleteScreen
{
private:
	book* ptrBook;
public:
	bookDeleteScreen();
	~bookDeleteScreen();
	void interact();
};

//////////////////////////end of class bookDeleteScreen/////////////////////////

////////////////////////////class memberInputScreen/////////////////////////////

class memberInputScreen
{
private:
	member* ptrMember;
	int mAccNo;
	char mName[80];
	char mTelNo[11];
public:
	memberInputScreen();
	~memberInputScreen();
	void interact();
};

//////////////////////////end of class memberInputScreen////////////////////////

///////////////////////////class memberDeleteScreen/////////////////////////////

class memberDeleteScreen
{
private:
	member* ptrMember;
public:
	memberDeleteScreen();
	~memberDeleteScreen();
	void interact();
};

//////////////////////////end of class memberDeleteScreen////////////////////////

////////////////////////////class bookSearchScreen//////////////////////////////

class bookSearchScreen
{
private:
	book* ptrBook;
	int accNo;
	char nameOfBook[40];
	char nameOfAuthor[30];
public:
	bookSearchScreen();
	~bookSearchScreen();
	void interact();
};

//////////////////////////end of class bookSearchScreen/////////////////////////

//////////////////////////////class bookIssueScreen/////////////////////////////

class bookIssueScreen
{
private:
	book* ptrBook;
	member* ptrMember;


public:
	bookIssueScreen();
	~bookIssueScreen();
	void interact();
};

//////////////////////////end of class bookInputScreen//////////////////////////

/////////////////////////////class bookReturnScreen/////////////////////////////

class bookReturnScreen
{
private:
	book* ptrBook;
	member* ptrMember;
public:
	bookReturnScreen();
	~bookReturnScreen();
	void interact();
};

/////////////////////////end of class bookReturnScreen//////////////////////////

////////////////////////////class memberSearchScreen////////////////////////////

class memberSearchScreen
{
private:
	member* ptrMember;
	int memberAccountNo;
	char memberName[80];
public:
	memberSearchScreen();
	~memberSearchScreen();
	void interact();
};

////////////////////////end of class memberSearchScreen/////////////////////////

//////////////////////////////class userInterface///////////////////////////////

class userInterface
{
private:
	bookInputScreen*	ptrBookInputScreen;
	bookDeleteScreen*	ptrBookDeleteScreen;
	bookIssueScreen*	ptrBookIssueScreen;
	bookReturnScreen*	ptrBookReturnScreen;
	bookSearchScreen*	ptrBookSearchScreen;
	memberInputScreen*	ptrMemberInputScreen;
	memberDeleteScreen*	ptrMemberDeleteScreen;
	memberSearchScreen*	ptrMemberSearchScreen;
	int choice;
	bool isRegistered;
	keyMaker KeyMaker;
	void drawMenu();
	void truncDatabase();
	void quit();
public:
	userInterface();
	~userInterface();
	void interact();
};

//////////////////////////end of class userInterface////////////////////////////
#endif
/////////////////////////////end of file library.h//////////////////////////////
