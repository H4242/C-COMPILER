#ifndef CODEGEN_H
#define CODEGEN_H

#include "../IR/CFG.h"
#include "../IR/IRInstr.h"
#include "../IR/BasicBlock.h"

class CodeGen
{
public:
    CodeGen(vector<CFG *> cfgs) { this->cfgs = cfgs; }
    virtual ~CodeGen() {}
    virtual void gen_asm(ostream &o) {}
    virtual void gen_asm_prologue(ostream &o, CFG *cfg) {}
    virtual void gen_asm_epilogue(ostream &o) {}

protected:
    vector<CFG *> cfgs;
};

#endif // CODEGEN_H