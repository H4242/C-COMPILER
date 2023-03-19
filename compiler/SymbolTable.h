#pragma once

#include <unordered_map>
#include <string>
using namespace std;

class SymbolTable
{

public:
    bool existingVariable(string name);
    void addVariable(string name, int offset);
    int getOffset(string name);
    SymbolTable()
    {
        variableTable = {};
    }
    virtual ~SymbolTable() {}

protected:
    unordered_map<string, int> variableTable;
};