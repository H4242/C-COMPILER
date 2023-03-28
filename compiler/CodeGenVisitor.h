#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"

class CodeGenVisitor : public ifccBaseVisitor
{
public:
	// 4.1
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	// 4.2
	virtual antlrcpp::Any visitReturnstmt(ifccParser::ReturnstmtContext *ctx) override;
	// 4.3
	virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
	virtual antlrcpp::Any visitAssignment(ifccParser::AssignmentContext *ctx) override;
	// 4.4
	virtual antlrcpp::Any visitExprconst(ifccParser::ExprconstContext *ctx) override;
	virtual antlrcpp::Any visitExprvar(ifccParser::ExprvarContext *ctx) override;
	virtual antlrcpp::Any visitAdd(ifccParser::AddContext *ctx) override;
	//  virtual antlrcpp::Any visitMuldiv(ifccParser::MuldivContext *ctx) override;
	//  virtual antlrcpp::Any visitSub(ifccParser::SubContext *ctx) override;
	//  virtual antlrcpp::Any visitExprpar(ifccParser::ExprparContext *ctx) override;

	// utils

protected:
	SymbolTable *symbolTable = new SymbolTable();
	int currentOffset = 0;
	string temporaryGenerator();
};
