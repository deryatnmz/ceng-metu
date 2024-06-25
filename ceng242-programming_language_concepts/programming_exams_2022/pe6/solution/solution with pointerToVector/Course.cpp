#include "Student.h"
#include "Course.h"

// YOU CAN COMPLETELY CHANGE THIS FILE AS YOU NEED. //

/****************************************************/
/****               *** COURSE ***               ****/

/* Constructor : initializes <name> of the object to 
   the given string.
*/
Course::Course(string o_name) : name(o_name), prereqs() {}

/* Destructor  : It is NOT responsible of the 
   destruction of the prerequisite courses and students
   who take the course.
*/
Course::~Course() = default;

/* Copy constructor : It totally applies shallow copy.
*/
Course::Course(const Course& course) = default;

/* This method directly adds (without creating a copy) the 
   given course as a prerequisite of the current course.
*/
void Course::addPrerequisite(const Course& course) {
    prereqs.push_back(&course);
}

/* This method returns the prerequisites of the course.
*/
const vector<const Course*> Course::getPrerequisites() const {
	return prereqs;
}

/* This method returns the name of the course.
*/
string Course::getName() const {
	return name;
}

/* == operator overload : This method returns true if
   the two courses have the same name, false otherwise.
*/
bool Course::operator== (const Course& rhs) const {
	return name == rhs.name;
}

/****************************************************/
/****             *** OPEN COURSE ***            ****/

/* Constructor : initializes the derived members of the
   Course class with the one given as first parameter.
   Also, initializes <term> of the object to the given 
   string. Initializes the <course_index> and the <quota>
   to the given last two integers, respectively.
   
   course_index is the index of the course in the array
   of OpenCourses at Metu.
   
   quota is the upper limit for the number of students
   who takes the course.
*/
OpenCourse::OpenCourse(const Course& course, string o_term, int o_course_index, int o_quota) 
  : Course(course), term(o_term), course_id(o_course_index), quota(o_quota), students()
{
	svptr = &students;
}

/* Destructor  : It is NOT responsible of the
   destruction of the prerequisite courses and students
   who take the course.
*/
OpenCourse::~OpenCourse() = default;

/* Copy constructor : It totally applies shallow copy.
*/

// This is where the sneakiness happens! The copied svptr
// will still refer to the vector of the original OpenCourse.
// See the CourseInstance constructor.
OpenCourse::OpenCourse(const OpenCourse& o) = default;

/* This method returns the ids of the students who take
   the current OpenCourse.
   In order this method to reach the students, it
   is advised to hold the students taking the course
   in a "static" array variable.
*/
const vector<int> OpenCourse::showStudentList() const {
	vector<int> v;
	for (const auto &s : students)
	    v.push_back(s->getId());
	return v;
}

/* This method calls the gradeCourse() method to grade
   the current OpenCourse for each student who take
   the course.
   In order this method to reach the students, it
   is advised to hold the students taking the course
   in a "static" array variable.
*/
void OpenCourse::finalize() {
    for (auto &student : students)
        student->gradeCourse(*this);
}


/****************************************************/
/****          *** COURSE INSTANCE ***           ****/

/* Constructor : initializes the derived members of the
   OpenCourse class with the one given as first parameter.
   Also, holds the information that the current 
   CourseInstance object is owned by which student, by
   using the second parameter.
   HINT: For the related OpenCourse object to reach the
   entire student list, it is recommended to store the
   given student in a "static" array. This way of
   implementation may be beneficial in the other methods 
   later.
   This method initializes the grade corresponding to
   this CourseInstance to Grade::NA.
*/

// Alright, this is where the cheese happens. Ideally, I would want to push_back
// the student to the original opencourse object's vector, but I can't do that
// since it is not public. Try this code:
// const_cast<OpenCourse &>(opencourse).students.push_back(o_student);
// Won't work since CourseInstance can only access ITS OWN protected members.
// Not those of another OpenCourse instance. Anyway, therefore I use a trick
// in which the pointer to the original opencourse's vector is copied in
// the copy constructor. Then I can use CourseInstance's own svptr to push
// back the student into the given opencourse object's list (not our own!).
CourseInstance::CourseInstance(const OpenCourse& opencourse, Student& o_student) 
  : OpenCourse(opencourse), student(o_student), grade(NA)
{
    svptr->push_back(&o_student);
}

/* Destructor  : It is NOT responsible of the
   destruction of the prerequisite courses and students
   who take the course.
*/
CourseInstance::~CourseInstance() = default;

/* This method saves the given grade value into the
   corresponding class member.
*/
void CourseInstance::setGrade(enum Grade o_grade) {
	grade = o_grade;
}

/* This method returns the grade of the CourseInstance.
*/
enum Grade CourseInstance::getGrade() const {
	return grade;
}