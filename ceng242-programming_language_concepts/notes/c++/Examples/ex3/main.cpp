#include "polynomial.h"

int main() {
	
	Polynomial p;
	Polynomial q(p);
	Polynomial r = p;	// copy constructor
	r = p;				// assignment operator
	cout << "break-1" << endl;

	double coeff_p[4] = {1, 2, 3, 4};
	p.setPolynomial(3, coeff_p);
	
	double coeff_q[4] = {5, 6, 7, 8};
	q.setPolynomial(3, coeff_q);

	cout << "break-2" << endl;

	/* The below summation does not give an error 
	   yet results in memory leak, consider on why :)
	*/
	r = p+q;		// r = p.operator+(q);
	r.print();
	//delete &r;	// error because it is a stack variable


	/* The below summation gives neither an error,
	   nor a memory leak, consider on why :)
	*/
	Polynomial& sum = p+q;
	delete &sum; 

	cout << "break-3" << endl;

	q = p.getShadow();					/* this is ok although the returned value is 
										// a reference since it calls assignment operator 
										// and takes a copy of the returned referenced object 
										// */
	
	//Polynomial& q2 = p.getShadow();	/* error because the returned object is a reference and
										// we want to take that reference directly (by using 
										// & operator after Polynomial declaration), yet the
										// returned reference is also a const object whereas
										// q2 is not
										// */

	const Polynomial& q3 = p.getShadow();	// now it is ok

	Polynomial* s = new Polynomial();	// heap object
	delete s;

	cout << "break-4" << endl;
	return 0;
}