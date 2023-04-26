#include "DeclarationVisitor.h"
#include <any>

using namespace std;

antlrcpp::Any DeclarationVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	visitChildren(ctx);

	for (auto &var : usedVariables)
	{
		if (!var.second)
		{
			cerr << "warning: variable '" << var.first << "' declared but not used\n";
		}
	}

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitSimpledeclaration(ifccParser::SimpledeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	for (int i = 0; i < size; i++)
	{
		if (usedVariables.find(ctx->VAR(i)->getText()) != usedVariables.end())
		{
			throw std::logic_error("error: redeclaration of '" + ctx->VAR(i)->getText() + "'");
		}
		usedVariables[ctx->VAR(i)->getText()] = false;
	}
	if (ctx->expr())
	{
		visit(ctx->expr());
	}

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitArraydeclaration(ifccParser::ArraydeclarationContext *ctx)
{
	if (usedVariables.find(ctx->VAR()->getText()) != usedVariables.end())
	{
		throw std::logic_error("error: redeclaration of '" + ctx->VAR()->getText() + "'");
	}
	usedVariables[ctx->VAR()->getText()] = false;

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string name = ctx->lvalue()->getText();
	if (usedVariables.find(name) == usedVariables.end())
	{
		throw std::logic_error("error: '" + name + "' undeclared");
	}
	visit(ctx->expr());
	usedVariables[name] = true;
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	string name = ctx->VAR()->getText();
	if (usedVariables.find(name) == usedVariables.end())
	{
		throw std::logic_error("error: '" + name + "' undeclared");
	}
	usedVariables[name] = true;

	return name;
}