#include "CodeGen.h"

void CodeGen::gen_asm(ostream &o)
{
    for (auto bb : cfg->get_bbs())
    {
        //o << bb->get_name() << ":\n";
        for (auto instr : bb->get_instrs())
        {
            instr->op.gen_x86(instr->params,o);
        }
    }
}