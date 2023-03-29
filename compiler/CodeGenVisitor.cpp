#include "CodeGenVisitor.h"
#include "SymbolTable.h"

using namespace std;

// utils
string CodeGenVisitor::temporaryGenerator()
{
	string name = "tmp" + to_string(symbolTable->variableTable.size());
	Variable tmp = Variable(name, currentOffset -= 4);
	symbolTable->variableTable[name] = tmp;
	return name;
}

// definitions
antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	std::cout << ".globl\tmain\n"
				 " main: \n"
				 // prologue
				 "\tpushq\t%rbp\n"
				 "\tmovq\t%rsp, %rbp\n";
	visitChildren(ctx);
	// epilogue
	std::cout
		<< "\tpopq\t%rbp\n"
		   "\tret\n";

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	string name = visit(ctx->expr()).as<string>();
	cout << "\tmovl\t" << symbolTable->variableTable[name].getOffset() << "(%rbp), %eax\n";
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	for (int i = 0; i < size; i++)
	{
		if (symbolTable->existingVariable(ctx->VAR(i)->getText()))
		{
			throw std::logic_error("variable declared twice");
		}
		symbolTable->addVariable(ctx->VAR(i)->getText(), currentOffset -= 4);
	}
	if (ctx->expr())
	{
		string var = ctx->VAR(size - 1)->getText();

		string rightExpr = visit(ctx->expr()).as<string>();

		cout << "\tmovl\t" << symbolTable->variableTable[rightExpr].getOffset() << "(%rbp)"
			 << ", %eax\n"
			 << "\tmovl\t %eax, " << symbolTable->variableTable[var].getOffset() << "(%rbp)" << endl;
	}

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	if (!symbolTable->existingVariable(ctx->VAR()->getText()))
	{
		throw std::logic_error("assignment of undeclared variable");
	}

	string var = ctx->VAR()->getText();

	string rightExpr = visit(ctx->expr()).as<string>();

	cout << "\tmovl\t" << symbolTable->variableTable[rightExpr].getOffset() << "(%rbp)"
		 << ", %eax\n"
		 << "\tmovl\t %eax, " << symbolTable->variableTable[var].getOffset() << "(%rbp)" << endl;

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprconst(ifccParser::ExprconstContext *ctx)
{
	string name = temporaryGenerator();

	cout << "\tmovl\t$" << ctx->CONST()->getText()
		 << "," << symbolTable->variableTable[name].getOffset() << "(%rbp)\n";

	return name;
}

antlrcpp::Any CodeGenVisitor::visitExprvar(ifccParser::ExprvarContext *ctx)
{
	string name = ctx->VAR()->getText();
	if (symbolTable->variableTable.find(name) == symbolTable->variableTable.end())
	{
		throw logic_error("assignment to an undeclared variable");
	}
	return name;
}

antlrcpp::Any CodeGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	string name = temporaryGenerator();

	string OP = ctx->OPA()->getText();
	if (OP == "+")
	{
		cout << "\tmovl\t" << symbolTable->variableTable[left].getOffset() << "(%rbp), %edx\n"
			 << "\tmovl\t" << symbolTable->variableTable[right].getOffset() << "(%rbp), %eax\n"
			 << "\taddl\t%edx, %eax\n"
			 << "\tmovl\t%eax, " << symbolTable->variableTable[name].getOffset() << "(%rbp)\n";
	}
	else
	{
		cout << "\tmovl\t" << symbolTable->variableTable[left].getOffset() << "(%rbp), %eax\n"
			 << "\tsubl\t" << symbolTable->variableTable[right].getOffset() << "(%rbp), %eax\n"
			 << "\tmovl\t%eax, " << symbolTable->variableTable[name].getOffset() << "(%rbp)\n";
	}
	return name;
}

antlrcpp::Any CodeGenVisitor::visitMuldiv(ifccParser::MuldivContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(0)).as<string>();

	string name = temporaryGenerator();

	string OP = ctx->OPM()->getText();
	if (OP == "*")
	{
		cout << "\tmovl\t" << symbolTable->variableTable[left].getOffset() << "(%rbp), %eax\n"
			 << "\timull\t" << symbolTable->variableTable[right].getOffset() << "(%rbp), %eax" << endl;
	}
	else
	{
		cout << "\tmovl\t" << symbolTable->variableTable[left].getOffset() << "(%rbp), %eax\n"
			 << "\tcltd\n" // case of division by 0
			 << "\tidivl\t" << symbolTable->variableTable[right].getOffset() << "(%rbp)" << endl;

		if (OP == "%")
		{
			cout << "\tmovl\t%edx, %eax" << endl;
		}
	}

	cout << "\tmovl\t%eax, " << symbolTable->variableTable[name].getOffset() << "(%rbp)\n"
		 << endl;

	return name;
}

antlrcpp::Any CodeGenVisitor::visitExprpar(ifccParser::ExprparContext *ctx)
{
	return visit(ctx->expr()).as<string>();
}
/*
int main(){
	int a,b,c;
	a=17;
	b=42;
	c=a*a + b*b +1;
	return c;
}
*/