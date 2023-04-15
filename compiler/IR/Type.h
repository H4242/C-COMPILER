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

public:
    Type() {}
    ~Type() {}
    Type(string name) { type = typeMap[name]; }
    typeEnum getType() { return type; }
};
