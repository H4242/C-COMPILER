#pragma once

#include <unordered_map>
#include <string>
#include "Variable.h"
using namespace std;

class SymbolTable
{

public:
    bool existingVariable(string name);
    void addVariable(string name, int offset, int value = 0);
    SymbolTable()
    {
        variableTable = {};
    }
    virtual ~SymbolTable() {}

public:
    unordered_map<string, Variable> variableTable;
};