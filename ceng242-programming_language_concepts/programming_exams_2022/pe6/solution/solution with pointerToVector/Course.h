#ifndef __COURSE_H__
#define __COURSE_H__


#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Student;

//////////////////////////////////////////////////////
//    DO NOT CHANGE *PUBLIC PARTS* IN THIS FILE!    //
//////////////////////////////////////////////////////

enum Grade {
	AA = 100,
	BA = 85,
	BB = 80,
	CB = 75,
	CC = 70,
	DC = 65,
	DD = 60,
	FF = 1,
	NA = 0
};

/****************************************************/
/****               *** COURSE ***               ****/
class Course {

private:

	// You can add your own variables/methods here.
    
protected:
    std::string name;
    vector<const Course *> prereqs;
	
	// You can add your own variables/methods here.

public:

	// Do NOT change the below part.
	Course(string);
	~Course();
	Course(const Course&);

	void addPrerequisite(const Course&);
	const vector<const Course*> getPrerequisites() const;
	string getName() const;
	bool operator== (const Course&) const;

};


/****************************************************/
/****             *** OPEN COURSE ***            ****/
class OpenCourse : public Course {

private:

	// You can add your own variables/methods here.
	std::string term;
    int course_id;
    int quota;

protected:
	
	// You can add your own variables/methods here.
    vector<Student *> students;
    
    // A pointer for cheating the vector in the copy constructor
    // I use this to handle the CourseInstance constructor with
    // no public members, const_cast and avoiding a static vector.
    // Ugly, but tricks can be fun as long as it's not real code.
    // See the CourseInstance constructor.
    vector<Student *> *svptr; 

public:

	// Do NOT change the below part.
	OpenCourse(const Course&, string, int, int);
	~OpenCourse();
	OpenCourse(const OpenCourse&);
	const vector<int> showStudentList() const;
	void finalize();

};


/****************************************************/
/****          *** COURSE INSTANCE ***           ****/
class CourseInstance : public OpenCourse {

private:

	// You can add your own variables/methods here.
    Student &student;
    enum Grade grade;

protected:
	
	// You can add your own variables/methods here.

public:

	// Do NOT change the below part.
	CourseInstance(const OpenCourse&, Student&);
	~CourseInstance();
	void setGrade(enum Grade);
	enum Grade getGrade() const;

};

#endif

