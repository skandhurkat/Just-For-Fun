// ScientificCalculator.cpp
// Copyright Skand Hurkat, Indrasen Bhattacharya, Rushabh Shah,
// Kumardhruv Soni, 2008

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdlib>                  //for exit()
#include <stack>                    //Stacks are the basic data structures in any calculator
#include <cmath>                    //For mathematical functions
#include <ncurses.h>                //For a GUI in the console
using namespace std;

int main()                          //Main function begins
{
  initscr();                        //Initialize curses
  raw();                            //Disable buffering of the console. Make characters available as soon as the user types them
  noecho();                         //Disable echoing of keystrokes
  keypad(stdscr, TRUE);             //Enable function keys and ESC key etc.
  curs_set(0);                      //Set cursor invisible
  stack<float> operandStack;        //Stores operands
  stack<char> operatorStack;        //Stores operators. Note that only binary operators and open brackets are stored. Unary operators act immediately on the operands
  int ch;                           //store keypress
  MEVENT event;                     //Structure storing the nature and position of the mouse event occured
  mousemask(BUTTON1_CLICKED, NULL); //Enable Mouse leftclick only
  mvprintw(2, 2, "0");              //Print Screen (GUI) for first and only time
  mvprintw(5, 2, "inv  AC Off    7  8  9  +");
  mvprintw(6, 2, "sin cos tan    4  5  6  -");
  mvprintw(7, 2, "x^y  ln log    1  2  3  *");
  mvprintw(8, 2, "+/-  (   )     0  .  =  /");
  bool sawDecimal = false;          //If number is being entered, does the number have a decimal point
  bool enteringNumber = true;       //Is number being entered
  int countPostDecimal = 0;         //Digits entered after decimal point
  float tempOperand1, tempOperand2; //Temporary operands popped from operandStack
  bool inverse = false;             //Was inverse key pressed
  bool errorOccured = false;        //Has any error occured?
  operandStack.push(0);             //Initialize operator Stack. User may do something like 0+2.
  mvprintw(2, 2, "%f", operandStack.top()); //Write to screen buffer
  refresh();                        //Display buffer
  while(true)
    {
      if(!sawDecimal) countPostDecimal = 0;  //If number is not entered, no digits are entered after decimal point (Kind of obvious)
      if(!enteringNumber) countPostDecimal = 0; //Ditto
      ch = getch();                 //Get a keypress or mouse click from user
      if(ch == KEY_MOUSE)           //If mouseclick was obtained....
	{
	  if(getmouse(&event) == OK)//Legal event
	    {
	      if(event.bstate & BUTTON1_CLICKED) //Button1 Clicked
		{
		  if(event.y == 5)  //1st row of the button panel
		    {
		      if(event.x >= 2 && event.x < 5)  //inverse key pressed
			ch = 'i';   //'i' is keyboard shortcut for inverse
		      else if(event.x >= 6 && event.x < 9) //All clear
			ch = 27;    //ESC is keyboard shortcut for All clear
		      else if(event.x >= 10 && event.x < 13) //Off
			ch = 'o';   //'o' is keyboard shortcut for Off
		      else if(event.x >= 16 && event.x <= 18) //7
			ch = '7';
		      else if(event.x >= 19 && event.x <= 21) //8
			ch = '8';
		      else if(event.x >= 22 && event.x <= 24) //9
			ch = '9';
		      else if(event.x >= 25 && event.x <= 27) //+
			ch = '+';
		    }
		  else if(event.y == 6) //2nd row of the button panel
		    {
		      if(event.x >= 2 && event.x < 5) //sin
			ch = 's';   //'s' is keyboard shortcut for sin
		      else if(event.x >= 6 && event.x < 9) //cos
			ch = 'c';   //'c' is keyboard shortcut for cos
		      else if(event.x >= 10 && event.x < 13) //tan
			ch = 't';   //'t' is keyboard shortcut for tan
		      else if(event.x >= 16 && event.x <= 18) //4
			ch = '4';
		      else if(event.x >= 19 && event.x <= 21) //5
			ch = '5';
		      else if(event.x >= 22 && event.x <= 24) //6
			ch = '6';
		      else if(event.x >= 25 && event.x <= 27) //-
			ch = '-';
		    }
		  else if(event.y == 7) //3rd row of the button panel
		    {
		      if(event.x >= 2 && event.x < 5) //x^y
			ch = '^';   //'^' is keyboard shortcut for x^y
		      else if(event.x >= 6 && event.x < 9) //ln
			ch = 'l';   //'l' is keyboard shortcut for ln
		      else if(event.x >= 10 && event.x < 13) //log
			ch = 'L';   //'L' is keyboard shortcut for log
		      else if(event.x >= 16 && event.x <= 18) //1
			ch = '1';
		      else if(event.x >= 19 && event.x <= 21) //2
			ch = '2';
		      else if(event.x >= 22 && event.x <= 24) //3
			ch = '3';
		      else if(event.x >= 25 && event.x <= 27) //*
			ch = '*';
		    }
		  else if(event.y == 8) //4th row of the button panel
		    {
		      if(event.x >= 2 && event.x < 5) //+/-
			ch = '~';   //'~' is keyboard shortcut for +/-
		      else if(event.x >= 6 && event.x < 9) //(
			ch = '(';
		      else if(event.x >= 10 && event.x < 13) //)
			ch = ')';
		      else if(event.x >= 16 && event.x <= 18) //0
			ch = '0';
		      else if(event.x >= 19 && event.x <= 21) //.
			ch = '.';
		      else if(event.x >= 22 && event.x <= 24) //=
			ch = '=';
		      else if(event.x >= 25 && event.x <= 27) ///(division)
			ch = '/';
		    }
		}
	    }
	}                           //End of mouse inputs
      //Now parse keyboard shortcuts which were either entered by the user or generated by the above code
      if(ch == 27)                  //All clear
	{
	  while(!operandStack.empty())
	    operandStack.pop();
	  while(!operatorStack.empty())
	    operatorStack.pop();
	  operandStack.push(0);
	  errorOccured = false;
	  enteringNumber = true;
	  sawDecimal = false;
	  inverse = false;
	}
      else if(ch == 'o')            //Off
	{
	  while(!operatorStack.empty()) //Empty the stacks
	    operatorStack.pop();
	  while(!operandStack.empty())
	    operandStack.pop();
	  endwin();                 //End curses mode
	  exit(0);                  //Terminate the program
	}
      else if(ch >= '0' && ch <= '9' && !errorOccured) //Numerical input
	{
	  inverse = false;          //Clear inverse bit, as if it was entered, it was clearly an error
	  if(enteringNumber)        //If number was being entered....
	    {
	      if(sawDecimal)        //....And that number had a decimal
		{
		  operandStack.top() += (ch - '0') * pow(static_cast<float>(10), -(++countPostDecimal));                              //Add to that number the entered digit at the corresponding place after the decimal point
		}
	      else                  //Otherwise simply append the digit to the number
		{
		  operandStack.top() *= 10;
		  operandStack.top() += ch - '0';
		}
	    }
	  else                      //Otherwise, this is a new number and the digit is pushed into operandStack
	    {
	      enteringNumber = true;
	      operandStack.push(ch - '0');
	    }
	}
      else if(ch == '.' && !errorOccured) //A decimal point is encountered
	{
	  inverse = false;          //Reset inverse bit
	  if(enteringNumber)        //If a number was being entered, then the decimal point belongs to that number
	    {
	      sawDecimal = true;
	    }
	  else                      //Otherwise, the number is 0.something
	    {
	      operandStack.push(0);
	      enteringNumber = true;
	      sawDecimal = true;
	    }
	}
      else if(ch == '+' && !errorOccured)
	{
	  inverse = false;          //reset inverse bit
	  if(enteringNumber)        //If number was being entered, then pop from the operatorStack till an operator with a lower precedence or an open bracket is encountered, or till the stack is empty
	    {
	      while(!operatorStack.empty() ? ((operatorStack.top() == '+' || operatorStack.top() == '-' || operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^') && operatorStack.top() != '(') : false)
		{
		  tempOperand1 = operandStack.top(); //pop two operands from operandStack
		  operandStack.pop();
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		  if(operatorStack.top() == '+')     //And push the result of the calculation
		    {
		      operandStack.push(tempOperand2 + tempOperand1);
		    }
		  else if(operatorStack.top() == '-')
		    {
		      operandStack.push(tempOperand2 - tempOperand1);
		    }
		  else if(operatorStack.top() == '*')
		    {
		      operandStack.push(tempOperand2 * tempOperand1);
		    }
		  else if(operatorStack.top() == '/')
		    {
		      operandStack.push(tempOperand2 / tempOperand1);
		    }
		  else if(operatorStack.top() == '^')
		    {
		      operatorStack.push(pow(tempOperand2, tempOperand1));
		    }
		  operatorStack.pop();               //Remove last operator from operatorStack
		}
	      operatorStack.push('+');//Push in the operator encountered
	      enteringNumber = false; //Not entering a number now
	      sawDecimal = false;     //So we cannot speak about a decimal point
	    }
	  else                        //If last input was not a number, overwrite the previous operator seen
	    {
	      if(!operatorStack.empty()) operatorStack.top() = '+';
	      else operatorStack.push('+');
	    }
	}
      else if(ch == '-' && !errorOccured)//Same as above
	{
	  inverse = false;
	  if(enteringNumber)
	    {
	      while(!operatorStack.empty() ? ((operatorStack.top() == '+' || operatorStack.top() == '-' || operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^') && operatorStack.top() != '(') : false)    //Everything is the same as above
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		  if(operatorStack.top() == '+')
		    {
		      operandStack.push(tempOperand2 + tempOperand1);
		    }
		  else if(operatorStack.top() == '-')
		    {
		      operandStack.push(tempOperand2 - tempOperand1);
		    }
		  else if(operatorStack.top() == '*')
		    {
		      operandStack.push(tempOperand2 * tempOperand1);
		    }
		  else if(operatorStack.top() == '/')
		    {
		      operandStack.push(tempOperand2 / tempOperand1);
		    }
		  else if(operatorStack.top() == '^')
		    {
		      operatorStack.push(pow(tempOperand2, tempOperand1));
		    }
		  operatorStack.pop();
		}
	      operatorStack.push('-');
	      enteringNumber = false;
	      sawDecimal = false;
	    }
	  else                            //Same as above
	    {
	      if(!operatorStack.empty()) operatorStack.top() = '-';
	      else operatorStack.push('-');
	    }
	}
      else if(ch == '*' && !errorOccured) //Everything remains the same, just operators with higher or equal precedence have decreased.
	{
	  inverse = false;
	  if(enteringNumber)
	    {
	      while(!operatorStack.empty() ? ((operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^') && operatorStack.top() != '(') : false) //Same as above
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		  if(operatorStack.top() == '*')
		    {
		      operandStack.push(tempOperand2 * tempOperand1);
		    }
		  else if(operatorStack.top() == '/')
		    {
		      operandStack.push(tempOperand2 / tempOperand1);
		    }
		  else if(operatorStack.top() == '^')
		    {
		      operatorStack.push(pow(tempOperand2, tempOperand1));
		    }
		  operatorStack.pop();
		}
	      operatorStack.push('*');
	      enteringNumber = false;
	      sawDecimal = false;
	    }
	  else                           //Same as above
	    {
	      if(!operatorStack.empty()) operatorStack.top() = '*';
	      else operatorStack.push('*');
	    }
	}
      else if(ch == '/' && !errorOccured)//no difference
	{
	  inverse = false;
	  if(enteringNumber)
	    {
	      while(!operatorStack.empty() ? ((operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^') && operatorStack.top() != '(') : false)
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		  if(operatorStack.top() == '*')
		    {
		      operandStack.push(tempOperand2 * tempOperand1);
		    }
		  else if(operatorStack.top() == '/')
		    {
		      operandStack.push(tempOperand2 / tempOperand1);
		    }
		  else if(operatorStack.top() == '^')
		    {
		      operatorStack.push(pow(tempOperand2, tempOperand1));
		    }
		  operatorStack.pop();
		}
	      operatorStack.push('/');
	      enteringNumber = false;
	      sawDecimal = false;
	    }
	  else
	    {
	      if(!operatorStack.empty()) operatorStack.top() = '/';
	      else operatorStack.push('/');
	    }
	}
      else if(ch == '~')         //Unary operator, just acts directly on operandStack.top()
	{
	  inverse = false;
	  if(!operandStack.empty())
	    {
	      operandStack.top() *= -1;
	      enteringNumber = false;
	      sawDecimal = false;
	    }
	  else errorOccured = true;
	}
      else if(ch == 'i' && !errorOccured) //inverse key pressed
	{
	  if(!inverse)           //toggle the state of the inverse bit
	    inverse = true;
	  else
	    inverse = false;
	}
      else if(ch == '^' && !errorOccured) //x^y, same as in +, -, *, /, but here, there is no binary operator with higher precedence
	{
	  inverse = false;
	  if(enteringNumber)
	    {
	      while(!operatorStack.empty() ? ((operatorStack.top() == '^') && operatorStack.top() != '(') : false)
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		  if(operatorStack.top() == '^')
		    {
		      operatorStack.push(pow(tempOperand2, tempOperand1));
		    }
		  operatorStack.pop();
		}
	      operatorStack.push('^');
	      enteringNumber = false;
	      sawDecimal = false;
	    }
	  else         //Everything else remains just the same
	    {
	      if(!operatorStack.empty()) operatorStack.top() = '^';
	      else operatorStack.push('^');
	    }
	}
      else if(ch == 's' && !errorOccured) //sine - unary operator, acts directly
	{
	  if(!operandStack.empty())
	    {
	      if(inverse)      //if inverse bit set, then acts as inverse sine
		if(operandStack.top() >= -1 && operandStack.top() <= 1) //Check for domain of inverse sine
		  operandStack.top() = asin(operandStack.top());
		else
		  errorOccured = true;
	      else             //if inverse bit not set, then apply sine
		operandStack.top() = sin(operandStack.top());
	    }
	  else errorOccured = true;
	  inverse = false;     //we can reset these bits now
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == 'c' && !errorOccured) //cosine - Same as for sine
	{
	  if(!operandStack.empty())
	    {
	      if(inverse)
		if(operandStack.top() >= -1 && operandStack.top() <= 1)
		  operandStack.top() = acos(operandStack.top());
		else
		  errorOccured = true;
	      else
		operandStack.top() = cos(operandStack.top());
	    }
	  inverse = false;
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == 't' && !errorOccured) //Same as for sine and cosine, but here we do not have to check for domain for inverse tangent
	{
	  if(!operandStack.empty())
	    {
	      if(inverse)
		operandStack.top() = atan(operandStack.top());
	      else
		operandStack.top() = tan(operandStack.top());
	    }
	  else errorOccured =  true;
	  inverse = false;
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == 'l' && !errorOccured) //ln - natural logarithm - unary operator, hence acts directly
	{
	  if(!operandStack.empty())
	    {
	      if(inverse)            //inverse is the exponential function
		operandStack.top() = exp(operandStack.top());
	      else
		if(operandStack.top() > 0)  //Check for Domain
		  operandStack.top() = log(operandStack.top());
		else
		  errorOccured = true;
	    }
	  else errorOccured = true;
	  inverse = false;           //Reset bits
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == 'L' && !errorOccured) //log - logarithm to base 10 - unary, hence acts directly
	{
	  if(!operandStack.empty())
	    {
	      if(inverse)            //inverse is 10^x
		operandStack.top() = exp(operandStack.top() * log(10));
	      else
		if(operandStack.top() > 0)  //Check for Domain
		  operandStack.top() = log10(operandStack.top());
		else
		  errorOccured = true;
	    }
	  else errorOccured = true;
	  inverse = false;           //Reset bits
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == '(' && !errorOccured) //Open bracket. Simply push it onto operatorStack and reset bits
	{
	  operatorStack.push('(');
	  inverse = false;
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if(ch == ')' && !errorOccured) //Close bracket. Pop from operatorStack each operator, pop from operandStack two operands and push the result into operandStack till an open bracket is encountered
	{
	  while(!operatorStack.empty() ? (operatorStack.top() != '(') : false)
	    {
	      if(!operandStack.empty()) //operandStack being empty is an error. Means that there are unmatched operators and operands. eg: 2 + ) is not a valid expression.
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		}
	      else errorOccured = true;
	      if(!operandStack.empty()) //Ditto
		{
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		}
	      else errorOccured = true;
	      if(operatorStack.top() == '+')  //Evaluate the expressions. Note that if an error has occured, the operation will still continue. However, error will be displayed instead of any number, so this is not an error, merely an annoyance
		operandStack.push(tempOperand2 + tempOperand1);
	      else if(operatorStack.top()== '-')
		operandStack.push(tempOperand2 - tempOperand1);
	      else if(operatorStack.top() == '*')
		operandStack.push(tempOperand2 * tempOperand1);
	      else if(operatorStack.top() == '/')
		operandStack.push(tempOperand2 / tempOperand1);
	      else if(operatorStack.top() == '^')
		operandStack.push(pow(tempOperand2, tempOperand1));
	      operatorStack.pop();
	    }
	  if(!operatorStack.empty())    //If operatorStack is not empty, then the operator left on it is an open bracket. Remove it.
	    operatorStack.pop();
	  inverse = false;              //Reset bits
	  enteringNumber = false;
	  sawDecimal = false;
	}
      else if((ch == '=' || ch == '\n') && !errorOccured) //= key or ENTER key. Means evaluate the expression
	{
	  while(!operatorStack.empty())
	    {
	      if(!operandStack.empty()) //operandStack being empty is an error. Means that there are unmatched operators and operands. eg: 2 + = is not a valid expression.
		{
		  tempOperand1 = operandStack.top();
		  operandStack.pop();
		}
	      else errorOccured = true;
	      if(!operandStack.empty()) //Ditto
		{
		  tempOperand2 = operandStack.top();
		  operandStack.pop();
		}
	      else errorOccured = true;
	      if(operatorStack.top() == '+')  //Evaluate the expressions. Note that if an error has occured, the operation will still continue. However, error will be displayed instead of any number, so this is not an error, merely an annoyance
		operandStack.push(tempOperand2 + tempOperand1);
	      else if(operatorStack.top()== '-')
		operandStack.push(tempOperand2 - tempOperand1);
	      else if(operatorStack.top() == '*')
		operandStack.push(tempOperand2 * tempOperand1);
	      else if(operatorStack.top() == '/')
		operandStack.push(tempOperand2 / tempOperand1);
	      else if(operatorStack.top() == '^')
		operandStack.push(pow(tempOperand2, tempOperand1));
	      operatorStack.pop();
	    }
	  inverse = false;
	  enteringNumber = false;
	  sawDecimal = false;
	}
      //Update Screen buffers....
      if(!errorOccured)
	{
	  mvprintw(2, 2, "                               ");
	  mvprintw(2, 2, "%f", operandStack.top());
	  mvprintw(3, 2, "                               ");
	  if(inverse)
	    mvprintw(3, 2, "inv");
	}
      else
	{
	  mvprintw(2, 2, "                               ");
	  mvprintw(2, 2, "error");
	  mvprintw(3, 2, "                               ");
	}
      refresh(); //....And display them
    }//end of while(true)
}//end of main function
