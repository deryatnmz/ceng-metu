#include <iostream>
#include <cstring>

using namespace std;

class Polynomial {
	int degree;
	double* coeffs;

public:
	Polynomial();								// constructor
	~Polynomial();								// destructor
	Polynomial(const Polynomial&);				// copy constructor
	Polynomial& operator=(const Polynomial&);	// assignment operator
	
	void setPolynomial(int, double*);
	Polynomial& operator+(const Polynomial&);
	void print() const;
	const Polynomial & getShadow();
};