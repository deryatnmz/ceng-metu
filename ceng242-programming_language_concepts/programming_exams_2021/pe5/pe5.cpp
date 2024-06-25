#include <iostream>
#include <math.h>


class PathTracker{
public:
    int final_x;
    int final_y;
    float displacement;
    float distance;
    PathTracker(); 
    //empty const to initialize all to 0
    PathTracker(int *existing_path, int number_of_vectors);
    //[1, 2, 3, 4, 5, 6] -> 3 vectors: [1, 2], [3, 4], [5, 6] Add them end-to end: [1, 2] + [3, 4] + [5, 6] -> [9, 12]
    //Update displacement and distance at each addition.
    PathTracker &operator+= (const int *new_vector);
    // adds new vector to the end of the path.
    //all properties of the object will be updated.
    bool operator==(const PathTracker &rhs) const;
    //This operator checks only displacements.
    bool operator>(const PathTracker &rhs) const;
    //This operator checks only displacements.
    bool operator<(const PathTracker &rhs) const;
    //This operator checks only displacements.
    bool operator==(float comp) const;
    //This operator checks only distance of the PathTracker
    float calculate_displacement();
    //Calculates and returns the displacement value of the path --with origin
    void calculate_distance(int x, int y);
    //Calculates and sets the distance value of the path
    float calculate_l2(int x1, int y1, int x2, int y2);
    //It is a helper function which calculates Euclidean distance between two points on the vector space.
    void summary();

};

PathTracker::PathTracker(){
    final_x=0;
    final_y=0;
    displacement=0;
    distance=0;
}
PathTracker::PathTracker(int *existing_path, int number_of_vectors){
    final_x = final_y=displacement=distance=0;
    for (int i=0; i< 2*number_of_vectors;i=i+2){
        calculate_distance(existing_path[i],existing_path[i+1]);
        final_x += existing_path[i];
        final_y += existing_path[i+1];

    }
    displacement = calculate_displacement(); 
}
PathTracker &PathTracker::operator+=(const int *new_vector){
    calculate_distance(new_vector[0],new_vector[1]);
    final_x +=new_vector[0];
    final_y+=new_vector[1];
    displacement = calculate_displacement(); 
    return *this;
}
bool PathTracker::operator==(const PathTracker &rhs) const {
    return (this->displacement==rhs.displacement);
}
bool PathTracker::operator>(const PathTracker &rhs) const {
    return (this->displacement>rhs.displacement);
}
bool PathTracker::operator<(const PathTracker &rhs) const {
    return (this->displacement>rhs.displacement);
}
bool PathTracker::operator==(float comp) const{
    return (distance==comp);
}
float PathTracker::calculate_displacement(){
    return sqrt(final_x*final_x + final_y*final_y);
}
void PathTracker::calculate_distance(int x, int y){
distance += sqrt(x*x+y*y);
}
float PathTracker::calculate_l2(int x1, int y1, int x2, int y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void PathTracker::summary(){
    std::cout<<"Final position: ["<<final_x<<","<<final_y<<"] Displacement: "<<displacement<<" Distance: "<<distance<< std::endl;
}


int main(){
int existing_path1 [2] = {25 , 30};
int existing_path2 [10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
PathTracker pt1 = PathTracker ( existing_path1 , 1);
PathTracker pt2 = PathTracker ( existing_path2 , 5);
pt1. summary ();
pt2. summary ();
std :: cout << "pt1 == pt2 : " << (pt1 == pt2) << "\n";
std :: cout << "pt1 > pt2: " << (pt1 > pt2 ) << "\n";
std :: cout << "pt1 < pt2: " << (pt1 < pt2 ) << "\n";
std :: cout << " distance comparison : " << ( pt1 == pt2. distance ) <<
"\n";
    return 0;
}
