#include "./IR/CFG.h"

class CodeGen
{
public:
    CodeGen() {}
    virtual ~CodeGen() {}
    virtual void gen_asm(ostream &o) override;
    virtual string IR_reg_to_asm(string reg) override; /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
    virtual void gen_asm_prologue(ostream &o) override;
    virtual void gen_asm_epilogue(ostream &o) override;
};