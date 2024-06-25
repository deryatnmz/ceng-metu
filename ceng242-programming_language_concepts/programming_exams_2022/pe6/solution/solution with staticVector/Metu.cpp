#include "Metu.h"

/****************************************************/
/****                *** METU ***                ****/

/* Constructor : Initialize your own variables if you
   need.
*/
Metu::Metu() {

}

/* Destructor  : It is responsible of the destruction of
   the Course and OpenCourse objects and the registered
   students.
*/
Metu::~Metu() {

	for (unsigned int i = 0; i < courses.size(); i++) {
		delete courses[i];
		courses[i] = NULL;
	}
	courses.clear();

	for (unsigned int i = 0; i < openCourses.size(); i++) {
		delete openCourses[i];
		openCourses[i] = NULL;
	}
	openCourses.clear();

	for (unsigned int i = 0; i < students.size(); i++) {
		if (students[i])
			delete students[i];
		students[i] = NULL;
	}
	students.clear();

	for (unsigned int i = 0; i < touchins.size(); i++) {
		if (touchins[i])
			destructTouchin(i);
		touchins[i] = NULL;
	}
	touchins.clear();

}

/* This method returns the student whose id is given
   in the argument. You need to find the corresponding
   student among the registered students.
*/
Student& Metu::getStudent(int id) {
	
	for (unsigned int i = 0; i < students.size(); i++)
		if (students[i]->getId() == id)
			return *students[i];
}

/* This method directly adds (without creating a copy)
   the given student to the list of registered students.
*/
void Metu::registerStudent(Student* student) {
	
	students.push_back(student);
}

/* This method directly adds (without creating a copy)
   the given course to the list of base course objects
   (not among the derived ones).
*/
void Metu::registerCourse(const Course& course) {

	courses.push_back(&course);
}

/* This method constructs a new OpenCourse object by deriving
   from the Course object given in the first argument and
   using the <term> and <quota> values given in the second
   and third arguments, respectively.
   Also, for each student given in the corresponding lists as
   function arguments, the newly created OpenCourse is tried 
   to be added in order.
   Note that the priority order is given from greater to less
   as follows: Seniors > Juniors > Sophomores > Freshmans.
   After the priority ordering, you should try to add the
   course for each student of the same type in the order inside
   the corresponding list.
   Also, during the course addition, you should take the course
   quota in consider.
   In the end, there may be left the students who could not add
   the course because of the quota or the course prerequisite.
   Finally, you should return the created OpenCourse object.
*/
OpenCourse& Metu::openCourse(const Course& course, string term, int quota, 
							vector<Freshman*> freshmans, vector<Sophomore*> sophomores, vector<Junior*> juniors, vector<Senior*> seniors) {

	OpenCourse* opencourse = new OpenCourse(course, term, openCourses.size(), quota);
	openCourses.push_back(opencourse);

	int population = 0;
	for (int i = 0; i < seniors.size(); i++) {
		if (population == quota)
			break;
		if(seniors[i]->addCourse(*opencourse))
			population++;
	}
	for (int i = 0; i < juniors.size(); i++) {
		if (population == quota)
			break;
		if (juniors[i]->addCourse(*opencourse))
			population++;
	}
	for (int i = 0; i < sophomores.size(); i++) {
		if (population == quota)
			break;
		if (sophomores[i]->addCourse(*opencourse))
			population++;
	}
	for (int i = 0; i < freshmans.size(); i++) {
		if (population == quota)
			break;
		if (freshmans[i]->addCourse(*opencourse))
			population++;
	}

	return *opencourse;
}

/* This method upgrades the given Freshman object to
   the Sophomore status by constructing a new Sophomore
   object with the properties of the given Freshman.
   You should delete the given Freshman and place the 
   newly created Sophomore object to its location in the 
   students array of Metu object.
   In the end, you should return the created Sophomore.
*/
Sophomore* Metu::upgradeStudent(Freshman& student) {

	Sophomore* upgradedStudent = new Sophomore(student);
	upgradeStudentHelper(&student, upgradedStudent);
	return upgradedStudent;
}

