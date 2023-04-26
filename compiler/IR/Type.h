#pragma once
#include <map>
#include <string>

using namespace std;
typedef enum
{
    VOID,
    INT,
    CHAR
} typeEnum;

class Type
{
private:
    typeEnum type;
    map<string, typeEnum> typeMap = {{"void", VOID}, {"int", INT}, {"char", CHAR}};

public:
    Type() {}
    ~Type() {}
    Type(string name) { type = typeMap[name]; }
    typeEnum getType() const { return type; }
    bool operator!=(const Type &t) const { return type != t.type; }
    bool operator==(const Type &t) const { return type == t.type; }
};
