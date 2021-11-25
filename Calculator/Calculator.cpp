#include "../../std_lib_facilities.h"

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

/*I got this code from https://stroustrup.com/Programming/calculator00.cpp */
Token get_token()    // read a token from cin
{
	char ch;
	cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.) It extracts the next charactor from input and assigns it to ch

	switch (ch) {
		//not yet   case ';':    // for "print"
		//not yet   case 'q':    // for "quit"
	case '(': case ')': case '+': case '-': case '*': case '/':
		return Token(ch);        // let each character represent itself, not a number so k.value uses default 0.0
	case '.': //do nothing
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9'://see a digit
	{
		cin.putback(ch);         // put digit back into the input stream. Skipping until see a whole floating-point number?
		double val;
		cin >> val;              // read a floating-point number
		return Token('8', val);   // let '8' represent "a number"
	}
	default:
		error("Bad token");
		return Token('Z');//TO DO Take this out! It's a hack to get this to compile. Why does Stroustrup's code at the URL above not build?
	}
}

class Token_stream {
public:
	Token get();            // get a Token
	void putback(Token t);  // put a Token back
private:
	bool full{ false };      // is there a Token in the buffer?
	Token buffer = { '0' };           // where we store a 'putback' Token
};

void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer = t;         // copy t to buffer
	full = true;        // buffer is now full
}

Token Token_stream::get()
{
	if (full) {
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;

	switch (ch) {
	case ';':       // for "print"
	case 'q':       // for "quit"
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
		return Token{ ch };   // let each character represent itself
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		cin.putback(ch);
		double val;
		cin >> val;
		return Token{ '8', val };
	}
	default:
		error("Bad Token");
	}
}

Token_stream ts; //provides get() and putback()

double expression();

double term();

int main()
{
	try {
		while (cin)
			cout << "=" << expression() << '\n';
		keep_window_open();
	}
	catch (exception& e) {
		cerr << e.what() << '\n';
		keep_window_open();
		return 1;//error return value
	}
	catch (...) {
		cerr << "exception \n";
		keep_window_open();
		return 2;//error return value
	}
}



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



/*Primary:
Number
"(" Expression ")"             // grouping
*/

double primary()
{
	Token t = get_token();
	switch (t.kind)
	{
	case'(':  //handle "(" Expression ")"
		{	double d = expression();
		t = ts.get();// get_token();
		if (t.kind != ')') error("')' expected");
		return d;
		}

	case '8':
		return t.value;

	default:
		error("primary expected");
	}
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
			t = ts.get();// get_token();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();// get_token();
			break;
		}

		default:
			ts.putback(t); 
			return left;
		}
	}	
}

double expression() 
{
	double left = term(); //An expression can be just a term, so look for one and get its value
	//this avoids expression calling expression and creating an infinite loop
	Token t = ts.get(); // get_token();//get the next token, an operator?
	
	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();
			t = ts.get();// get_token();
			break;
		case '-':
			left -= term();
			t = ts.get();// get_token();
			break;

		default:
			ts.putback(t);
			return left;
		}	
		
	}	
}




