#include "../../std_lib_facilities.h"

int main()
{
	cout << "Please enter expression (can handle +, -, * and /):\n";
	cout << "add an x to end expression (e.g., 1+2*3x): "; //hacky marker for end of expression
	int lval = 0;
	int rval;
	cin >> lval;
	if (!cin) error("Missing first operand!");
	for (char op; cin >> op;) {//TO DO - review for loop forms. I think this is "for each op that gets extracted from the input stream ....
		if (op != 'x') cin >> rval;//not the end of the expression, so keep reading
		if (!cin) error("There is not a second operand.");
		switch (op) {
		case '+':
			lval += rval;
			break;
		case '-':
			lval -= rval;
			break;
		case '*':
			lval *= rval;
			break;
		case '/':
			lval /= rval;
			break;
		default:
			cout << "Result: " << lval << '\n';
			keep_window_open();
			return 0;
		}
	}

	error("Bad expression!");
}

/* My first user defined type to fix a deficiency in the standard libraries! Yay! Holds a token. Token is an example of a C++ user-defined type. */
//TO DO - Study and understand https://en.cppreference.com/w/cpp/language/constructor
class Token
{
public:
	char kind;
	double value;

	//constructor
	Token(char k) :kind{ k }, value{ 0.0 } {}
	Token(char k, double v) :kind{ k }, value{ v } {}
};

/*Writing functions to implement a grammar*/

/*
The Grammar ........

Expression:
		  Term
		  Expression "+" Term         // addition
		  Expression "–" Term         // subtraction
Term:
		  Primary
		  Term "*" Primary             // multiplication
		  Term "/" Primary              // division
		  Term "%" Primary               // remainder (modulo)
Primary:
		  Number
		   "(" Expression ")"             // grouping
Number:
		  floating-point-literal
*/

Token get_token()
{
	return Token{ '+' };
}

double primary()
{
	return 1;
}

double term()
{
	double left = primary();
	Token t = get_token();

	while (true)
	{
		switch (t.kind)
		{
		case '*':
			left *= term();
			t = get_token();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = get_token();
			break;
		}

		default:
			return left;
		}
	}	
}

double expression() 
{
	double left = term(); //An expression can be just a term, so look for one and get its value
	//this avoids expression calling expression and creating an infinite loop
	Token t = get_token();//get the next token, an operator?
	
	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();
			t = get_token();
			break;
		case '-':
			left -= term();
			t = get_token();
			break;

		default:
			return left;
		}	
		
	}	
}




