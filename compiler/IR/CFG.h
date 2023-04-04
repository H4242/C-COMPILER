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
#include <string>
#include <iostream>
#include <initializer_list>

#include "BasicBlock.h"

class CFG
{
public:
    CFG(string name);

    void add_bb(BasicBlock *bb);

    // symbol table methods
    void add_to_symbol_table(string name, Type t);
    void set_var_used(string name);
    bool is_var_used(string name);
    string create_new_tempvar(Type t);
    int get_var_index(string name);
    Type get_var_type(string name);
    boolean is_in_symbol_table(string name);

    string new_BB_name();

    void add_to_current_bb(IRInstr *instr);

    // getters
    string get_name() { return name; }

protected:
    map<string, Type> SymbolTableType; /**< part of the symbol table  */
    map<string, int> SymbolTableIndex; /**< part of the symbol table  */
    map<string, bool> SymbolTableUsed; /**< part of the symbol table  */
    int currentOffset = 0;             /**< to allocate new symbols in the symbol table */
    int nextBBnumber = 0;              /**< just for naming */

    vector<BasicBlock *> bbs; /**< all the basic blocks of this CFG*/

    string name; /**< name of the function */

    // basic block management
    BasicBlock *current_bb;
};

#endif