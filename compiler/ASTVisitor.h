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
	// 4.7
	vector<CFG *> getCFGs();
	// 4.9
	virtual antlrcpp::Any visitFunctiondef(ifccParser::FunctiondefContext *ctx) override;
	virtual antlrcpp::Any visitFunctiondecl(ifccParser::FunctiondeclContext *ctx) override;
	virtual antlrcpp::Any visitCallFunction(ifccParser::CallFunctionContext *ctx) override;
	virtual antlrcpp::Any visitCallexpr(ifccParser::CallexprContext *ctx) override;
	virtual antlrcpp::Any visitPutchar(ifccParser::PutcharContext *ctx) override;
	// 4.11
	virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
	virtual antlrcpp::Any visitStat_block(ifccParser::Stat_blockContext *ctx) override;
	// 4.13
	virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

protected:
	vector<CFG *> cfgs;
	CFG *currentCFG;
	vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
	string currentFunctionName = "";

	unordered_map<string, Type> functionReturnType = {};
};