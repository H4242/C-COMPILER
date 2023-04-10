#pragma once

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
    IRInstr(Operation *op, Type t, vector<string> params);

    /** Actual code generation */
    void gen_IR(ostream &o); /**< x86 assembly code generation for this IR instruction */

    /** Getters */
    Operation *getOp();
    vector<string> getParams();

private:
    Type t;
    Operation *op;
    vector<string> params;

    /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */

    // if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very important) comment becomes useless: it would be a better design.
};
