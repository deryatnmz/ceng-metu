# include <iostream>

using namespace std;

class parent {
	public:
		void printclass() {
		printf("p ");
	};

	virtual void printvirtual() {
		printf("p ");
	};
};

class child : public parent {
	public:
		void printclass() {
			printf("c ");
		};

		void printOwnFunction() {
			printf("c ");
		};

		virtual void printvirtual() {
			printf("c ");
		};
};

int main() {

	parent p; child c; parent *q;
	p.printclass();
	p.printvirtual();
	cout << "break-1" << endl;
	
	c.printclass();
	c.printvirtual();
	c.printOwnFunction();
	cout << "break-2" << endl;
	
	q = &p;
	q->printclass();
	q->printvirtual();
	cout << "break-3" << endl;
	
	q = &c;
	q->printclass();
	q->printvirtual();
	//q->printOwnFunction();	// error
	dynamic_cast<child*>(q)->printOwnFunction();	// not to see error, we have to call by casting

	return 0;
}
//output: p p c c c p p p c c
