/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
    The entry block is the one with the same label as the AST function name.
    (it could be the first of bbs, or it could be defined by an attribute value)
    The exit block is the one with both exit pointers equal to nullptr.
    (again it could be identified in a more explicit way)

*/
#ifndef CFG_H
#define CFG_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <initializer_list>
#include "Type.h"
#include "BasicBlock.h"
using namespace std;

class CFG
{
public:
    CFG(string name);

    void add_bb(BasicBlock *bb);

    // symbol table methods
    void add_to_symbol_table(string name, Type t);
    string create_new_tempvar(Type t);
    int get_var_index(string name);
    Type get_var_type(string name);
    bool is_in_symbol_table(string name);
    void add_const_to_symbol_table(string name, int val);
    string new_BB_name();
    map<string, int> get_symbol_table_const();
    map<string, Type> get_symbol_table_type();
    map<string, int> get_symbol_table_index();
    void add_to_current_bb(Operation *op, Type t, vector<string> params);

    // getters
    vector<BasicBlock *> get_bbs();
    BasicBlock *get_current_bb();

    string
    get_name()
    {
        return name;
    }

protected:
    map<string, Type> symbolTableType; /**< part of the symbol table  */
    map<string, int> symbolTableIndex; /**< part of the symbol table  */
    map<string, int> symbolTableConst; /**< part of the symbol table  */
    int nextFreeSymbolIndex = 0;       /**< to allocate new symbols in the symbol table */
    int nextBBnumber = 0;              /**< just for naming */

    vector<BasicBlock *> bbs; /**< all the basic blocks of this CFG*/

    string name; /**< name of the function */

    // basic block management
    BasicBlock *current_bb;
};

#endif