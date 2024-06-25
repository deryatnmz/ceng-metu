#include <iostream>
using namespace std;

class A { int x;
public : virtual void get() { cout << "A:: get ()\n";}
};
class B : public A { int y;
public : void get() { cout << "B:: get () \n";}
};
/*
****without virtual keyword in A get
Polymorphism
Binding is still static, at compile time
Pointers of derived classes are converted to superclass types
----
A:: get ()
A:: get ()
----

******with virtual keyword
Inheritance
inclusion polymorphism
late binding, done at run time,dynamic binding

----
A:: get ()
B:: get () 
----

*/

int main(){
    cout <<"----\n";
    A a , *p;
    B b;
    p=& a; 
    p->get();
    p=&b; 
    p-> get();
     cout <<"----\n";

    return 0;
}