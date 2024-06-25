#include "Student.h"

// YOU CAN COMPLETELY CHANGE THIS FILE AS YOU NEED. //

/****************************************************/
/****               *** STUDENT ***              ****/

/* Constructor : initializes <id> of the object to
   the given integer, and <fullname> and <department>
   to the given strings, respectively.
   Also, initalizes the <gpa> of the object to 100.0.
   Also, it is recommended to hold a bool variable to
   carry the upgrade information and initalize it to
   false. This may be needed in the other methods
   later on.
*/
Student::Student(int _id, string _fullname, string _department)
  : id(_id), name(_fullname), dept(_department), gpa(0.0), courses(),
    grade_sum(0.0), grade_cnt(0)
{}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current student.
*/
Student::~Student() = default;

/* Copy constructor : It totally applies shallow copy.
*/
Student::Student(const Student& student) = default;

/* This method returns the id of the student.
*/
int Student::getId() const {
	return id;
}

/* This method returns the GPA of the student.
*/
float Student::getGPA() const {
	return gpa;
}

/* This method returns the courses taken by
   this student. Note that the grade taken in that
   course is not important. You should return
   all the passed and failed courses.
*/
vector<const CourseInstance*> Student::listCourses() const {
    vector<const CourseInstance *> v;
    for (auto &ci : courses)
        v.push_back(&ci);
    return v;
}

/* This method grades the given course for this student.
   In order to grade the course, learnGrade() method is 
   called. learnGrade() method is already implemented by 
   the HW admin, yet it is invisible to you. You call it
   by giving the course name as the first argument and
   student id as the second argument. For instance;
       learnGrade("Programming Languages", 1234567);
   Then, you should save the grade returned by learnGrade().
   
   Also, after learning the grade, you should recalculate
   the GPA of the student in this method.
   GPA is a float value calculated by simply taking the 
   average of all the graded courses (The courses for which
   gradeCourse() method is called before or now).
*/
void Student::gradeCourse(const OpenCourse& openCourse) {
    // Cross fingers to not get same course again!
    grade_sum += learnGrade(openCourse.getName(), id);
    grade_cnt++;
    gpa = grade_sum / grade_cnt;
}

/* This method sets the upgrade information for the current
   student to true. You may not need this method, yet still
   it is defined in case that it may be beneficial later.
*/
void Student::setUpgradeStatus() {} // I don't use this


/****************************************************/
/****              *** FRESHMAN ***              ****/

/* Constructor : initializes <id> of the object to
   the given integer, and <fullname> and <department>
   to the given strings, respectively.
   Also, initalizes the <gpa> of the object to 100.0.
   Also, it is recommended to hold a bool variable to 
   carry the upgrade information and initalize it to 
   false. This may be needed in the other methods 
   later on.
*/
Freshman::Freshman(int id, string fullname, string department) 
  : Student(id, fullname, department)
{
	
}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current freshman.
*/
Freshman::~Freshman() = default;

/* Copy constructor : It totally applies shallow copy.
*/
Freshman::Freshman(const Freshman& freshman) = default;

/* This method tries to add the given opencourse.
   In order to add, it just checks the prerequisites of the
   course is satisfied ot not. If all the prerequisites are
   satisfied (the prerequisite courses had been taken before),
   then it adds the course by constructiong a new CourseInstance
   object by using given OpenCourse object, then the newly
   created CourseInstance should be stored in an array of taken
   courses.
   If the course could be added successfuly, the method returns
   true. Otherwise (if there is at least one non-satisfied 
   prerequisite), it returns false.
*/
bool Freshman::addCourse(const OpenCourse& opencourse) {
    // Standard search double-loop. May want to use a set/hash table IRL.
    for (const auto &prereq : opencourse.getPrerequisites()) {
        bool taken = false;
        for (const auto &inst : courses) {
            if (*prereq == inst) {
                taken = true;
                break;
            }
        }
        if (!taken)
            return false;
    }
    courses.emplace_back(opencourse, *this); // Add course instance
    // Important to add grade for the course instance
    courses.back().setGrade(learnGrade(opencourse.getName(), id)); 
    return true;
}

