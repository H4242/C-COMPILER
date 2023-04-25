#include "Operation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Operation::genIR(vector<string> params)
{
    for (vector<string>::iterator it = params.begin(); it != params.end(); it++)
    {
        instrIR += *it + (next(it) != params.end() ? ", " : "\n");
    }
}

string Operation::getInstrIR()
{
    return instrIR;
}
void Add::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "add\t" + instrIR;
}

void Sub::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "sub\t" + instrIR;
}

void Mul::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "mul\t" + instrIR;
}

void Div::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "div\t" + instrIR;
}

void Mod::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "mod\t" + instrIR;
}
void Copy::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "copy\t" + instrIR;
}

void Ldconst::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "ldconst\t" + instrIR;
}

void Rmem::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "rmem\t" + instrIR;
}

void Wmem::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "wmem\t" + instrIR;
}

void Call::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "call\t" + instrIR;
}

void Cmp_eq::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_eq\t" + instrIR;
}

void Cmp_lt::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_lt\t" + instrIR;
}

void Cmp_le::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Cmp_ge::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Cmp_gt::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Bite_or::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Bite_xor::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Bite_and::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Unary_different::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Unary_negate::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Return_::genIR(vector<string> params)
{
    Operation::genIR(params);
    instrIR = "cmp_le\t" + instrIR;
}

void Add::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %edx\n"
      << "\tmovl\t" << params[2] << "(%rbp), %eax\n"
      << "\taddl\t%edx, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Sub::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n" // left
      << "\tsubl\t" << params[2] << "(%rbp), %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Mul::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\timull\t" << params[2] << "(%rbp), %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)" << endl;
}

void Div::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n" // left
      << "\tcltd\n"                                  // case of division by 0
      << "\tidivl\t" << params[2] << "(%rbp)\n"      // right
      << "\tmovl\t%eax, " << params[0] << "(%rbp)" << endl;
}

void Ldconst::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t$" << params[1] << ", " << params[0] << "(%rbp)" << endl;
}

void Copy::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp)"
      << ", %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)" << endl;
}

void Rmem::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t %" << params[0] << ", " << params[1] << "(%rbp)" << endl;
}

void Wmem::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t " << params[0] << "(%rbp), %" << params[1] << endl;
}

void Call::gen_x86(vector<string> params, ostream &o)
{
    o << "\tcall\t " << params[0] << endl;
}

void Cmp_gt::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetg\t%al\n"         // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_ge::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetge\t%al\n"        // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_eq::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsete\t%al\n"         // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_lt::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetl\t%al\n"         // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_le::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetle\t%al\n"        // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_ne::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetne\t%al\n"        // %al is 0 or 1 (8 bits)
      << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Unary_negate::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tnegl\t%eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Unary_different::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmp\t$0," << params[1] << "(%rbp)\n"
      << "\tsetne\t%al\n"
      << "\txorb\t$1, %al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Bite_or::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\torl\t" << params[2] << "(%rbp), %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Bite_xor::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\txorl\t" << params[2] << "(%rbp), %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Bite_and::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tandl\t" << params[2] << "(%rbp), %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Return_::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[0] << "(%rbp), %eax\n";
}

// TODO: implement the following functions
// ArrayStore
// ArrayLoad
// ArrayDeclaration

void ArrayStore::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tmovl\t" << params[2] << "(%rbp), %edx\n"
      << "\tmovl\t%edx, " << params[0] << "(%rbp, %eax, 4)\n";
}

void ArrayStore::genIR(vector<string> params)
{
    instrIR = "ArrayStore"; // Revoir
}

void ArrayLoad::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tmovl\t$" << params[3] << ", " << params[2] << "(%rbp)\n"
      << "\timull\t" << params[2] << "(%rbp), %eax\n";
    // TODO: voir ajouter la valeur dans eax a la valeur base de l'array dans %eax

    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tmovl\t" << params[0] << "(%rbp, %eax, 4), %eax\n"
      << "\tmovl\t%eax, " << params[2] << "(%rbp)\n";
}

void ArrayLoad::genIR(vector<string> params)
{
    instrIR = "ArrayLoad"; // Revoir
}

void ArrayDeclaration::gen_x86(vector<string> params, ostream &o)
{

    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tmovl\t$" << params[3] << ", " << params[2] << "(%rbp)\n"
      << "\timull\t" << params[2] << "(%rbp), %eax\n"
      << "\tsubq\t%eax, %rsp\n";
}

void ArrayDeclaration::genIR(vector<string> params)
{
    instrIR = "ArrayDeclaration"; // Revoir
}