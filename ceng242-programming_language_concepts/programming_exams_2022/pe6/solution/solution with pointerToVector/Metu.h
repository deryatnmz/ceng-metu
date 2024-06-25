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

private:

	// You can add your own variables/methods here.
    vector<Student *> students;
    vector<const Course *> courses;
    vector<OpenCourse *> open_courses; // To store courses we openCourse
    
    // For the cheating stuff
    int rows, cols;
    
    struct CInfo {
        int id1, id2;
        int ioff, joff;
    };
    
    vector<CInfo> cinfo;
    
    // Easy upgrade function template
    template<typename U, typename C>
    U *upgrade(C &);
    
    // Easy template for adding courses for different students
    template <typename S>
    void addStudents(const OpenCourse &c, int *quota, vector<S *> students);

public:

	// Do NOT change the below part.
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
