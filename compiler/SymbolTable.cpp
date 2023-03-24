#include <iostream>
#include "SymbolTable.h"
using namespace std;

bool SymbolTable::existingVariable(string name)
{
    return variableTable.find(name) != variableTable.end();
}

void SymbolTable::addVariable(string name, int offset, int value)
{
    variableTable[name] = Variable(name, offset, value);
}