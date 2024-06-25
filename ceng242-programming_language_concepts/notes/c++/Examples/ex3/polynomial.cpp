#include "polynomial.h"

Polynomial::Polynomial () {
	// put a dummy argument and observe the error due to the call inside main
	degree = 0;
	coeffs = new double[1];
	coeffs[0] = 0;
	cout << "constructor called" << endl;
}

Polynomial::~Polynomial () {
	delete[] coeffs;
	coeffs = NULL;
	cout << "destructor called" << endl;
}

Polynomial::Polynomial(const Polynomial& p) {
	degree = p.degree;
	coeffs = new double[degree+1];
	memcpy(coeffs, p.coeffs, sizeof(double) * (degree+1));
	cout << "copy constructor called" << endl;
}

Polynomial& Polynomial::operator=(const Polynomial& p) {
	if (this!= &p) {
		delete[] coeffs;
		degree = p.degree;
		coeffs = new double[degree+1];
		memcpy(coeffs, p.coeffs, sizeof(double) * (degree+1));
	}
	cout << "assignment operator called" << endl;
	return *this;
}

void Polynomial::setPolynomial(int d, double* c) {
	degree = d;
	delete[] coeffs;
	coeffs = new double[degree+1];
	for (int i=0; i<degree+1; i++)
		coeffs[i] = c[i];
}

/*
Polynomial& Polynomial::operator+ (const Polynomial& p) {
	Polynomial local_p;	// talk about returning reference, yet local variable here, gives warning 
	local_p.degree = p.degree;
	for (int i=0; i<degree+1; i++)
		local_p.coeffs[i] = this->coeffs[i] + p.coeffs[i];
	// local_p dies because it is a local variable
	return local_p;
}
*/

/*
Polynomial& Polynomial::operator+ (const Polynomial& p) {
	// talk about const argument of the method
	for (int i=0; i<degree+1; i++)
		p.coeffs[i] = this->coeffs[i] + p.coeffs[i];
	return p;
}
*/


Polynomial& Polynomial::operator+ (const Polynomial& p) {
	Polynomial* sum = new Polynomial();
	sum->degree = p.degree;
	delete[] sum->coeffs;
	sum->coeffs = new double[degree+1];

	for (int i=0; i<degree+1; i++)
		sum->coeffs[i] = this->coeffs[i] + p.coeffs[i];
	return *sum;
}

void Polynomial::print() const {
	for (int i=0; i<degree; i++)
		cout << coeffs[i] << "x^" << degree-i << " + ";
	cout << coeffs[degree] << endl;
	//degree++;	// show purpose of const
}

const Polynomial & Polynomial::getShadow() { 
	return *this;
}