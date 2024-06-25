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
Student::Student(int _id, string _fullname, string _department) {
	// TODO
}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current student.
*/
Student::~Student() {
	// TODO
}

/* Copy constructor : It totally applies shallow copy.
*/
Student::Student(const Student& student) {
	// TODO
}

/* This method returns the id of the student.
*/
int Student::getId() const {
	// TODO
}

/* This method returns the GPA of the student.
*/
float Student::getGPA() const {
	// TODO
}

/* This method returns the courses taken by
   this student. Note that the grade taken in that
   course is not important. You should return
   all the passed and failed courses.
*/
vector<const CourseInstance*> Student::listCourses() const {
	// TODO
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
	// TODO
}

/* This method sets the upgrade information for the current
   student to true. You may not need this method, yet still
   it is defined in case that it may be beneficial later.
*/
void Student::setUpgradeStatus() {
	// TODO
}


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
Freshman::Freshman(int id, string fullname, string department) {
	// TODO
}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current freshman.
*/
Freshman::~Freshman() {
	// TODO
}

/* Copy constructor : It totally applies shallow copy.
*/
Freshman::Freshman(const Freshman& freshman) {
	// TODO
}

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
	// TODO
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
Sophomore::Sophomore(int id, string fullname, string department) {
	// TODO
}

/* Constructor : initializes the derived members of the
   Freshman class with the one given as first parameter.
   Also, this class should hold two bool variables to
   store the information about the completion of 
   INTERNSHIP-1 and INTERNSHIP-2. Constructor should
   initialize those variables to false.
 */
Sophomore::Sophomore(const Freshman& freshman) {
	// TODO
}

/* Destructor  : It is responsible of the destruction of
   the CourseInstance objects owned by the current sophomore.
*/
Sophomore::~Sophomore() {
	// TODO
}

/* Copy constructor : It totally applies shallow copy.
*/
Sophomore::Sophomore(const Sophomore& sophomore) {
	// TODO
}

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
void Sophomore::doAnInternship(int intership_no) {
	// TODO
}

/* This method returns the status of the internship with
   the given id (1 or 2). The status is the fact that 
   it is completed successfully or not (true or false, 
   respectively).
*/
bool Sophomore::getInternshipStatus(int intership_no) {
	// TODO
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

Junior::Junior(int id, string fullname, string department) {
	// TODO
}

Junior::Junior(const Sophomore& sophomore) {
	// TODO
}

Junior::~Junior() {
	// TODO
}

Junior::Junior(const Junior& junior) {
	// TODO
}

void Junior::selectElectiveCourse() {
	// LEAVE EMPTY
}

/****************************************************/
/****                *** SENIOR ***              ****/

Senior::Senior(int id, string fullname, string department) {
	// TODO
}

Senior::Senior(const Junior& junior) {
	// TODO
}

Senior::~Senior() {
	// TODO
}

Senior::Senior(const Senior& senior) {
	// TODO
}

bool Senior::graduate() {
	// LEAVE EMPTY
	return true;
}