/* This method upgrades the given Sophomore object to
   the Junior status by constructing a new Junior
   object with the properties of the given Sophomore.
   You should delete the given Sophomore and place the
   newly created Junior object to its location in the
   students array of Metu object.
   In the end, you should return the created Junior.
*/
Junior* Metu::upgradeStudent(Sophomore& student) {

	Junior* upgradedStudent = new Junior(student);
	upgradeStudentHelper(&student, upgradedStudent);
	return upgradedStudent;
}

/* This method upgrades the given Junior object to
   the Senior status by constructing a new Senior
   object with the properties of the given Junior.
   You should delete the given Junior and place the
   newly created Senior object to its location in the
   students array of Metu object.
   In the end, you should return the created Senior.
*/
Senior* Metu::upgradeStudent(Junior& student) {

	Senior* upgradedStudent = new Senior(student);
	upgradeStudentHelper(&student, upgradedStudent);
	return upgradedStudent;
}

/* My own method.
*/
void Metu::upgradeStudentHelper(Student* oldStudent, Student* newStudent) {

	for (unsigned int i = 0; i < students.size(); i++)
		if (students[i]->getId() == oldStudent->getId()) {
			students[i] = newStudent;
			break;
		}

	oldStudent->setUpgradeStatus();
	delete oldStudent;
	oldStudent = NULL;
}

/* This method sets the number of rows and number of columns in a
   standard Metu classroom with the given arguments, respectively.
*/
void Metu::setClassroomSize(int row_size, int column_size) {

	this->row_size = row_size;
	this->column_size = column_size;
}

/* This method tries to construct the 2D seating plan by using
   the given information as function argument one-by-one.
   The information consists of 2 integers and 1 string argument.
   The string parameter can be either "|" or "-".
   If it is "|", then it means location of the student whose id 
   given in the first argument is <row_id, column_id> whereas
   location of the student whose id given in the second argument
   is <row_id + 1, column_id>.
   If it is "-", then it means location of the student whose id 
   given in the first argument is <row_id, column_id> whereas
   location of the student whose id given in the second argument
   is <row_id, column_id + 1>.
*/
void Metu::addCheatInfo(int from_student_id, int to_student_id, string dir) {

	// find the students in the school list
	Student* from_student = & getStudent(from_student_id);
	Student* to_student = & getStudent(to_student_id);

	// Find both students' indices in touch-in groups
	struct triple* index_f = NULL;
	struct triple* index_t = NULL;
	for (unsigned int t = 0; t < touchins.size(); t++) {
		if (index_f == NULL)
			index_f = findStudentInTouchins(from_student_id, t);
		if (index_t == NULL)
			index_t = findStudentInTouchins(to_student_id, t);
		if (index_f && index_t)
			break;
	}

	// Set the sitting plan
	if (index_f == NULL && index_t == NULL)
		addNewTouchin(index_f, index_t, from_student_id, to_student_id, dir);
	else if (index_f == NULL) {
		index_f = addNewTouchin(index_f, index_t, from_student_id, to_student_id, dir);
		int* shifts = computeShifts(index_f, index_t, dir);
		combine2TouchinsIntoNewOne(index_f, index_t, dir, shifts);
	}
	else if (index_t == NULL) {
		index_t = addNewTouchin(index_f, index_t, from_student_id, to_student_id, dir);
		int* shifts = computeShifts(index_f, index_t, dir);
		combine2TouchinsIntoNewOne(index_f, index_t, dir, shifts);
	}
	else {
		int* shifts = computeShifts(index_f, index_t, dir);
		combine2TouchinsIntoNewOne(index_f, index_t, dir, shifts);
	}

	delete index_f;
	delete index_t;
}

/* This method prints the given seating plan, in
   other words the 2D array. Between each seat,
   print an empty space, i.e. " " character.
   Also, put an empty space to the end of each row.
   After each row (including the last one), print a
   new line, i.e. "\n" character.
   For the empty seats, print a "X" character (uppercase).
   For non-empty seats, print the id of the student
   sitting there.
*/
void Metu::printSeatingPlan() {

	int** group = touchins[0];

	for (int i = 0; i < row_size; i++) {
		for (int j = 0; j < column_size; j++)
			if (group[i][j] < 0)
				cout << "X ";
			else
				cout << group[i][j] << " ";
		cout << endl;
	}

}

