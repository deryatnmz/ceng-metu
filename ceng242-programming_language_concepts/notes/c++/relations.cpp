#include <iostream>
using namespace std;

class A {
    int a;
    public:
    A(int num=11):a(num){ cout<<"A's constructor is called\n";};
};

class B{
    int b;
    public:
    B(int num=22):b(num){ cout<<"B's constructor is called\n";};
};

class C{
    int c;
    A a_ob;
    B b_ob;
    public:
    //C(int num,A x,B y):c(num),aob(){ cout<<"A's constructor is called\n"};
};


int main(){
    cout<<"------\n";
    C c;
    cout<<"------\n";
    return 0;
}