#ifndef __STUDENT_H__
#define __STUDENT_H__


#include "Course.h"

//////////////////////////////////////////////////////
//    DO NOT CHANGE *PUBLIC PARTS* IN THIS FILE!    //
//////////////////////////////////////////////////////

enum Grade learnGrade(string course_name, int student_id);

/****************************************************/
/****               *** STUDENT ***              ****/
class Student {

protected:
	const int id;
	const string fullname;
	const string department;
	float gpa;

	int countedCourses;	// just to be used by gradeCourse
	bool upgraded;	// just to be used by destructor

	vector<CourseInstance*> courses;

public:
	Student(int id, string fullname, string department);
	~Student();
	Student(const Student&);
	int getId() const;
	float getGPA() const;
	vector<const CourseInstance*> listCourses() const;
	void gradeCourse(const OpenCourse& openCourse);
	void setUpgradeStatus();

};

/****************************************************/
/****              *** FRESHMAN ***              ****/
class Freshman : public Student {

protected:
	

public:
	Freshman(int id, string fullname, string department);
	~Freshman();
	Freshman(const Freshman&);
	bool addCourse(const OpenCourse&);
	void printTranscript() const;	// Do not confuse. This will be implemented by the PE admin.

};

/****************************************************/
/****              *** SOPHOMORE ***             ****/
class Sophomore : public Freshman {

protected:
	bool internship1;
	bool internship2;

public:
	Sophomore(int id, string fullname, string department);
	Sophomore(const Freshman&);
	~Sophomore();
	Sophomore(const Sophomore&);
	void doAnInternship(int);
	bool getInternshipStatus(int);
	void printTranscript() const;	// Do not confuse. This will be implemented by the PE admin.
};

/****************************************************/
/****                *** JUNIOR ***              ****/
class Junior : public Sophomore {

public:
	Junior(int, string, string);
	Junior(const Sophomore&);
	~Junior();
	Junior(const Junior&);
	void selectElectiveCourse();
	void printTranscript() const;	// Do not confuse. This will be implemented by the PE admin.
};

/****************************************************/
/****                *** SENIOR ***              ****/
class Senior : public Junior {

public:
	Senior(int, string, string);
	Senior(const Junior&);
	~Senior();
	Senior(const Senior&);
	bool graduate();
	void printTranscript() const;	// Do not confuse. This will be implemented by the PE admin.
};

#endif

