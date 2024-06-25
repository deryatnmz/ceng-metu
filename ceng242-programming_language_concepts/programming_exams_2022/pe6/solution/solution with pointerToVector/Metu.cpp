#include "Metu.h"

// YOU CAN COMPLETELY CHANGE THIS FILE AS YOU NEED. //

/****************************************************/
/****                *** METU ***                ****/

/* Constructor : Initialize your own variables if you
   need.
*/
Metu::Metu() = default;

/* Destructor  : It is responsible of the destruction of
   the Course and OpenCourse objects and the registered
   students, and the constrcuted seating plan, if any.
*/
Metu::~Metu() {
    for (auto s : students) // Virtual destructor would be necessary IRL
        delete s;
    for (auto c : courses)
        delete c;
        
    // We -could- still add the new open courses into courses, but that's
    // not really right since it wouldn't call the proper destructor. Would
    // still work in the scope of the PE, but I try to avoid bad things.
    for (auto c : open_courses)
        delete c;
}

/* This method returns the student whose id is given
   in the argument. You need to find the corresponding
   student among the registered students.
*/
Student& Metu::getStudent(int id) {
	for (auto s : students)
	    if (s->getId() == id)
	        return *s;
	// Ideally, should return a pointer or option<> type, or a custom type
	// But have to return something here, so just do whatever.
	return *students[0];
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

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
OpenCourse& Metu::openCourse(const Course& course, string term, int quota, 
							vector<Freshman*> freshmans, vector<Sophomore*> sophomores, vector<Junior*> juniors, vector<Senior*> seniors) {
    // I need to allocate the courses dynamically, otherwise the reference I return
    // may be invalidated when the vector is realloc'd. Oof!
    OpenCourse *oc = new OpenCourse(course, term, 0, quota);
    open_courses.push_back(oc);
	
	// Also, since we can't access the course's own quota (something like
	// .addStudents() for course objects), we have to count the quota here
	addStudents(*oc, &quota, seniors);
	addStudents(*oc, &quota, juniors);
	addStudents(*oc, &quota, sophomores);
	addStudents(*oc, &quota, freshmans);
	
	return *oc;
}

template <typename S>
void Metu::addStudents(const OpenCourse &c, int *rem_quota, vector<S *> students)
{
    // Just add students until there is no more quota left.
    // Passing the quota as a pointer to keep modifying it.
    // Could also use a reference or return a new value, but
    // this is how I like it. 1,000 ways to do sth in C++...
    for (auto s : students) {
        if (*rem_quota <= 0)
            break;
        if (s->addCourse(c))
            *rem_quota -= 1;
    }
}

// Hate pasting code? Templates to the rescue! This is the common 
// function for all upgrade() methods. Essentially, look for a student
// with the same ID as the given ID to find the location, then replace 
// with its upgraded version. 
template<typename U, typename C>
U *Metu::upgrade(C &student) {
    U *upg = NULL;
	for (size_t i = 0; i < students.size(); i++) {
	    auto s = students[i];
	    if (s->getId() == student.getId()) {
	        upg = new U(student);
	        delete s;
	        students[i] = upg;
	        break;
	    }
	}
	return upg;
}

/* This method upgrades the given Freshman object to
   the Sophomore status by constructing a new Sophomore
   object with the properties of the given Freshman.
   You should delete the given Freshman and place the 
   newly created Sophomore object to its location in the 
   students array of Metu object.
   In the end, you should return the created Sophomore.

   By the way; of course it would be nicer to define 
   this method in "virtual" in order to use it for different 
   types of Students. However, we do not want to test 
   you with "virtual" keyword in this PE. We've left it 
   to next PE.
*/
Sophomore* Metu::upgradeStudent(Freshman& student) {
    // Observe how C++ doesn't disambiguate via return type here.
    // It can deduce that upgrade<*, Freshman> should be called
    // since the argument is a Freshman, but doesn't deduce
    // Sophomore from the return type of the function. Instead,
    // Sophomore has to be provided manually.
    return upgrade<Sophomore>(student);
}

/* This method upgrades the given Sophomore object to
   the Junior status by constructing a new Junior
   object with the properties of the given Sophomore.
   You should delete the given Sophomore and place the
   newly created Junior object to its location in the
   students array of Metu object.
   In the end, you should return the created Junior.

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
Junior* Metu::upgradeStudent(Sophomore& student) {
    return upgrade<Junior>(student);
}

/* This method upgrades the given Junior object to
   the Senior status by constructing a new Senior
   object with the properties of the given Junior.
   You should delete the given Junior and place the
   newly created Senior object to its location in the
   students array of Metu object.
   In the end, you should return the created Senior.

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
Senior* Metu::upgradeStudent(Junior& student) {
    return upgrade<Senior>(student);
}


/* This method sets the number of rows and number of columns in a
   standard Metu classroom with the given arguments, respectively.
*/
void Metu::setClassroomSize(int row_size, int column_size) {
	rows = row_size;
	cols = column_size;
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
	CInfo c = {from_student_id,to_student_id,0,0};
	if (dir == "|") {
	    c.ioff = 1;
	} else {
	    c.joff = 1;
	}
	cinfo.push_back(c);
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

// Using an index is basic compared to itrs but useful in our case
int vfind(vector<int> &v, int x)
{
    for (int i = 0, sz = v.size(); i < sz; i++)
        if (v[i] == x)
            return i;
    return -1;
}

void Metu::printSeatingPlan() {
	// Have to resolve the seating plan
	
	// Can do a depth-first kind of thing
	// Pick a random (first) student as the origin
	// Find all coordinates relative to that student
	// Use a vector as a stack. 
	
	// Would be nice to have
	// something to store <key, coordinate> pairs in
	// like an unordered_map, but can't. Using an array as a map
	// would work if indices were limited, but let's not
	// assume that either. Let's just make a map-like interface
	// for a vector even though it's some effort and has bad perf.
	vector<int> stack;
	vector<int> sids;
	vector<pair<int, int>> student_coords;
	
	// Initial student and coordinates
	int start_id = cinfo[0].id1;
	stack.push_back(start_id);
	sids.push_back(start_id);
	student_coords.emplace_back(0, 0);
	// Run the search, will work since connected
	while (!stack.empty()) {
	    int top = stack.back();
	    stack.pop_back();
	    int top_i = vfind(sids, top);
	    auto c = student_coords[top_i];
	    for (const auto &info : cinfo) { // Find connected coordinates
	        if (info.id1 == top && vfind(sids, info.id2) == -1) {
	            stack.push_back(info.id2);
	            sids.push_back(info.id2);
	            student_coords.emplace_back(c.first + info.ioff, c.second + info.joff);
	        } else if (info.id2 == top && vfind(sids, info.id1) == -1) {
	            stack.push_back(info.id1);
	            sids.push_back(info.id1);
	            student_coords.emplace_back(c.first - info.ioff, c.second - info.joff);
	        }
	    }
	}
	
	// Now everyone's relative offsets are known. Find 0 coord and rebalance. 
	// Of course, there should be someone at every edge to not have ambiguity.
	
	// Trick for int max, you'll learn this stuff in CENG331. Any large value would be fine.
	int i_min = (~0U) >> 1; 
	int j_min = i_min;
	for (const auto &p : student_coords) {
	    if (p.first < i_min)
	        i_min = p.first;
	    if (p.second < j_min)
	        j_min = p.second;
	}
	
	// Now, make everyone's coords start from (0, 0) by subtracting mins
	for (auto &p : student_coords) {
	    p.first -= i_min;
	    p.second -= j_min;
	}
	
	// Prepare a vector using the coordinates. -1 if no-one is there.
	vector<vector<int>> seating(rows, vector<int>(cols, -1));
	for (size_t i = 0; i < sids.size(); i++) {
	    const auto &p = student_coords[i];
	    seating[p.first][p.second] = sids[i];
	}
	
	// Finally, print
	for (const auto &row : seating) {
	    for (auto sid : row) {
	        if (sid == -1)
	            cout << 'X';
	        else
	            cout << sid;
	        cout << ' ';
	    }
	    cout << endl;
	}
}
