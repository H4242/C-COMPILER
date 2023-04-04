#ifndef IRINSTR_H
#define IRINSTR_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>

// Declarations from the parser -- replace with your own
#include "Type.h"
#include "BasicBlock.h"
#include "Operation.h"
using namespace std;
class IRInstr
{

public:
    /**  constructor */
    IRInstr(BasicBlock *bb_, Operation &op, Type t, vector<string> params);

    /** Actual code generation */
    void gen_IR(ostream &o); /**< x86 assembly code generation for this IR instruction */

private:
    BasicBlock *bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
    Operation op;
    Type t;
    vector<string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */
                           // if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very important) comment becomes useless: it would be a better design.
};
#endif