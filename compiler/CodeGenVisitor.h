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
	virtual antlrcpp::Any visitConstexpr(ifccParser::ConstexprContext *ctx) override;
	virtual antlrcpp::Any visitVarexpr(ifccParser::VarexprContext *ctx) override;
	virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;
	virtual antlrcpp::Any visitMuldiv(ifccParser::MuldivContext *ctx) override;
	virtual antlrcpp::Any visitParexpr(ifccParser::ParexprContext *ctx) override;
	virtual antlrcpp::Any visitUnaryexpr(ifccParser::UnaryexprContext *ctx) override;
	virtual antlrcpp::Any visitBitexpr(ifccParser::BitexprContext *ctx) override;
	virtual antlrcpp::Any visitCompexpr(ifccParser::CompexprContext *ctx) override;

protected:
	SymbolTable *symbolTable = new SymbolTable();
	int currentOffset = 0;
	string temporaryGenerator();
};
