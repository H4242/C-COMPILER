#pragma once

#include <unordered_map>
#include "antlr4-runtime.h"
#include <string>
#include "generated/ifccBaseVisitor.h"
#include "Function.h"

using namespace std;

class DeclarationVisitor : public ifccBaseVisitor
{
public:
	virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
	virtual antlrcpp::Any visitAssignment(ifccParser::AssignmentContext *ctx) override;
	virtual antlrcpp::Any visitVarexpr(ifccParser::VarexprContext *ctx) override;
	virtual antlrcpp::Any visitFunctiondef(ifccParser::FunctiondefContext *ctx) override;
	virtual antlrcpp::Any visitFunctiondecl(ifccParser::FunctiondeclContext *ctx) override;
	virtual antlrcpp::Any visitDeclParams(ifccParser::DeclParamsContext *ctx) override;
	virtual antlrcpp::Any visitDefParams(ifccParser::DefParamsContext *ctx) override;
	virtual antlrcpp::Any visitCallFunction(ifccParser::CallFunctionContext *ctx) override;
	virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

protected:
	unordered_map<string, bool> usedVariables = {};
	unordered_map<string, Function> declaredFunctions = {};
	unordered_map<string, Function> definedFunctions = {};
	vector<Function> calledFunctions = {};
	string currentFunctionName = "";
	int currentBlockName = 0;
	stack<string> blockNameStack;
};