/* My own method.
*/
Metu::triple* Metu::findStudentInTouchins(int student_id, int t) {

	int** group = touchins[t];
	for (int i = 0; i < row_size; i++)
		for (int j=0; j<column_size; j++)
			if (group[i][j] == student_id) {
				struct triple* index = new triple();
				index->t = t;
				index->i = i;
				index->j = j;
				return index;
			}
	return NULL;

}

/* My own method.
*/
Metu::triple* Metu::addNewTouchin(triple* index_f, triple* index_t, int from_student_id, int to_student_id, string dir) {
	
	constructNewTouchin();
	int t = touchins.size() - 1;
	touchins[t][0][0] = from_student_id;
	triple* to_return_index = NULL;

	if (index_f == NULL) {
		if (index_t) {
			index_f = new triple();
			index_f->t = t;
			index_f->i = 0;
			index_f->j = 0;
			to_return_index = index_f;
		}
	}

	if (index_t == NULL) {
		if (index_f) {
			index_t = new triple();
			index_t->t = t;
			index_t->i = 0;
			index_t->j = 0;
			to_return_index = index_t;
			if (dir == "-")
				index_t->j = 1;
			else
				index_t->i = 1;
		}
	}

	if (dir == "-")
		touchins[t][0][1] = to_student_id;
	else
		touchins[t][1][0] = to_student_id;

	return to_return_index;
}

/* My own method.
*/
int* Metu::computeShifts(triple* index_f, triple* index_t, string dir) {

	int row_shift_f = 0, row_shift_t = 0;
	int column_shift_f = 0, column_shift_t = 0;

	if (dir == "-") {
		if (index_f->i < index_t->i)
			row_shift_f = index_t->i - index_f->i;
		else
			row_shift_t = index_f->i - index_t->i;
		if (index_f->j + 1 < index_t->j)
			column_shift_f = index_t->j - (index_f->j + 1);
		else
			column_shift_t = (index_f->j + 1) - index_t->j;
	}
	else {
		if (index_f->i + 1 < index_t->i)
			row_shift_f = index_t->i - (index_f->i + 1);
		else
			row_shift_t = (index_f->i + 1) - index_t->i;
		if (index_f->j < index_t->j)
			column_shift_f = index_t->j - index_f->j;
		else
			column_shift_t = index_f->j - index_t->j;
	}

	int* shifts = new int[4]{ row_shift_f, row_shift_t, column_shift_f, column_shift_t };
	return shifts;
}

/* My own method.
*/
void Metu::combine2TouchinsIntoNewOne(triple* index_f, triple* index_t, string dir, int* shifts) {

	constructNewTouchin();
	int** newgroup = touchins[touchins.size() - 1];
	int row_shift_f = shifts[0];
	int row_shift_t = shifts[1];
	int column_shift_f = shifts[2]; 
	int column_shift_t = shifts[3];

	for (int i = 0; i < row_size; i++) {
		for (int j = 0; j < column_size; j++) {
			if (touchins[index_f->t][i][j] != -1)
				newgroup[i + row_shift_f][j + column_shift_f] = touchins[index_f->t][i][j];
			if (touchins[index_t->t][i][j] != -1)
				newgroup[i + row_shift_t][j + +column_shift_t] = touchins[index_t->t][i][j];
		}
	}

	if (index_f->t > index_t->t) {
		destructTouchin(index_f->t);
		touchins.erase(touchins.begin() + index_f->t);
		destructTouchin(index_t->t);
		touchins.erase(touchins.begin() + index_t->t);
	}
	else {
		destructTouchin(index_t->t);
		touchins.erase(touchins.begin() + index_t->t);
		destructTouchin(index_f->t);
		touchins.erase(touchins.begin() + index_f->t);
	}

	delete[] shifts;

}

/* My own method.
*/
void Metu::constructNewTouchin() {
	int** new_group = new int* [row_size];
	for (int i = 0; i < row_size; i++) {
		new_group[i] = new int[column_size];
		for (int j = 0; j < column_size; j++)
			new_group[i][j] = -1;
	}
	touchins.push_back(new_group);
}

/* My own method.
*/
void Metu::destructTouchin(int t) {

	int** group = touchins[t];
	for (int i = 0; i < row_size; i++) {
		delete[] group[i];
		group[i] = NULL;
	}
	delete[] group;
	group = NULL;

}





