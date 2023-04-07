#include "../IR/CFG.h"
#include "../IR/IRInstr.h"
#include "../IR/BasicBlock.h"

class CodeGen
{
public:
    CodeGen(CFG *cfg) { this->cfg = cfg; }
    virtual ~CodeGen() {}
    void gen_asm(ostream &o);
    //virtual string IR_reg_to_asm(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
    //virtual void gen_asm_prologue(ostream &o);
    //virtual void gen_asm_epilogue(ostream &o);

protected:
    CFG *cfg;
};