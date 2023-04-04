#ifndef TYPE_H
#define TYPE_H

typedef enum
{
    INT,
    CHAR
} typeEnum;

class Type
{
private:
    map<string, typeEnum> typeMap = {{"int", INT}, {"char", CHAR}};

public:
    Type(string name)
    {
        type = typeMap[name];
    }
    Type getType()
    {
        return type;
    }
    ~Type();
};

#endif