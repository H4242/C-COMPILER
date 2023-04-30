#include "IRInstr.h"
using namespace std;

IRInstr::IRInstr(Operation *op, vector<string> params)
{
    this->op = op;
    this->params = params;
}

Operation *IRInstr::getOp()
{
    return op;
}

vector<string> IRInstr::getParams()
{
    return params;
}