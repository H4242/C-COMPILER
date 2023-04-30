#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>
#include "Operation.h"

using namespace std;
class IRInstr;

class BasicBlock
{
public:
  BasicBlock(string entry_label) : label(entry_label) {}
  void add_IRInstr(Operation *op, vector<string> params);

  BasicBlock *get_next_block() { return next_block; }
  string get_test_var_name() { return test_var_name; }
  string get_label() { return label; }
  vector<IRInstr *> get_instrs() { return instrs; }

  void set_next_block(BasicBlock *bb) { next_block = bb; }
  void set_test_var_name(string name) { test_var_name = name; }

protected:
  BasicBlock *next_block = nullptr;
  string label;             /** < the CFG where this block belongs */
  vector<IRInstr *> instrs; /** < the instructions themselves. */
  string test_var_name;     /** < when generating IR code for an if(expr) or while(expr) etc,
                                                        store here the name of the variable that holds the value of expr */
};

#endif

/**  The class for a basic block */

/* A few important comments.
     IRInstr has no jump instructions.
     cmp_* instructions behaves as an arithmetic two-operand instruction (add or mult),
      returning a boolean value (as an int)

     Assembly jumps are generated as follows:
     BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions, and then
            if  exit_true  is a  nullptr,
            the epilogue is generated
        else if exit_false is a nullptr,
          an unconditional jmp to the exit_true branch is generated
                else (we have two successors, hence a branch)
          an instruction comparing the value of test_var_name to true is generated,
                    followed by a conditional branch to the exit_false branch,
                    followed by an unconditional branch to the exit_true branch
     The attribute test_var_name itself is defined when converting
  the if, while, etc of the AST  to IR.

Possible optimization:
     a cmp_* comparison instructions, if it is the last instruction of its block,
       generates an actual assembly comparison
       followed by a conditional jump to the exit_false branch
*/