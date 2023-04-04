#include "IRInstr.h"
using namespace std;
IRInstr::IRInstr(Operation &op, Type t, vector<string> params)
{
    this->op = op;
    this->t = t;
    this->params = params;
}

void IRInstr::gen_IR(ostream &o)
{
    this->op.genIR(this->params);
    o << this->op.getInstrIR();
}