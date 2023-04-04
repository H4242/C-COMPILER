#include "IRInstr.h"
using namespace std;
IRInstr::IRInstr(BasicBlock *bb_, Operation &op, Type t, vector<string> params)
{
    this->bb = bb_;
    this->op = op;
    this->t = t;
    this->params = params;
}

void IRInstr::gen_IR(ostream &o)
{
    this->op.genIR(this->params);
    o << this->op.getInstrIR();
}