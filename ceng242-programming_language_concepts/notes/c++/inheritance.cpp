#include <iostream>
using namespace std;
/*
-----
without parameter base class costructor is called
derived class costructor is called
-----
derived class destructor is called
base class destructor is called

*/

class shape{ //generalized class, super class
protected:
    double x,y;
public:
    shape (){
        x=7;
        y=8;
        cout<< "without parameter base class costructor is called\n";
    }
    shape(double a, double b):x(a),y(b){ 
        cout<< "with parameter base class costructor is called\n";
    }
    ~shape(){
        cout<< "base class destructor is called\n";
    }

    void draw();
};

class circle: public shape{
    double radius;
public:
    circle(int r):radius(r){ 
        //to implement derived classes base class elements must be initalized in itself
        cout<< "derived class costructor is called\n";
    }
    void draw();
    ~circle(){
        cout<< "derived class destructor is called\n";
    }
};

class square:public shape {
    double width;
public:
    square(int w):width(w){
        cout<< "derived class costructor is called\n";
    }
    void draw();
    ~square(){
        cout<< "derived class destructor is called\n";
    }
};

int main(){
    cout<<"-----\n";

    //shape REct;
    circle A(8);
    cout<<"-----\n";
    return 0;
}