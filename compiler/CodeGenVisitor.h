#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class CodeGenVisitor : public ifccBaseVisitor
{
public:
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
	// virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;
	// virtual antlrcpp::Any visitAffect(ifccParser::AffectContext *ctx) override;
	virtual antlrcpp::Any visitReturnvar(ifccParser::ReturnvarContext *ctx) override;
	virtual antlrcpp::Any visitReturnconst(ifccParser::ReturnconstContext *ctx) override;
};
