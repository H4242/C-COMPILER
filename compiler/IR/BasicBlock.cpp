#include "BasicBlock.h"
#include "IRInstr.h"
BasicBlock::BasicBlock(string entry_label) : label(entry_label) {}

void BasicBlock::add_IRInstr(Operation *op, Type t, vector<string> params)
{
    IRInstr *instr = new IRInstr(op, t, params);
    instrs.push_back(instr);
}

/*void BasicBlock::gen_IR(ostream &o)
{
    // uj = unconditional jump
    // cj = conditional jump
    // cmp = compare

    o << label << ":" << endl;
    for (auto instr : instrs)
    {
        instr->gen_IR(o);
    }
    if (exit_true == nullptr) // && exit_false == nullptr) Copilot
    {
        o << "\tret" << endl;
    }
    else if (exit_false == nullptr)
    {
        o << "\tuj " << exit_true->label << endl;
    }
    else
    {
        o << "\tcmp " << test_var_name << ", 0" << endl;
        o << "\tcj " << exit_false->label << endl;
        o << "\tuj " << exit_true->label << endl;
    }
}*/

void BasicBlock::set_next_block(BasicBlock *bb) { next_block = bb; }

BasicBlock *BasicBlock::get_next_block()
{
    return next_block;
}

/*void BasicBlock::set_exit_true(BasicBlock *bb) { exit_true = bb; }

BasicBlock *BasicBlock::get_exit_true() { return exit_true; }

void BasicBlock::set_exit_false(BasicBlock *bb) { exit_false = bb; }

BasicBlock *BasicBlock::get_exit_false() { return exit_false; }*/

void BasicBlock::set_test_var_name(string name) { test_var_name = name; }

string BasicBlock::get_test_var_name() { return test_var_name; }

string BasicBlock::get_label() { return label; }

vector<IRInstr *> BasicBlock::get_instrs()
{
    return instrs;
}