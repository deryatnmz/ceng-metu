#include "polynomial.h"

Polynomial::Polynomial () {
	// put a dummy argument and observe the error due to the call inside main
	degree = 0;
	coeffs = new double[1];
	coeffs[0] = 0;
	cout << "first constructor called" << endl;
}

Polynomial::Polynomial(int degree) {
	this->degree = degree;
	coeffs = new double[degree];
	for(int i=0; i<degree; i++)
		coeffs[i] = 0;

	cout << "second constructor called" << endl;
}

Polynomial::~Polynomial () {
	delete[] coeffs;
	cout << "destructor called" << endl;
}

Polynomial::Polynomial(const Polynomial& p) {
	degree = p.degree;
	coeffs = new double[degree];
	memcpy(coeffs, p.coeffs, sizeof(double) * degree);
	cout << "copy constructor called" << endl;
}

Polynomial& Polynomial::operator=(const Polynomial& p) {
	if (this!= &p) {
		delete[] coeffs;
		degree = p.degree;
		coeffs = new double[degree];
		memcpy(coeffs, p.coeffs, sizeof(double) * degree);
	}
	cout << "assignment operator called" << endl;
	return *this;
}
