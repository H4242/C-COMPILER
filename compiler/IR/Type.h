#pragma once
#include <map>
#include <string>

using namespace std;
typedef enum
{
    INT,
    CHAR
} typeEnum;

class Type
{
private:
    typeEnum type;
    map<string, typeEnum> typeMap = {{"int", INT}, {"char", CHAR}};
    map<typeEnum, int> typeSize = {{INT, 4}, {CHAR, 1}};

public:
    Type() {}
    ~Type() {}
    Type(string name) { type = typeMap[name]; }
    typeEnum getType() { return type; }
    int getSize() { return typeSize[type]; }
};
