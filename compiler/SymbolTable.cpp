#include <iostream>
#include "SymbolTable.h"
using namespace std;

bool SymbolTable::existingVariable(string name)
{
    return variableTable.find(name) != variableTable.end();
}

void SymbolTable::addVariable(string name, int offset)
{
    variableTable[name] = offset;
}

int SymbolTable::getOffset(string name)
{
    if (existingVariable(name))
    {
        return variableTable[name];
    }
    return -1;
}