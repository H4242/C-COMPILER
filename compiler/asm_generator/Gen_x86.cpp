#include "Gen_x86.h"

void Gen_x86::gen_asm(ostream &o)
{
    gen_asm_prologue(o);
    for (auto bb : cfg->get_bbs())
    {
        o << bb->get_label() << ":\n";
        for (auto instr : bb->get_instrs())
        {
            instr->getOp()->gen_x86(instr->getParams(), o);
        }
        if (bb->get_next_block() != nullptr)
        {
            o << "\tjmp\t" << bb->get_next_block()->get_label() << "\n";
        }
    }
    gen_asm_epilogue(o);
}

void Gen_x86::gen_asm_prologue(ostream &o)
{
    o << ".globl\tmain\n"
         " main: \n"
         // prologue
         "\tpushq\t%rbp\n"
         "\tmovq\t%rsp, %rbp\n";
}

void Gen_x86::gen_asm_epilogue(ostream &o)
{
    o << "\tpopq\t%rbp\n"
      << "\tret\n";
}
