#include<iostream>
#include <iomanip>
#include <sstream>

using namespace std;

string convert(float number) {
    ostringstream buff;
    buff << fixed << setprecision(2) << number;
    return buff.str();
}

string convertInt(int number) {
    ostringstream buff;
    buff << number;
    return buff.str();
}