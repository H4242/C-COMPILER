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
    if (t.getType() == INT)
        currentOffset -= 4;
    else if (t.getType() == CHAR)
        currentOffset -= 1;
    symbolTableIndex[name] = currentOffset;
    symbolTableType[name] = t;
    symbolTableUsed[name] = false;
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

void CFG::add_to_current_bb(Operation op, Type t, vector<string> params)
{
    current_bb->add_IRInstr(op, t, params);
}

void CFG::add_to_const_symbol(string name, int val)
{
    symbolTableConst[name] = val;
}

map<string, bool> CFG::get_symbol_table_used()
{
    return symbolTableUsed;
}