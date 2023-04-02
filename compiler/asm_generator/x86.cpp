#include "x86.h"

void CodeGen::gen_asm(ostream &o)
{
    gen_asm_prologue(o);
    for (auto bb : cfg->get_bbs())
    {
        o << bb->get_name() << ":\n";
        for (auto instr : bb->get_instrs())
        {
                }
    }
    gen_asm_epilogue(o);
}

void x86::gen_asm_prologue(ostream &o)
{
    o << "\t.globl " << cfg->get_name() << endl;
    // o << "\t.type " << cfg->get_name() << ", @function" << endl;
    o << cfg->get_name() << ":\n"
      << "\tpushq\t%rbp\n"
         "\tmovq\t%rsp, %rbp\n";
}

void x86::gen_asm_epilogue(ostream &o)
{
    o << "\tpopq\t%rbp\n"
      << "\tret";
}