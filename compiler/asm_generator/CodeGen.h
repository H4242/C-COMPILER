#ifndef CODEGEN_H
#define CODEGEN_H

#include "../IR/CFG.h"
#include "../IR/IRInstr.h"
#include "../IR/BasicBlock.h"

class CodeGen
{
public:
    CodeGen(CFG *cfg) { this->cfg = cfg; }
    virtual ~CodeGen() {}
    virtual void gen_asm(ostream &o) {}
    virtual void gen_asm_prologue(ostream &o) {}
    virtual void gen_asm_epilogue(ostream &o) {}

protected:
    CFG *cfg;
};

#endif // CODEGEN_H