#include <iostream>
using namespace std;

class me{
    public:
    me(){}
    me(string ad, int yas):name(ad),age(yas){}

    friend ostream& operator<<(ostream &o, const me& p){ //print operator //without const error //to access name and age outside the func it is friend
        o<<"name: "<< p.name <<"\nage: "<< p.age<<"\n";
        return o;
        }
   friend istream& operator>>(istream &o, me& p){ //input overloading
        o>>p.age;//input ist as age
        return o;
    }
    void operator>>(me &p){p.name=name, p.age=99;}

    private:
    string name;
    int age; 
};



int main(){
    me ben = me("derya",21);
/*    me sen = me("ali",13);
    cout<<me(ben);s
    cout <<me(sen);
    cout<<" .. \n";
    ben>>sen;
    cout<<me(ben);
    cout <<me(sen);*/
cout<<" .. \n";
cout<<me(ben);
    cin>> ben; //works in terminal
    cout<<me(ben);
    cout<<" .. \n";

    return 0;
}