#include "../../std_lib_facilities.h"

int main()
{
	cout << "Please enter expression (can handle + and -):";
	int lval = 0;
	int rval;
	char op;
	int res = 0;//did this to stop compiler warning about uninitialized variable
	cin >> lval >> op >> rval; //reads in something like 1 + 3

	if (op == '+')
		res = lval + rval;
	else if (op == '-')
		res = lval - rval;

	cout << "The result is: " << res << endl;
	return 0; //TO DO why? what does this do?
}