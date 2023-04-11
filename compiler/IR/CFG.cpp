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
    {
        nextFreeSymbolIndex -= 4;
    }
    if (t.getType() == CHAR)
    {
        nextFreeSymbolIndex -= 1;
    }
    symbolTableIndex[name] = nextFreeSymbolIndex;
    symbolTableType[name] = t;
}

string CFG::create_new_tempvar(Type t)
{
    string name = "!tmp" + to_string(nextFreeSymbolIndex);
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

void CFG::add_to_current_bb(Operation *op, Type t, vector<string> params)
{
    current_bb->add_IRInstr(op, t, params);
}

void CFG::add_const_to_symbol_table(string name, int val)
{
    symbolTableConst[name] = val;
}

map<string, int> CFG::get_symbol_table_index()
{
    return symbolTableIndex;
}

map<string, Type> CFG::get_symbol_table_type()
{
    return symbolTableType;
}

map<string, int> CFG::get_symbol_table_const()
{
    return symbolTableConst;
}

vector<BasicBlock *> CFG::get_bbs()
{
    return bbs;
}

BasicBlock *CFG::get_current_bb()
{
    return current_bb;
}

void CFG::set_current_bb(BasicBlock *bb)
{
    current_bb = bb;
}
