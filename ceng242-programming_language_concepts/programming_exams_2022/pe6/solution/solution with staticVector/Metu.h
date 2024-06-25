#ifndef __METU_H__
#define __METU_H__


#include "Course.h"
#include "Student.h"

//////////////////////////////////////////////////////
//    DO NOT CHANGE *PUBLIC PARTS* IN THIS FILE!    //
//////////////////////////////////////////////////////

/****************************************************/
/****                *** METU ***                ****/

class Metu {

	vector<const Course*> courses;
	vector<OpenCourse*> openCourses;
	vector<Student*> students;
	vector<int**> touchins;

	// extra members
	void upgradeStudentHelper(Student* oldStudent, Student* newStudent);

	int row_size, column_size;
	struct triple {
		int t = -1;
		int i = 0;
		int j = 0;
	};

	struct triple* findStudentInTouchins(int, int);
	struct triple* addNewTouchin(triple*, triple*, int, int, string);
	void combine2TouchinsIntoNewOne(triple*, triple*, string, int*);
	int* computeShifts(triple*, triple*, string);
	void constructNewTouchin();
	void destructTouchin(int);

public:
	Metu();
	~Metu();

	Student& getStudent(int);
	void registerStudent(Student*);
	void registerCourse(const Course&);
	OpenCourse& openCourse(const Course&, string, int, vector<Freshman*>, vector<Sophomore*>, vector<Junior*>, vector<Senior*>);

	Sophomore* upgradeStudent(Freshman&);
	Junior* upgradeStudent(Sophomore&);
	Senior* upgradeStudent(Junior&);

	void setClassroomSize(int, int);
	void addCheatInfo(int, int, string);
	void printSeatingPlan();

};

#endif
