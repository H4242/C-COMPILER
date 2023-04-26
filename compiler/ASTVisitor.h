#pragma once

#include <unordered_map>
#include "antlr4-runtime.h"
#include <string>
#include "generated/ifccBaseVisitor.h"

#include "IR/CFG.h"

using namespace std;

class ASTVisitor : public ifccBaseVisitor
{
public:
	// 4.1
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	// 4.2
	virtual antlrcpp::Any visitReturnstmt(ifccParser::ReturnstmtContext *ctx) override;
	// 4.3
	virtual antlrcpp::Any visitSimpledeclaration(ifccParser::SimpledeclarationContext *ctx) override;
	// virtual antlrcpp::Any visitPointerdeclaration(ifccParser::PointerdeclarationContext *ctx) override;
	virtual antlrcpp::Any visitArraydeclaration(ifccParser::ArraydeclarationContext *ctx) override;
	virtual antlrcpp::Any visitAssignment(ifccParser::AssignmentContext *ctx) override;
	virtual antlrcpp::Any visitVarlvalue(ifccParser::VarlvalueContext *ctx) override;
	// virtual antlrcpp::Any visitPtrlvalue(ifccParser::PtrlvalueContext *ctx) override;
	virtual antlrcpp::Any visitArraylvalue(ifccParser::ArraylvalueContext *ctx) override;

	// 4.4
	virtual antlrcpp::Any visitConstexpr(ifccParser::ConstexprContext *ctx) override;
	virtual antlrcpp::Any visitVarexpr(ifccParser::VarexprContext *ctx) override;
	virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;
	virtual antlrcpp::Any visitMuldiv(ifccParser::MuldivContext *ctx) override;
	virtual antlrcpp::Any visitParexpr(ifccParser::ParexprContext *ctx) override;
	virtual antlrcpp::Any visitUnaryexpr(ifccParser::UnaryexprContext *ctx) override;
	virtual antlrcpp::Any visitBitexpr(ifccParser::BitexprContext *ctx) override;
	virtual antlrcpp::Any visitCompexpr(ifccParser::CompexprContext *ctx) override;
	virtual antlrcpp::Any visitLvalueexpr(ifccParser::LvalueexprContext *ctx) override;
	virtual antlrcpp::Any visitBracesassignment(ifccParser::BracesassignmentContext *ctx) override;
	// 4.7
	CFG *getCFG();

protected:
	CFG *cfg;
};
