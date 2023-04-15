#pragma once

#include <unordered_map>
#include "antlr4-runtime.h"
#include <string>
#include "generated/ifccBaseVisitor.h"

using namespace std;

class DeclarationVisitor : public ifccBaseVisitor
{
public:
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	virtual antlrcpp::Any visitSimpledeclaration(ifccParser::SimpledeclarationContext *ctx) override;
	virtual antlrcpp::Any visitAssignment(ifccParser::AssignmentContext *ctx) override;
	virtual antlrcpp::Any visitVarexpr(ifccParser::VarexprContext *ctx) override;

protected:
	unordered_map<string, bool> usedVariables = {};
};
