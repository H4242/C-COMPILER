#include "ASTVisitor.h"
#include <any>

using namespace std;

// utils
string ASTVisitor::temporaryGenerator()
{
	string name = "t_" + to_string(symbolTable.size());
	currentOffset -= 4;
	symbolTable[name] = currentOffset;
	return name;
}

// definitions
antlrcpp::Any ASTVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	cfg = new CFG("main");

	visitChildren(ctx);

	for (auto &var : usedVariables)
	{
		if (!var.second)
		{
			cerr << "warning: variable " << var.first << " declared but not used\n";
		}
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	string name = visit(ctx->expr()).as<string>();
	cout << "\tmovl\t" << symbolTable[name] << "(%rbp), %eax\n";
	return 0; // Dummy return
}

antlrcpp::Any ASTVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	for (int i = 0; i < size; i++)
	{
		if (symbolTable.find(ctx->VAR(i)->getText()) != symbolTable.end())
		{
			throw std::logic_error("error: variable declared twice");
		}
		currentOffset -= 4;
		symbolTable[ctx->VAR(i)->getText()] = currentOffset;
		usedVariables[ctx->VAR(i)->getText()] = false;
	}
	if (ctx->expr())
	{
		string var = ctx->VAR(size - 1)->getText();

		string rightExpr = visit(ctx->expr()).as<string>();

		cout << "\tmovl\t" << symbolTable[rightExpr] << "(%rbp)"
			 << ", %eax\n"
			 << "\tmovl\t %eax, " << symbolTable[var] << "(%rbp)" << endl;
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	if (symbolTable.find(ctx->VAR()->getText()) == symbolTable.end())
	{
		throw std::logic_error("error: assignment of undeclared variable");
	}

	string var = ctx->VAR()->getText();

	string rightExpr = visit(ctx->expr()).as<string>();

	cout << "\tmovl\t" << symbolTable[rightExpr] << "(%rbp)"
		 << ", %eax\n"
		 << "\tmovl\t %eax, " << symbolTable[var] << "(%rbp)" << endl;

	usedVariables[var] = true;

	return 0;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	string name = temporaryGenerator();

	cout << "\tmovl\t$" << ctx->CONST()->getText()
		 << "," << symbolTable[name] << "(%rbp)\n";

	return name;
}

antlrcpp::Any ASTVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	string name = ctx->VAR()->getText();
	if (symbolTable.find(name) == symbolTable.end())
	{
		throw logic_error("error: undeclared variable");
	}
	usedVariables[name] = true;
	return name;
}

antlrcpp::Any ASTVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	string name = temporaryGenerator();

	string OP = ctx->OPA()->getText();
	if (OP == "+")
	{
		cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %edx\n"
			 << "\tmovl\t" << symbolTable[right] << "(%rbp), %eax\n"
			 << "\taddl\t%edx, %eax\n"
			 << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n";
	}
	else
	{
		cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %eax\n"
			 << "\tsubl\t" << symbolTable[right] << "(%rbp), %eax\n"
			 << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n";
	}
	return name;
}

antlrcpp::Any ASTVisitor::visitMuldiv(ifccParser::MuldivContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	string name = temporaryGenerator();

	string OP = ctx->OPM()->getText();
	if (OP == "*")
	{
		cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %eax\n"
			 << "\timull\t" << symbolTable[right] << "(%rbp), %eax" << endl;
	}
	else
	{
		cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %eax\n"
			 << "\tcltd\n" // case of division by 0
			 << "\tidivl\t" << symbolTable[right] << "(%rbp)" << endl;

		if (OP == "%")
		{
			cout << "\tmovl\t%edx, %eax" << endl;
		}
	}

	cout << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n"
		 << endl;

	return name;
}

antlrcpp::Any ASTVisitor::visitParexpr(ifccParser::ParexprContext *ctx)
{
	return visit(ctx->expr()).as<string>();
}

antlrcpp::Any ASTVisitor::visitUnaryexpr(ifccParser::UnaryexprContext *ctx)
{
	string name = temporaryGenerator();

	string expr = visit(ctx->expr()).as<string>();

	cout << "\tmovl\t" << symbolTable[expr] << "(%rbp), %eax\n";

	string OP = ctx->OPU()->getText();
	if (OP == "-")
	{
		cout << "\tnegl\t%eax\n";
	}
	else
	{
		cout << "\tnotl\t%eax\n";
	}

	cout << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n";

	return name;
}

antlrcpp::Any ASTVisitor::visitBitexpr(ifccParser::BitexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	string name = temporaryGenerator();

	cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %eax\n";

	string OP = ctx->OPB()->getText();
	if (OP == "&")
	{
		cout << "\tandl\t" << symbolTable[right] << "(%rbp), %eax\n";
	}
	else if (OP == "|")
	{
		cout << "\torl\t" << symbolTable[right] << "(%rbp), %eax\n";
	}
	else
	{
		cout << "\txorl\t" << symbolTable[right] << "(%rbp), %eax\n";
	}

	cout << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n";

	return name;
}

antlrcpp::Any ASTVisitor::visitCompexpr(ifccParser::CompexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	string name = temporaryGenerator();

	cout << "\tmovl\t" << symbolTable[left] << "(%rbp), %eax\n"
		 << "\tcmpl\t" << symbolTable[right] << "(%rbp), %eax\n";

	string OP = ctx->OPC()->getText();
	if (OP == ">")
	{
		cout << "\tsetg\t%al\n"; // %al is 0 or 1 (8 bits)
	}
	else if (OP == "<")
	{
		cout << "\tsetl\t%al\n"; // %al is 0 or 1 (8 bits)
	}
	else if (OP == "==")
	{
		cout << "\tsete\t%al\n"; // %al is 0 or 1 (8 bits)
	}
	else
	{
		cout << "\tsetne\t%al\n"; // %al is 0 or 1 (8 bits)
	}

	cout << "\tmovzbl\t%al, %eax\n" // movzbl : convert 8 bits to 32 bits
		 << "\tmovl\t%eax, " << symbolTable[name] << "(%rbp)\n";

	return name;
}
