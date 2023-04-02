#include "CFG.h"

CFG::CFG(string name_)
{
    name = name_;
    add_bb(new BasicBlock(this, new_BB_name()));
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
    symbolTable[name] = currentOffset;
    symbolType[name] = t;
}

string CFG::create_new_tempvar(Type t)
{
    string name = "!tmp" + currentOffset;
    add_to_symbol_table(name, t);
    return name;
}

int CFG::get_var_index(string name)
{
    return symbolIndex[name];
}

Type CFG::get_var_type(string name)
{
    return symbolType[name];
}

string CFG::new_BB_name()
{
    return "bb" + to_string(nextBBnumber++);
}