#include "Gen_x86.h"

void Gen_x86::gen_asm(ostream &o)
{
    for (auto cfg : cfgs)
    {
        gen_asm_prologue(o, cfg->get_name());
        for (auto bb : cfg->get_bbs())
        {
            // o << bb->get_name() << ":\n";
            for (auto instr : bb->get_instrs())
            {
                instr->getOp()->gen_x86(instr->getParams(), o);
            }
        }
        gen_asm_epilogue(o);
    }
}

void Gen_x86::gen_asm_prologue(ostream &o, string name)
{
    o << "\t.globl\t" << name << "\n"
      << "\t.type " << name << ",@function\n"
      << name << ": \n"
                 // prologue
                 "\tpushq\t%rbp\n"
                 "\tmovq\t%rsp, %rbp\n";
}

void Gen_x86::gen_asm_epilogue(ostream &o)
{
    o << "\tmovq\t%rbp, %rsp\n"
      << "\tpopq\t%rbp\n"
      << "\tret\n";
}
