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

	for (auto &var : usedVariables)
	{
		if (!var.second)
		{
			cerr << "warning: variable '" << var.first << "' declared but not used\n";
		}
	}

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	currentFunctionName = "main";

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitFunctiondecl(ifccParser::FunctiondeclContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (declaredFunctions.find(funcName) != declaredFunctions.end())
	{
		throw std::logic_error("error: redeclaration of '" + funcName + "'");
	}
	Function func;
	func.name = funcName;
	func.returnType = ctx->retType->getText();
	func.paramsCount = ctx->declParams()->type().size();
	for (int i = 0; i < func.paramsCount; i++)
	{
		func.paramsTypes.push_back(ctx->declParams()->type(i)->getText());
	}
	declaredFunctions[funcName] = func;

	if (definedFunctions.find(funcName) != definedFunctions.end() && definedFunctions[funcName] != func)
	{
		throw std::logic_error("error: conflicting function signature for '" + funcName + "'");
	}

	currentFunctionName = funcName;

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitFunctiondef(ifccParser::FunctiondefContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (definedFunctions.find(funcName) != definedFunctions.end())
	{
		throw std::logic_error("error: redefinition of '" + funcName + "'");
	}
	Function func;
	func.name = funcName;
	func.returnType = ctx->retType->getText();
	func.paramsCount = ctx->defParams()->type().size();
	for (int i = 0; i < func.paramsCount; i++)
	{
		func.paramsTypes.push_back(ctx->defParams()->type(i)->getText());
	}
	definedFunctions[funcName] = func;

	if (declaredFunctions.find(funcName) != declaredFunctions.end() && declaredFunctions[funcName] != func)
	{
		throw std::logic_error("error: conflicting function signature for '" + funcName + "'");
	}

	currentFunctionName = funcName;

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitDeclParams(ifccParser::DeclParamsContext *ctx)
{
	vector<string> vars;
	int size = ctx->VAR().size();
	for (int i = 0; i < size; i++)
	{
		if (find(vars.begin(), vars.end(), ctx->VAR(i)->getText()) != vars.end())
		{
			throw std::logic_error("error: redeclaration of '" + ctx->VAR(i)->getText() + "' in function '" + currentFunctionName + "'");
		}
		vars.push_back(ctx->VAR(i)->getText());
	}
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitDefParams(ifccParser::DefParamsContext *ctx)
{
	int size = ctx->VAR().size();
	for (int i = 0; i < size; i++)
	{
		string varName = currentFunctionName + "_" + ctx->VAR(i)->getText();
		if (usedVariables.find(ctx->VAR(i)->getText()) != usedVariables.end())
		{
			throw std::logic_error("error: redeclaration of '" + ctx->VAR(i)->getText() + "' in function '" + currentFunctionName + "'");
		}
		usedVariables[varName] = false;
	}
	return 0;
}

antlrcpp::Any DeclarationVisitor::visitCallFunction(ifccParser::CallFunctionContext *ctx)
{
	string funcName = ctx->VAR()->getText();

	if (definedFunctions.find(funcName) == definedFunctions.end() && declaredFunctions.find(funcName) == declaredFunctions.end())
	{
		throw std::logic_error("error: '" + funcName + "' was not declared or defined before use");
	}

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
		string varName = currentFunctionName + "_" + ctx->VAR(i)->getText();
		if (usedVariables.find(varName) != usedVariables.end())
		{
			throw std::logic_error("error: redeclaration of '" + ctx->VAR(i)->getText() + "'");
		}
		usedVariables[varName] = false;
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
	string varName = currentFunctionName + "_" + name;
	if (usedVariables.find(varName) == usedVariables.end())
	{
		throw std::logic_error("error: '" + varName + "' undeclared");
	}
	usedVariables[varName] = true;
	visit(ctx->expr());

	return 0;
}

antlrcpp::Any DeclarationVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	string name = ctx->VAR()->getText();
	string varName = currentFunctionName + "_" + name;
	if (usedVariables.find(varName) == usedVariables.end())
	{
		throw std::logic_error("error: '" + varName + "' undeclared");
	}
	usedVariables[varName] = true;

	return name;
}