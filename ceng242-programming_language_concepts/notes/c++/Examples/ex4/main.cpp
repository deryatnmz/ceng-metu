#include "virus.h"

int main() {
	
	Virus* virusA = new A(5, "AUUSG");
	Virus* virusB = new B(8, "SSUAUGAG");
	//Virus C(4, "ATSG");					// show the error
	//Virus* ptr = new Virus(4, "ATSG");	// show the error
	Virus* ptr = new A(4, "ATSG");			// but this one is ok
	delete ptr;								// pay attention to which destructor called only
											//     when you put 'virtual' to Virus' destructor, the things are changed. 
	vector<Virus*> viruses;
	viruses.push_back(virusA); 
	viruses.push_back(virusB);

	for (int i=0; i<viruses.size(); i++) {
		viruses[i]->mutate();
		viruses[i]->printRNA();
	}

	for (int i=0; i<viruses.size(); i++) {
		delete viruses[i];	// pay attention to which destructor called only
		viruses[i] = NULL;
	}

/*
	the above part does the same job with the below part
	virusA->mutate();
	virusA->printRNA();

	virusB->mutate();
	virusB->printRNA();

	delete virusA;						
	virusA = NULL;							
	delete virusB;							
	virusB = NULL;

*/
	cout << "observe the below construction/destruction:" << endl;
	A* new_virusA = new A(12, "AUUAGSASSUGS");
	delete new_virusA;		// pay attention to which destructors called

	return 0;
}