#ifndef TYPE_H
#define TYPE_H

#include <map>
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
    map<string, typeEnum> typeMap = {{"int", INT}, {"char", CHAR}, {"i", INT}, {"c", CHAR}};

public:
    Type() {}
    Type(string name)
    {
        type = typeMap[name];
    }
    typeEnum getType()
    {
        return type;
    }
    ~Type() {}
};

#endif