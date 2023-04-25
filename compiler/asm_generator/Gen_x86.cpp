#include "Gen_x86.h"

void Gen_x86::gen_asm(ostream &o)
{
    for (auto cfg : cfgs)
    {
        gen_asm_prologue(o, cfg);
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

void Gen_x86::gen_asm_prologue(ostream &o, CFG *cfg)
{
    o << "\t.globl\t" << cfg->get_name() << "\n"
      << "\t.type " << cfg->get_name() << ",@function\n"
      << cfg->get_name() << ": \n"
      // prologue
      << "\tpushq\t%rbp\n"
      << "\tmovq\t%rsp, %rbp\n"
      << "\tsubq	$" << -cfg->get_nextFreeSymbolIndex() << ", %rsp\n";
}

void Gen_x86::gen_asm_epilogue(ostream &o)
{
    o << "\tmovq\t%rbp, %rsp\n"
      << "\tpopq\t%rbp\n"
      << "\tret\n";
}
