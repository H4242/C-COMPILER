#pragma once

#include <string>
using namespace std;

class Variable
{
protected:
    string name;
    int offset;
    int value;

public:
    Variable() {}
    Variable(string name, int offset, int value = 0) : name(name), offset(offset), value(value) {}
    ~Variable() {}
    int getOffset() { return offset; }
    int getValue() { return value; }
    void setValue(int value) { this->value = value; }
};
