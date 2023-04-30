#include "Operation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string Operation::getInstrIR()
{
    return instrIR;
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
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
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
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcltd\n"
      << "\tidivl\t" << params[2] << "(%rbp)\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)" << endl;
}

void Ldconst::gen_x86(vector<string> params, ostream &o)
{
    if (params[2] == "int")
    {
        o << "\tmovl\t$";
    }
    else if (params[2] == "char")
    {
        o << "\tmovb\t$";
    }
    o << params[1] << ", " << params[0] << "(%rbp)" << endl;
}

void Copy::gen_x86(vector<string> params, ostream &o)
{
    if (params[2] == "int")
    {
        o << "\tmovl\t" << params[3] << "(%rbp)"
          << ", %eax\n";
    }
    else if (params[2] == "char")
    {
        o << "\tmovb\t" << params[3] << "(%rbp)"
          << ", %al\n";
    }

    if (params[0] == "int")
    {
        if (params[2] == "char")
        {
            o << "\tmovzbl\t%al, %eax" << endl;
        }
        o << "\tmovl\t%eax, " << params[1] << "(%rbp)" << endl;
    }
    else if (params[0] == "char")
    {
        if (params[2] == "int")
        {
            o << "\tmovl\t$256, %ebx\n"
              << "\tcltd\n"
              << "\tidivl\t%ebx\n"
              << "\tmovl\t%edx, %eax\n";
        }
        o << "\tmovb\t%al, " << params[1] << "(%rbp)" << endl;
    }
}

void Rmem::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t%" << params[0] << ", " << params[1] << "(%rbp)" << endl;
}

void Wmem::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[0] << "(%rbp), %" << params[1] << endl;
}

void Call::gen_x86(vector<string> params, ostream &o)
{
    o << "\tcall\t" << params[0] << endl;
}

void Cmp_gt::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetg\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_ge::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetge\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_eq::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsete\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_lt::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetl\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_le::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetle\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)\n";
}

void Cmp_ne::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcmpl\t" << params[2] << "(%rbp), %eax\n"
      << "\tsetne\t%al\n"
      << "\tmovzbl\t%al, %eax\n"
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
    if (params[2] == "char")
    {
        o << "\tmovb\t" << params[0] << "(%rbp), %al\n"
          << "\tmovzbl\t%al, %eax" << endl;
    }
    else if (params[2] == "int")
    {
        o << "\tmovl\t" << params[0] << "(%rbp), %eax\n";
    }
    o << "\tjmp\t" << params[1] << "\n";
}

void Cmp::gen_x86(vector<string> params, ostream &o)
{
    o << "\tcmpl\t$0," << params[0] << "(%rbp)\n";
}

void JmpNotEqual::gen_x86(vector<string> params, ostream &o)
{
    o << "\tjne\t" << params[0] << "\n";
}

void Mod::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[1] << "(%rbp), %eax\n"
      << "\tcltd\n"
      << "\tidivl\t" << params[2] << "(%rbp)\n"
      << "\tmovl\t%edx, %eax\n"
      << "\tmovl\t%eax, " << params[0] << "(%rbp)" << endl;
}

void PutChar::gen_x86(vector<string> params, ostream &o)
{
    o << "\tmovl\t" << params[0] << "(% rbp), %edi\n "
      << "\tcall\tputchar@PLT\n";
}