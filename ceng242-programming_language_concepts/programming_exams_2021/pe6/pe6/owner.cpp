#include <iostream>
#include <string>
#include <vector>
#include "owner.h"

using namespace std;

Owner::Owner()
{
}

Owner::Owner(const string &name, float balance)
{
}

void Owner::print_info()
{
}

string &Owner::get_name()
{
}

void Owner::add_property(Property *property)
{
}

void Owner::buy(Property *property, Owner *seller)
{
}

void Owner::sell(Property *property, Owner *owner)
{
}

void Owner::list_properties()
{
}