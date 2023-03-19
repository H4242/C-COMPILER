#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"

class CodeGenVisitor : public ifccBaseVisitor
{
public:
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	// virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;
	virtual antlrcpp::Any visitReturnvar(ifccParser::ReturnvarContext *ctx) override;
	virtual antlrcpp::Any visitReturnconst(ifccParser::ReturnconstContext *ctx) override;
	virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
	virtual antlrcpp::Any visitAssignconst(ifccParser::AssignconstContext *ctx) override;
	virtual antlrcpp::Any visitAssignvar(ifccParser::AssignvarContext *ctx) override;

protected:
	SymbolTable *symbolTable = new SymbolTable();
	int currentOffset = 0;
};