#include "CFG.h"

CFG::CFG(string name_)
{
    name = name_;
    add_bb(new BasicBlock(new_BB_name()));
}

void CFG::add_bb(BasicBlock *bb)
{
    bbs.push_back(bb);
    current_bb = bb;
}

void CFG::add_to_symbol_table(string name, Type t)
{
    if (t == INT)
        currentOffset -= 4;
    else if (t == CHAR)
        currentOffset -= 1;
    symbolTableIndex[name] = currentOffset;
    symbolTableType[name] = t;
    SymbolTableUsed[name] = false;
}

string CFG::create_new_tempvar(Type t)
{
    string name = "!tmp" + currentOffset;
    add_to_symbol_table(name, t);
    return name;
}

int CFG::get_var_index(string name)
{
    return symbolTableIndex[name];
}

Type CFG::get_var_type(string name)
{
    return symbolTableType[name];
}

string CFG::new_BB_name()
{
    string new_name = "bb_" + to_string(nextBBnumber);
    nextBBnumber += 1;
    return new_name;
}

bool CFG::is_in_symbol_table(string name)
{
    return symbolTableIndex.find(name) != symbolTableIndex.end();
}

void CFG::set_var_used(string name)
{
    symbolTableUsed[name] = true;
}

bool CFG::is_var_used(string name)
{
    return symbolTableUsed[name];
}

void CFG::add_to_current_bb(IRInstr *instr)
{
    current_bb->add_IRInstr(instr);
}