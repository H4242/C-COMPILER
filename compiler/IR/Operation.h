#include <string>
#include <iostream>
#include <vector>
using namespace std;
class Operation
{
public:
    Operation() {}
    virtual void genIR(vector<string> params);
    string getInstrIR();
    virtual ~Operation() {}

protected:
    string instrIR = "";
};

class Add : public Operation
{
public:
    Add() : Operation() {}
    void genIR(vector<string> params);
    virtual ~Add() {}

protected:
};

class Sub : public Operation
{
public:
    Sub() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Mul : public Operation
{
public:
    Mul() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Ldconst : public Operation
{
public:
    Ldconst() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Copy : public Operation
{
public:
    Copy() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Rmem : public Operation
{
public:
    Rmem() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Wmem : public Operation
{
public:
    Wmem() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Call : public Operation
{
public:
    Call() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Cmp_eq : public Operation
{
public:
    Cmp_eq() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Cmp_lt : public Operation
{
public:
    Cmp_lt() : Operation() {}
    void genIR(vector<string> params);

protected:
};

class Cmp_le : public Operation
{
public:
    Cmp_le() : Operation() {}
    void genIR(vector<string> params);

protected:
};