#include "BasicBlock.h"
#include "IRInstr.h"

void BasicBlock::add_IRInstr(Operation *op, vector<string> params)
{
    instrs.push_back(new IRInstr(op, params));
}