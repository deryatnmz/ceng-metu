#include "polynomial.h"

int main() {
	
	Polynomial p;
	Polynomial p2(5);

	Polynomial q(p);
	cout << "break-1" << endl;
	Polynomial r = p;
	cout << "break-2" << endl;
	q = p;

	return 0;
}