#include <iostream>
using namespace std;

class A{
    public:
    A(){};
    A(int v1):var1(v1){}; //initialization before {}
    private:
    int var1;
};

/*
    //delete object
    A *pointer=new A(12);
    delete pointer;

    //delete array
    int *arr = new int[23];
    delete [] arr;

*/

class int_array{
public:
    int_array(int len=6) //if no parameter len = 6, else given len
    {
        cout<<"constructor is called.\n";
        size=len;
        array= new int [size];
        for (int i =0;i<size;i++)
            array[i]=1;
    }
    int_array(const int_array &rhs) //copy constructor
    {
        cout<<"copy constructor is called.\n";
        //since no initial array no delete, no return this bc it just created
        //int_array a = b; -> it called when a is created
        size=rhs.size;
        array = new int [size];
        for (int i =0 ;i<size;i++)
            array[i]=rhs.array[i];
    }
    int_array &operator=(const int_array &rhs){
        //int_array a;
        //a=b; -> assignment op called here.
        cout<<"assignment operator is called.\n";
        if(this != &rhs){
            delete array;
            size=rhs.size;
            this->array= new int[size];
            for(int i=0 ;i<size;i++)
                array[i]=rhs.array[i];
        }
        return *this;
    }
    ~int_array(){
        cout<<"destructor is called.\n";
        //it is called when main is ended
        delete array; }
private:
    int size;
    int *array;
};
/*
cout<<"begining ----\n";
int_array my;
cout<<"end --------\n";
return 0;

output:
begining ----
constructor is called.
end --------
destructor is called.
*/

/*
pointer to constant int:
int var = 4;
int var2 = 5;
const int* pointer = &var;
var = 5; // is okay
*var = 6; //not okay -> address can change but direct value of pointer cant without unchanging address
pointer= &var2; // is okay


constant pointer to int:
int a =99;
int b =88;
int* const pointer=&a;
*pointer=13; // is okay -> value can change but address cant
pointer= &b; // is not okay
a = 13; // is okay
*/



class X {
 public:
    X() {
     x =17;
 }
    void operator += (int p) {x+=p;}
    void operator += (X &p) {p.x=2*p.x+2;}
    void operator >> (X &p){p.x =x;}
    int x;
    friend void friend_func(X &x); //friend 
 private:
 int y=3333;
};
void friend_func(X &x){ //can access private members of a class if stated in class as friend
    cout<< x.y<<endl;
};


int main(){
    cout<<"begining ----\n";
    int_array a1 = int_array(2);
    int_array a2 =a1;
    a1=a2;
    ///int_array 
    /*X x1;
    cout << x1.x<<endl;
    x1 += 6;
    cout << x1.x<<endl;
    X x2;
    cout << x1.x<<"     "<<x2.x<<endl;
    x1 += x2;
    cout << x1.x<<"     "<<x2.x<<endl;
    x2>>x1;
    cout << x1.x<<"     "<<x2.x<<endl;
    friend_func(x1);*/
    cout<<"end --------\n";
    return 0;
}