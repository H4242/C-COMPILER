#pragma once
#include <map>
#include <string>

using namespace std;

class Type
{
private:
    string type;

public:
    Type() {}
    ~Type() {}
    Type(string name) { type = name; }
    string getType() const { return type; }
};
