#include <iostream>
#include <cstring>

using namespace std;

class Polynomial {
	int degree;
	double* coeffs;

public:
	Polynomial();								// constructor-1
	Polynomial(int);							// constructor-2
	~Polynomial();								// destructor
	Polynomial(const Polynomial&);				// copy constructor
	Polynomial& operator=(const Polynomial&);	// assignment operator (also an example for operator overloading)
};