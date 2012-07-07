//Library.cpp
//Contains definitions for Library.h
//© Skand Hurkat, 2012

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Library.h"
#include "KeyMaker.h"

////////////////////////////////global Methods/////////////////////////////////
void getString(char* str, int size)
{
	do
	{
		cin.get(str, size);
		if(cin.good())
		{
			cin.ignore(size + 80, '\n');
			break;
		}
		else
		{
			cout << "Error in input. Re-enter: ";  
			cin.clear();
			cin.ignore(size + 80, '\n');
		}
	}while(true);
}
//-----------------------------------------------------------------------------
int getInt()
{
	int intVar;
	do
	{
		cin.unsetf(ios::skipws);
		cin >> intVar;
		if(cin.good())
		{
			cin.ignore(80, '\n');
			break;
		}
		else
		{
			cout << "Error in input. Input must be an Integer" << endl
				<< "Re-enter: ";
			cin.clear();
			cin.ignore(80, '\n');
		}
	}while(true);
	return intVar;
}
//-----------------------------------------------------------------------------
bool getYN()
{
	char inChar;
	do
	{
		inChar = getch();
		if(inChar == 'Y' || inChar == 'y')
		{
			cout << inChar;
			return true;
		}
		if(inChar == 'N' || inChar == 'n')
		{
			cout << inChar;
			return false;
		}
	}while(true);
}

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////methods for class book////////////////////////////
book::book(): accCode(0), totalCopies(0), copiesOnShelf(0)
{
	name[0] = '\0';
	author[0] = '\0';
}
//-----------------------------------------------------------------------------
void book::read(int no)
{
	bookFile.seekg((no-1)*sizeof(book));
	bookFile.read(reinterpret_cast<char*>(this), sizeof(book));
}
//-----------------------------------------------------------------------------
void book::write()
{
	bookFile.seekp((accCode-1)*sizeof(book));
	bookFile.write(reinterpret_cast<char*>(this), sizeof(book));
}
//-----------------------------------------------------------------------------
bool book::issue()
{
	if(copiesOnShelf > 0)
	{
		copiesOnShelf--;
		return true;
	}
	else return false;
}
//-----------------------------------------------------------------------------
bool book::deposit()
{
	if(copiesOnShelf < totalCopies)
	{
		copiesOnShelf++;
		return true;
	}
	else return false;
}
//-----------------------------------------------------------------------------
void book::setData(int acc, char* bName, char* bAuthor, int totCopies, int pr)
{
	accCode = acc;
	totalCopies = totCopies;
	price = pr;
	copiesOnShelf = totalCopies;
	strcpy(name, bName);
	strcpy(author, bAuthor);
}
//-----------------------------------------------------------------------------
void book::displayData()
{
	cout << endl << "Details of book are as follows:" << endl
		<< "Accession Code: " << accCode << endl
		<< "Name: " << name << endl << "Author: " << author << endl
		<< "Price: " << price << endl
		<< "Total Copies: " << totalCopies << endl
		<< "Copies on Shelf: " << copiesOnShelf << endl;
}
//-----------------------------------------------------------------------------
bool book::removeBook()
{
	if(copiesOnShelf == totalCopies)
	{
		price = 0;
		totalCopies = 0;
		copiesOnShelf = 0;
		return true;
	}
	else return false;
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

////////////////////////////methods for class member///////////////////////////
member::member(): accountNo(0), deposit(0), bookIssued(0)
{
	name[0] = '\0';
	telNo[0] = '\0';
}
//-----------------------------------------------------------------------------
void member::input(int accNo,char* memName,char* tele)
{
	accountNo = accNo;
	strcpy(name, memName);
	strcpy(telNo, tele);
	deposit = 0;
}	
//-----------------------------------------------------------------------------
void member::read(int no)
{
	memberFile.seekg((no-1)*sizeof(member));
	memberFile.read(reinterpret_cast<char*>(this), sizeof(member));
}
//-----------------------------------------------------------------------------
void member::write()
{
	memberFile.seekp((accountNo-1)*sizeof(member));
	memberFile.write(reinterpret_cast<char*>(this), sizeof(member));
}
//-----------------------------------------------------------------------------
int member::issueBook(book* ptrBook)
{
	int toPay = 0;
	if(bookIssued) throw bookAlreadyIssued();
	if(ptrBook->issue())
	{
		bookIssued = ptrBook->getAccCode();
		dateBookIssued = *console.get_time();
		if(deposit < MIN_DEPOSIT)
		{
			toPay += MIN_DEPOSIT - deposit;
			deposit = MIN_DEPOSIT;
		}
		if(deposit < ptrBook->getPrice())
		{
			toPay += ptrBook->getPrice() - deposit;
			deposit = ptrBook->getPrice();
		}
		return toPay;
	}
	else throw couldNotIssueBook();
}
//-----------------------------------------------------------------------------
int member::depositBook(book* ptrBook)
{   
	int toReturn = 0;
	if(!bookIssued) throw bookNotIssued();
	ptrBook->read(bookIssued);
	if(ptrBook->getAccCode() == bookIssued)
	{   
		toReturn = deposit - MIN_DEPOSIT;
		toReturn -= (console.months_elapsed(console.get_time(),&dateBookIssued))*MONTHLY_CHARGES;
		ptrBook->deposit();
		bookIssued = 0;
		deposit = MIN_DEPOSIT;
		return toReturn;
	}
	else throw accessionCodeMismatch();
}
//-----------------------------------------------------------------------------
void member::setData(int acNo, char* memName, char* memTelNo)
{
	accountNo = acNo;
	strcpy(name, memName);
	strcpy(telNo, memTelNo);
	bookIssued = 0;
	deposit = MIN_DEPOSIT;
}
//-----------------------------------------------------------------------------
void member::displayData()
{
	cout << endl << "Details of member are as follows:" << endl
		<< "Account Number: " << accountNo << endl
		<< "Name: " << name << endl << "Phone: " << telNo << endl
		<< "Deposit: " << deposit << endl;
	if(bookIssued != 0)
		cout << "Book Issued: " << bookIssued << endl << "Date book issued: "
			<< console.asc_date(&dateBookIssued) << endl;
}
//-----------------------------------------------------------------------------
int member::removeMember()
{
	int toReturn;
	if(!bookIssued)
	{
		toReturn = deposit;
		deposit = 0;
		return toReturn;
	}
	else throw bookAlreadyIssued();
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

////////////////////////methods for class userInterface////////////////////////
//-----------------------------------------------------------------------------
userInterface::userInterface(): choice(0)
{
	bookFile.open("Books.dat", ios::in|ios::out|ios::binary);
	if(!bookFile)
	{
		cerr << "Error opening book database. Press any key to continue";
		getch();
		exit(1);
	}
	memberFile.open("Members.dat", ios::in|ios::out|ios::binary);
	if(!memberFile)
	{
		cerr << "Error opening member database. Press any key to continue";
		getch();
		exit(1);
	}
	isRegistered = false;
	fstream registFile;
	registFile.open("Application.sif", ios::in|ios::out|ios::binary);
	if(!registFile)
	{
		cerr << "Error checking registration of the program." << endl
			 << "Press any key to continue.";
		getch();
		exit(1);
	}
	registFile.clear();
	registFile.seekg(0);
	registFile.read(reinterpret_cast<char*>(&isRegistered), sizeof(bool));
	while(!isRegistered)
	{
		isRegistered = KeyMaker.interact();
		if(!isRegistered)
		{
			console.set_cursor_pos(1, console.get_max_y());
			cout << "Invalid Key. Press any key to continue...";
			getch();
		}
	}
	registFile.clear();
	registFile.seekp(0);
	registFile.write(reinterpret_cast<char*>(&isRegistered), sizeof(bool));
	registFile.close();
	ptrBookInputScreen = new bookInputScreen;
	ptrBookDeleteScreen = new bookDeleteScreen;
	ptrMemberInputScreen = new memberInputScreen;
	ptrMemberDeleteScreen = new memberDeleteScreen;
	ptrBookIssueScreen = new bookIssueScreen;
	ptrBookReturnScreen = new bookReturnScreen;
	ptrBookSearchScreen = new bookSearchScreen;
	ptrMemberSearchScreen = new memberSearchScreen;
}
//-----------------------------------------------------------------------------
userInterface::~userInterface()
{
	delete ptrBookInputScreen;
	delete ptrBookDeleteScreen;
	delete ptrBookIssueScreen;
	delete ptrBookReturnScreen;
	delete ptrBookSearchScreen;
	delete ptrMemberInputScreen;
	delete ptrMemberDeleteScreen;
	delete ptrMemberSearchScreen;
	bookFile.close();
	memberFile.close();
}
//-----------------------------------------------------------------------------
void userInterface::truncDatabase()
{
	console.clear_screen();
	cout << endl << endl << "Are you sure??" << endl
		 << "Truncating database will delete all records (Y/N): ";
	if(getYN())
	{   bookFile.close();
	    memberFile.close();
        bookFile.open("Books.dat", ios::in|ios::out|ios::binary|ios::trunc);
     	 if(!bookFile)
		 {
		 cerr << "Error truncating book database. Press any key to exit";
		 getch();
		 exit(1);
		 }
		 memberFile.open("Members.dat", ios::in|ios::out|ios::binary|ios::trunc);
	     if(!memberFile)
		 {
		 cerr << "Error truncating member database. Press any key to exit";
		 getch();
		 exit(1);
		 }
		 cout<<endl<<"Database truncated"; 
	}
	cout<<endl<<"Press any key to return to main"; getch();
}
void userInterface::drawMenu()
{
	console.clear_screen();
	console.draw_box(MID_X - 25, MID_Y - 7, 50, 15);
	console.set_cursor_pos(MID_X - 24, MID_Y - 5);
	cout << "Select your choice:";
	console.set_cursor_pos(MID_X - 22, MID_Y - 4);
	cout << "1. Issue book";
	console.set_cursor_pos(MID_X - 22, MID_Y - 3);
	cout << "2. Return book";
	console.set_cursor_pos(MID_X - 22, MID_Y - 2);
	cout << "3. Search existing book";
	console.set_cursor_pos(MID_X - 22, MID_Y - 1);
	cout << "4. Enter new book";
	console.set_cursor_pos(MID_X - 22, MID_Y);
	cout << "5. Delete existing book";
	console.set_cursor_pos(MID_X - 22, MID_Y + 1);
	cout << "6. Search existing member";
	console.set_cursor_pos(MID_X - 22, MID_Y + 2);
	cout << "7. Enter new member";
	console.set_cursor_pos(MID_X - 22, MID_Y + 3);
	cout << "8. Delete existing member";
	console.set_cursor_pos(MID_X - 22, MID_Y + 4);
	cout << "9. Truncate existing database";
	console.set_cursor_pos(MID_X - 24, MID_Y + 6);
	cout << "Press ESCAPE to quit";
	console.set_cursor_pos(MID_X - 9, 3);
	cout << "Welcome to LIBRARY";
}
//-----------------------------------------------------------------------------
void userInterface::quit()
{
	console.clear_screen();
	console.set_cursor_pos(MID_X-13, MID_Y-3);
	cout << "Thank You for using LIBRARY.";
	console.set_cursor_pos(MID_X-5, MID_Y-1);
	cout << "Created By:";
	console.set_cursor_pos(MID_X-5, MID_Y);
	cout << "M Aravindh";
	console.set_cursor_pos(MID_X-10, MID_Y+1);
	cout << "Navraj Shubham Saxena";
	console.set_cursor_pos(MID_X-6, MID_Y+2);
	cout << "Skand Hurkat";
	console.set_cursor_pos(1, console.get_max_y());
	cout << "Press any key to continue";
	getch();
	exit(0);
}
//-----------------------------------------------------------------------------
void userInterface::interact()
{
	char inChar;
	do
	{   
		drawMenu();
		console.set_cursor_pos(MID_X - 23, MID_Y - 4 + choice);
		cout << POINTER;
		do
		{
			console.show_time(console.get_time(), 1);
			if(kbhit())
			{
				inChar = getch();
				console.set_cursor_pos(MID_X - 23, MID_Y - 4 + choice);
				cout << SPACE;
				switch(inChar)
				{
				case UP_KEY:	choice--; break;
				case DOWN_KEY:	choice++; break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':		choice = inChar - '1'; break;		
				}
				if(choice < 0) choice += 9;
				choice %= 9;
				console.set_cursor_pos(MID_X - 23, MID_Y - 4 + choice);
				cout << POINTER;
				if(inChar == ENTER) break;
				else if(inChar == ESC) quit();
			}
			console.wait(100);
		}while(true);
		switch(choice)
		{       
		case 0: ptrBookIssueScreen->interact();			break;
		case 1: ptrBookReturnScreen->interact();		break;
		case 2: ptrBookSearchScreen->interact();		break;
		case 3: ptrBookInputScreen->interact();			break;
		case 4: ptrBookDeleteScreen->interact();		break;
		case 5: ptrMemberSearchScreen->interact();		break;
		case 6: ptrMemberInputScreen->interact();		break;
		case 7: ptrMemberDeleteScreen->interact();		break;
		case 8: truncDatabase();						break;
		}
	}while(true);
}

///////////////////////////////////////////////////////////////////////////////

//////////////////////methods for class book Input Screen//////////////////////
//-----------------------------------------------------------------------------
bookInputScreen::bookInputScreen(): aCode(0), copiesReceived(0), priceOfBook(0.0)
{
	ptrBook = new book;
	nameOfBook[0] = '\0';
	nameOfAuthor[0] = '\0';
}
//-----------------------------------------------------------------------------
bookInputScreen::~bookInputScreen()
{
	delete ptrBook;
}
//-----------------------------------------------------------------------------
void bookInputScreen::interact()
{	bool avail = false;
	int counter = 0; 
	console.clear_screen();
	{   
		cout<<"\t\t\tBook Registration";
		console.set_cursor_pos(1,3);
		cout<<"Enter the following details of the book to add the book to the library";
		console.set_cursor_pos(1,4);
		cout<<"Book name: ";		getString(nameOfBook,80);   
		cout<<"Name of Author: ";	getString(nameOfAuthor,80);
		cout<<"Copies Received: ";	copiesReceived = getInt();
		cout<<"Price of book: ";	priceOfBook = getInt();
		bookFile.clear();
		bookFile.seekg(0,ios::beg); 
		while(!bookFile.eof())
		{
			counter++; 
			ptrBook->setData(counter," \0"," \0",0,0);
			ptrBook->read(counter);
			if(ptrBook->getTotalNoCopies() == 0) 
			{
				aCode = counter;
				avail = true;
				bookFile.clear();
				break;
			}
		}
		bookFile.clear();
		if(!avail)
			aCode = counter;
		cout<<"\nConfirm book purchase: ";  
		if(getYN() == true)
		{
			cout << "\nAccession Code is: "<< aCode;
			ptrBook->setData(aCode,nameOfBook,nameOfAuthor,copiesReceived,priceOfBook);
			ptrBook->write();
			ptrBook->displayData();
		}
		cout<<"\nPress any key to continue";
		getch();
	}
	bookFile.clear();
}
//-----------------------------------------------------------------------------
//////////////////////end of methods for bookInputScreen///////////////////////

/////////////////////////methods for bookDeleteScreen//////////////////////////
bookDeleteScreen::bookDeleteScreen()
{  
	ptrBook = new book;
}
//-----------------------------------------------------------------------------
bookDeleteScreen::~bookDeleteScreen()
{
	delete ptrBook;
}
//-----------------------------------------------------------------------------
void bookDeleteScreen::interact()
{
	int accno, counter = 0;
	bool avail = false;
	console.clear_screen();
	{
		console.set_cursor_pos(MID_X-5,1);
		cout<<"Book Deletion";
		console.set_cursor_pos(1,3);
		cout<<"Enter the accession number of the book to be deleted: ";
		accno = getInt();
		bookFile.clear();
		bookFile.seekg(0,ios::beg);
		while(!bookFile.eof())
		{
			counter++;
			ptrBook->read(counter);
			if((ptrBook->getAccCode() == accno)&&
				(ptrBook->getTotalNoCopies()!=0))
			{
				avail = true;
				break;
			}
		}
		ptrBook->read(accno);
		if(avail)
		{
			cout<<"\nThe details of the book to be removed are: ";
			ptrBook->displayData();
			cout<<"\nConfirm Removal of Book: ";  
			if(getYN() == true)
			{ 
				if(ptrBook->removeBook() == true)
					ptrBook->write();
				else
					cout << endl << "Some of the copies of this book are issued. "
						<< "Book records cannot be deleted";
			}
		}
		else
		{
			cout << endl << "Invalid book accession code.";
			bookFile.clear();
		}
		cout << endl << "Press any key to continue";
		getch();
	}
	bookFile.clear();
}
//-----------------------------------------------------------------------------
//////////////////end of methods for class bookDeleteScreen////////////////////

/////////////////////methods for class memberInputScreen///////////////////////
//-----------------------------------------------------------------------------
memberInputScreen::memberInputScreen(): mAccNo(0)
{
	mName[0] = '\0';
	mTelNo[0] = '\0';
	ptrMember = new member;
}
//-----------------------------------------------------------------------------
memberInputScreen::~memberInputScreen()
{
	delete ptrMember;
}
//-----------------------------------------------------------------------------
void memberInputScreen::interact()
{
	bool avail = false;
	int counter = 0;
	console.clear_screen();
	{
		console.set_cursor_pos(MID_X-6,1);
		cout<<"Member Registration";
		console.set_cursor_pos(1,3);
		cout<<"\nEnter the following details of the new member: ";
		cout<<"\nMember name: ";	getString(mName,80);
		cout<<"Telephone Number: ";	getString(mTelNo,11);
		memberFile.seekg(0,ios::beg);
		while(!memberFile.eof())
		{
			counter++;
			ptrMember->input(counter," \0"," \0");
			ptrMember->read(counter);
			if(ptrMember->getDeposit() == 0)
			{
				mAccNo = counter;
				avail = true;
				memberFile.clear();
				break;
			}
		}
		if(!avail)
		{
			mAccNo = counter;
			memberFile.clear();
		}
		cout<<"\nConfirm Member Registration: ";  
		if(getYN() == true)
		{
			cout<<"\nAccession Code is: "<<mAccNo;
			cout<<"\nPay initial deposit of Rs. 200";
			getch();
			ptrMember->setData(mAccNo,mName,mTelNo);
			ptrMember->write();
			ptrMember->displayData();
		}
		cout << endl << "Press any key to continue";
		getch();
	}
	memberFile.clear();
}
//-----------------------------------------------------------------------------
//////////////////end of methods for class memberInputScreen///////////////////
////////////////////methods for class memberDeleteScreen///////////////////////
//-----------------------------------------------------------------------------
memberDeleteScreen::memberDeleteScreen()
{
	ptrMember = new member;
}
//-----------------------------------------------------------------------------
memberDeleteScreen::~memberDeleteScreen()
{
	delete ptrMember;
}
//-----------------------------------------------------------------------------
void memberDeleteScreen::interact()
{
	int accno, counter = 0;
	bool avail = false;
	console.clear_screen();
	console.set_cursor_pos(MID_X-5,1);
	cout << "Member Detetion";
	console.set_cursor_pos(1,3);
	cout << "Enter the accession number of the member to be deleted: ";
	accno = getInt();
	memberFile.seekg(0,ios::beg);
	while(!memberFile.eof())
	{
		counter++;
		ptrMember->read(counter);
		if((ptrMember->getAccountNo()== accno)&&(ptrMember->getDeposit()!=0)) 
		{
			avail = true;
			break;
		}
	}
	if(avail)
	{
		cout<<"\nThe details of the member to be removed are: ";
		ptrMember->displayData();
		cout<<"\nConfirm Removal of Member<Y/N>: ";  
		if(getYN() == true)
			try
			{
				int toPay = ptrMember->removeMember();
				cout<<"\nPlease return Deposit Rs: "<< toPay;
				ptrMember->write();
			}
		catch(member::bookAlreadyIssued)
			{
				cerr << "Error! Given member has an issued book."
					<< "Please return the book and try again"
					<< "\nPress any key to continue";
				getch();
			}
	}
	else
	{
		cerr << "\nInvalid Accession no. There is no such member in the library";
		memberFile.clear();
	}
	cout << endl << "Press any key to continue";
	getch();
	memberFile.clear();  
}
//-----------------------------------------------------------------------------
/////////////////end of methods for class memberDeleteScreen///////////////////

//////////////////////methods for class bookSearchScreen///////////////////////
//-----------------------------------------------------------------------------
bookSearchScreen::bookSearchScreen() 
{
	ptrBook = new book;
	strcpy(nameOfBook," \0");
	strcpy(nameOfAuthor," \0");
}
//-----------------------------------------------------------------------------
bookSearchScreen::~bookSearchScreen()
{
	delete ptrBook;
}
//-----------------------------------------------------------------------------
void bookSearchScreen::interact()
{
	console.clear_screen();
	int choice = 0, counter = 0;
	bool avail = false;
	char inchar;
	console.set_cursor_pos(MID_X-6,1);
	cout << "Book Search";
	console.set_cursor_pos(1,3);
	cout << "Enter the field you want to search by: \n";
	cout << "  1.Book Accession Number.\n  2.Book name.\n  3.Name of Author.";
	console.set_cursor_pos(1,4);
	cout << POINTER;
	int get = 0;
	do
	{
		if(kbhit())
		{
			inchar = getch();
			console.set_cursor_pos(1,4+choice); 
			cout << POINTER;
			switch(inchar)
			{
			case UP_KEY:	choice--;	break;
			case DOWN_KEY:	choice++;	break;
			case '1':		choice = 0;	break;
			case '2':		choice = 1;	break;
			case '3':		choice = 2;	break;
			case ENTER:		console.set_cursor_pos(1,7);   
							cout<<"\nSearch by: "<<choice+1;  
							get = choice+1;   break;
			}
			if(choice == -1)
				choice = 2;
			else 
				choice %= 3;
			cout<<"\b ";
			console.set_cursor_pos(1,4+choice);
			cout << POINTER;
		}
	}while(!get);
	console.set_cursor_pos(1,8);
	bookFile.clear();
	switch(get)
	{
	case 1:	cout<<"\nEnter the accession number: ";
			accNo = getInt();
			bookFile.seekg(0,ios::beg);
			while(!bookFile.eof())
			{ 
				counter++;
				ptrBook->read(counter);
				if(!bookFile.eof()) 
				if((ptrBook->getAccCode() == accNo)&&(ptrBook->getTotalNoCopies()!= 0))
				{	avail = true;
					cout<<"\nDetails of this book are:\n";
					ptrBook->displayData();
					break;
				}
			}
			break;
	case 2:	cout<<"\nEnter the name of the book: "; 
			getString(nameOfBook,40);
			bookFile.seekg(0,ios::beg);
			while(!bookFile.eof())
			{
				counter++;
				ptrBook->read(counter);
				if(!bookFile.eof())
					if((!strcmpi(ptrBook->getName(),nameOfBook))&&
						(ptrBook->getTotalNoCopies()!= 0))
					{
						avail = true;
						cout<<"\nDetails of this book are:\n";
						ptrBook->displayData();
						break;
					}
			}
            break;
	case 3:	cout<<"\nEnter the name of Author: ";
			getString(nameOfAuthor,40);
			bookFile.seekg(0,ios::beg);
			while(!bookFile.eof())
			{
				counter++;
				ptrBook->read(counter);
				if(!bookFile.eof())
					if((!strcmpi(ptrBook->getAuthor(),nameOfAuthor))&&
						(ptrBook->getTotalNoCopies()!= 0))
					{
						avail = true;
						cout<<"\nDetails of a book by this Author are:\n";
						ptrBook->displayData();
						cout<<endl<<"Press any key to continue";  getch();
					}
			}
			break;
	}
	if(!avail)
	{
		cout<<"\nSorry! This book is not available in the library";
		getch();
		bookFile.clear();
	}
	cout<<endl<<"Press any key to go back to main menu";
	getch();
	bookFile.clear();                       
}
//-----------------------------------------------------------------------------
//////////////////end of methods for class bookSearchScreen////////////////////
//////////////////////methods for class bookIssueScreen////////////////////////
//-----------------------------------------------------------------------------
bookIssueScreen::bookIssueScreen()
{
	ptrMember = new member;
	ptrBook = new book;
}
//-----------------------------------------------------------------------------
bookIssueScreen::~bookIssueScreen()
{
	delete ptrMember;
	delete ptrBook;
}
//-----------------------------------------------------------------------------
void bookIssueScreen::interact()
{
	int mAccNo, bAccNo, counter = 0;
	bool avail = false, valid = true;
	console.clear_screen();
	console.set_cursor_pos(MID_X-5,1);
	cout<<"Book Issue";
	console.set_cursor_pos(1,3);
	{
		cout<<"Enter accession number of member who wants to issue the book: ";
		mAccNo = getInt();
		memberFile.seekg(0,ios::beg);
		while(!memberFile.eof())
		{
			counter++;
			ptrMember->read(counter);
			if((ptrMember->getAccountNo() == mAccNo)&&
				(ptrMember->getDeposit()!=0))
			{
				avail = true;
				cout<<"\nMember found\n";
				ptrMember->displayData();
				if(ptrMember->getBookIssued())
				{ 
					cout<<"\nThe concerned member has already issued one book. He can issue only one at a time";
					valid = false;					
				}
				break;
			}
		}
		if(!avail)
		{
			cout<<"\nSorry! No such member exists"; 
			memberFile.clear();
		}
		else if(valid)
		{
			counter = 0;
			avail = false;
			cout<<"\nEnter the accession number of the book to be issued: ";
			bAccNo = getInt();
			bookFile.clear();
			bookFile.seekg(0,ios::beg);
			while(!bookFile.eof())
			{
				counter++;
				ptrBook->read(counter);
				if((ptrBook->getAccCode() == bAccNo)&&
					(ptrBook->getTotalNoCopies()))
				{
					avail = true;
					cout<<"\nBook records found\n";
					ptrBook->displayData();
					break;
				}
			}
			if(!avail)
			{ 
				cout<<"\nSorry! No such book exists";
				bookFile.clear();
			}
			else
			{
				try
				{
					int result;
					result = ptrMember->issueBook(ptrBook);
					cout<<"\nThe member has to pay Rs. :" << result;
					getch();
					ptrMember->write();
					ptrBook->write();
					cout<<endl<<"Book has been issued.\nOne copy removed from shelf";  
				}
				catch(member::bookAlreadyIssued)
				{
					cout << endl << "Member has already issued book. "
						<<"Only one book can be issued at a time.";
				}
				catch(member::accessionCodeMismatch)
				{
					cerr << "\nDatabase corrupt. Try restarting the program "
						<< "or truncating the database.";
				}
			}
		}
	}
	cout << endl << "\nPress any key to go back to main";
	getch();
}
//-----------------------------------------------------------------------------
///////////////////end of methods for class bookIssueScreen////////////////////

//////////////////////methods for class bookReturnScreen///////////////////////
//-----------------------------------------------------------------------------
bookReturnScreen::bookReturnScreen()
{
	ptrBook = new book;
	ptrMember = new member;
}
//-----------------------------------------------------------------------------
bookReturnScreen::~bookReturnScreen()
{
	delete ptrBook;
	delete ptrMember;
}
//-----------------------------------------------------------------------------
void bookReturnScreen::interact()
{   console.clear_screen();
	int counter = 0;
	bool valid = true, avail = false;
	int mAccNo;
	console.set_cursor_pos(MID_X-5,1);
	cout<<"Book Return";
	console.set_cursor_pos(1,3);  
	cout<<"Enter accession number of member who wants to return the book: ";
	mAccNo = getInt();
	memberFile.clear ();
	memberFile.seekg(0,ios::beg);
	while(!memberFile.eof())
	{
		counter++;
		ptrMember->read(counter);
		if((ptrMember->getAccountNo() == mAccNo)&&
			(ptrMember->getDeposit()!=0) )
		{
			avail = true;
            ptrMember->displayData();
			if(ptrMember->getBookIssued() == 0)
			{ 
				cout<<"\nThe concerned member has not issued any book!!!";
				valid = false;					
			}
			break;
		}
	}
	if(!avail)
	{
         cout << endl << "Member does not exist"; 
		 memberFile.clear();
	}
	else if(valid)
	{
		ptrBook->read(ptrMember->getBookIssued());
		try
		{
			int toReturn = ptrMember->depositBook(ptrBook);
			if(toReturn > 0)
				cout << endl << "Please return Rs. " << toReturn;
			else if(toReturn < 0)
				cout << endl << "Please take Rs. " << -toReturn;
             getch();		
		}
		catch(member::bookNotIssued)
		{
			cerr << "\nGiven Member has not issued any book";
		}
		catch(member::accessionCodeMismatch)
		{
			cerr << "\nError in database. Try restarting the program "
				<< "or truncating the database.";
		}
		cout << endl << "Book Returned";
		ptrMember->write();
		ptrBook->write();
	}
	cout << endl << "Press any key to go back to main";  getch();
}
//-----------------------------------------------------------------------------
//////////////////end of methods for class bookReturnScreen////////////////////

/////////////////////methods for class memberSearchScreen//////////////////////
//-----------------------------------------------------------------------------
memberSearchScreen::memberSearchScreen()
{
	ptrMember = new member;
}
//-----------------------------------------------------------------------------
memberSearchScreen::~memberSearchScreen()
{
	delete ptrMember;
}
//-----------------------------------------------------------------------------
void memberSearchScreen::interact()
{
	console.clear_screen();
	int counter = 0, choice = 0;
	bool avail = false;
	char inchar;
	console.set_cursor_pos(MID_X-7,1);
	cout<<"Member Search";
	console.set_cursor_pos(1,3);
	cout<<"Enter the field you want to search by: \n";
	cout<<"  1.Member Accession Number.\n  2.Member name.\n  3.Acc. No. of book issued.";
	console.set_cursor_pos(1,4);
	cout<<POINTER;
	int get=0;
	do
	{
		if(kbhit())
		{
			inchar = getch();
			console.set_cursor_pos(1,4+choice); 
			cout<<POINTER;
			switch(inchar)
			{
			case UP_KEY:	choice--;	break;
			case DOWN_KEY:	choice++;	break;
			case '1':		choice = 0;	break;
			case '2':		choice = 1;	break;
			case '3':		choice = 2;	break;
			case ENTER: console.set_cursor_pos(1,6);   cout<<endl<<"Search by: "<<choice+1;  get = choice+1; break;
			}
			if(choice == -1)
				choice = 2;
			else 
				choice%=3;
			cout<<"\b ";
			console.set_cursor_pos(1,4+choice);
			cout<<POINTER;
		}
	}while(!get);
	console.set_cursor_pos(1,8);
	switch(get)
	{
	case 1:	cout<<"\nEnter the accession number: ";   memberAccountNo = getInt();
			memberFile.seekg(0,ios::beg);
			while(!memberFile.eof())
			{
				counter++;
                ptrMember->read(counter);
                if(!memberFile.eof()) 
				if((ptrMember->getAccountNo() == memberAccountNo)&&(ptrMember->getDeposit()!=0))
				{
					avail = true;
					cout<<"\nDetails of this member are:\n";
					ptrMember->displayData();
					break;
				}
			}
			break;
	case 2:	cout<<"\nEnter the name of the Member: "; 
			getString(memberName,80);
			memberFile.seekg(0,ios::beg);
			while(!memberFile.eof())
			{
				counter++;
				ptrMember->read(counter);
				if(!memberFile.eof()) 
				if((!strcmpi(ptrMember->getName(),memberName))&&
					(ptrMember->getDeposit()!=0))
				{
					avail = true;
					cout<<"\nDetails of this member are:\n";
					ptrMember->displayData();
					break;
				}
			}
			break;
	case 3:	int accCode;
			cout<<"\nEnter the Accession number of the concerned book: ";
			accCode = getInt();
			memberFile.seekg(0,ios::beg);
			while(!memberFile.eof())
			{
				counter++;
				ptrMember->read(counter);
				if(!memberFile.eof()) 
				if((ptrMember->getBookIssued() == accCode)&&
					(ptrMember->getDeposit() != 0)) 
				{
					avail = true;
					cout<<"\nDetails of a member who has issued this book are:\n";
					ptrMember->displayData();
					break;
				}
			}
	}
	if(!avail)
	{
		cout<<"\nSorry! Such person is not library member";
		getch();
		memberFile.clear();
	}
	cout<<"\nPress any key to go back to main menu";
	getch();
	memberFile.clear();
}
//-----------------------------------------------------------------------------
/////////////////end of methods for class memberSearchScreen///////////////////

///////////////////////////end of file Library.cpp/////////////////////////////