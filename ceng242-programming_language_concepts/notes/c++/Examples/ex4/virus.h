#ifndef VIRUS_H
#define VIRUS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Virus {

protected:
	int rna_length;
	string RNA;

public:
	Virus(int length, string rna) {
		rna_length = length;
		RNA = rna;
		cout << "abstract base class' constructor called!" << endl;
	}

	~Virus() {cout << "abstact base class' destructor called!" << endl;}

	void printRNA() const {
		cout << RNA << endl;
	}

	virtual void mutate() = 0;

};

class A : public Virus {

public:
	A(int length, string rna) : Virus(length, rna) {cout << "A's constructor called!" << endl;};
	~A() {cout << "A's destructor called!" << endl;}

	void mutate() {
		string new_rna = "";
		for (int i=0; i < rna_length; i+=2)
			new_rna = new_rna + RNA.at(i);
		RNA = new_rna;
	}
};

class B : public Virus {

public:
	B(int length, string rna) : Virus(length, rna) {cout << "B's constructor called!" << endl;};
	~B() {cout << "B's destructor called!" << endl;}

	void mutate() {
		string new_rna;
		for (int i=0; i<rna_length; i++)
			if (RNA.at(i) == 'U')
				new_rna = new_rna + 'A';
			else if (RNA.at(i) == 'A')
				new_rna = new_rna + 'U';
			else
				new_rna = new_rna + RNA.at(i);
		RNA = new_rna;
	}
};

#endif