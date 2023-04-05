#include "Operation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Operation::genIR(vector<string> params)
{
    for (vector<string>::iterator it = params.begin(); it != params.end(); it++)
    {
        instrIR += *it + (next(it) != params.end() ? ", " : "\n");
    }
}

string Operation::getInstrIR()
{
    return instrIR;
}
void Add::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "add\t" + instrIR;
}

void Sub::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "sub\t" + instrIR;
}

void Mul::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "mul\t" + instrIR;
}
void Div::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "div\t" + instrIR;
}
void Mod::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "mod\t" + instrIR;
}
void Copy::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "copy\t" + instrIR;
}
void Ldconst::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "ldconst\t" + instrIR;
}
void Rmem::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "rmem\t" + instrIR;
}
void Wmem::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "wmem\t" + instrIR;
}
void Call::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "call\t" + instrIR;
}
void Cmp_eq::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_eq\t" + instrIR;
}
void Cmp_lt::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_lt\t" + instrIR;
}
void Cmp_le::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Cmp_ge::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_ge\t" + instrIR;
}
void Cmp_gt::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_gt\t" + instrIR;
}

void Unary_negate::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "unary_negate\t" + instrIR;
}

void Unary_different::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "unary_different\t" + instrIR;
}

void Bite_and::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "bite_and\t" + instrIR;
}

void Bite_or::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "bite_or\t" + instrIR;
}
void Bite_xor::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "bite_xor\t" + instrIR;
}
void Return_::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "return_\t" + instrIR;
}