/****************************************************/
/****              *** SOPHOMORE ***             ****/

/* Constructor : initializes <id> of the object to
   the given integer, and <fullname> and <department>
   to the given strings, respectively.
   Also, initalizes the <gpa> of the object to 100.0.
   Also, it is recommended to hold a bool variable to
   carry the upgrade information and initalize it to
   false. This may be needed in the other methods
   later on.
   Also, this class should hold two bool variables to
   store the information about the completion of 
   INTERNSHIP-1 and INTERNSHIP-2. Constructor should
   initialize those variables to false.
*/
Sophomore::Sophomore(int id, string fullname, string department)
  : Freshman(id, fullname, department), int1(false), int2(false)
{}

/* Constructor : initializes the derived members of the
   Freshman class with the one given as first parameter.
   Also, this class should hold two bool variables to
   store the information about the completion of 
   INTERNSHIP-1 and INTERNSHIP-2. Constructor should
   initialize those variables to false.
 */
Sophomore::Sophomore(const Freshman& freshman)
  : Freshman(freshman), int1(false), int2(false)
{}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current sophomore.
*/
Sophomore::~Sophomore() = default;

/* Copy constructor : It totally applies shallow copy.
*/
Sophomore::Sophomore(const Sophomore& sophomore) = default;

/* This method grades the internship with the given id
   (1 or 2).
   In order to grade the internship, learnGrade() method 
   is called. learnGrade() method is already implemented 
   by the HW admin, yet it is invisible to you. You call 
   it by giving the internship name as the first argument 
   and student id as the second argument. For instance;
       learnGrade("INTERNSHIP-1", 1234567); or
	   learnGrade("INTERNSHIP-2", 1234567);
   If the grade returned by learnGrade() method is greater
   than or equal to DD, then restore the value of the 
   corresponding internship variable as true, else leave it
   as false.
*/
void Sophomore::doAnInternship(int internship_no) {
    // Forget error checking
	if (internship_no == 1) {
	    if (learnGrade("INTERNSHIP-1", id) >= DD)
	        int1 = true;
	} else {
	    if (learnGrade("INTERNSHIP-2", id) >= DD)
	        int2 = true;
	}
}

/* This method returns the status of the internship with
   the given id (1 or 2). The status is the fact that 
   it is completed successfully or not (true or false, 
   respectively).
*/
bool Sophomore::getInternshipStatus(int internship_no) {
	if (internship_no == 1)
	    return int1;
	else
	    return int2;
}

/****************************************************/
/*                                                  */
/*           IT IS ENOUGH TO IMPLEMENT              */
/*    JUST CONSTRUCTORs, DESTRUCTOR & COPY CONS.    */
/*        FOR THE CLASS JUNIOR and SENIOR           */
/*     SIMILAR TO CLASS FRESHMAN and SOPHOMORE.     */
/*                                                  */
/*     JUNIOR and SENIOR OBJECTS MAY BE DEFINED     */
/*     IN TESTS, YET selectElectiveCourse() and     */ 
/*        graduate() METHOD WILL NOT BE USED.       */
/*                                                  */
/****************************************************/


/****************************************************/
/****                *** JUNIOR ***              ****/

Junior::Junior(int id, string fullname, string department)
  : Sophomore(id, fullname, department) {}

Junior::Junior(const Sophomore& sophomore) : Sophomore(sophomore) {}

Junior::~Junior() = default;

Junior::Junior(const Junior& junior) = default;

void Junior::selectElectiveCourse() {
	// LEAVE EMPTY
}

/****************************************************/
/****                *** SENIOR ***              ****/

Senior::Senior(int id, string fullname, string department) 
  : Junior(id, fullname, department) {}

Senior::Senior(const Junior& junior) : Junior(junior) {}

Senior::~Senior() = default;

Senior::Senior(const Senior& senior) = default;

bool Senior::graduate() {
	// LEAVE EMPTY
	return true;
}

