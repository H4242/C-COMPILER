#include "DeclarationVisitor.h"
#include <any>

using namespace std;

antlrcpp::Any DeclarationVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
	visitChildren(ctx);

	for (auto &func : calledFunctions)
	{
		if (definedFunctions.find(func.name) == definedFunctions.end())
		{
			throw std::logic_error("error: undefined reference to '" + func.name + "'");
		}
		else if (func.paramsCount != definedFunctions[func.name].paramsCount)
		{
			throw std::logic_error("error: wrong number of arguments to function '" + func.name + "'");
		}
	}

	return 0;
}

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

antlrcpp::Any DeclarationVisitor::visitFunctiondecl(ifccParser::FunctiondeclContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (usedVariables.find(funcName) != usedVariables.end())
	{
		throw std::logic_error("error: redeclaration of '" + funcName + "'");
	}
	Function func;
	func.name = funcName;
	func.returnType = ctx->type()->getText();
	func.paramsCount = ctx->params()->type().size();
	for (int i = 0; i < func.paramsCount; i++)
	{
		func.paramsTypes.push_back(ctx->params()->type(i)->getText());
	}
	declaredFunctions[funcName] = func;

	if (definedFunctions.find(funcName) != definedFunctions.end() && definedFunctions[funcName] != func)
	{
		throw std::logic_error("error: conflicting types for '" + funcName + "'");
	}

	visitChildren(ctx);
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitFunctiondef(ifccParser::FunctiondefContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (usedVariables.find(funcName) != usedVariables.end())
	{
		throw std::logic_error("error: redefinition of '" + funcName + "'");
	}
	Function func;
	func.name = funcName;
	func.returnType = ctx->type()->getText();
	func.paramsCount = ctx->params()->type().size();
	for (int i = 0; i < func.paramsCount; i++)
	{
		func.paramsTypes.push_back(ctx->params()->type(i)->getText());
	}
	definedFunctions[funcName] = func;

	if (declaredFunctions.find(funcName) != declaredFunctions.end() && declaredFunctions[funcName] != func)
	{
		throw std::logic_error("error: conflicting types for '" + funcName + "'");
	}

	visitChildren(ctx);
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitCallFunction(ifccParser::CallFunctionContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	Function func;
	func.name = funcName;
	func.paramsCount = ctx->args()->expr().size();
	calledFunctions.push_back(func);
	visitChildren(ctx);
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
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

antlrcpp::Any DeclarationVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string name = ctx->VAR()->getText();
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