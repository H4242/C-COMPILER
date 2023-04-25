#include "ASTVisitor.h"
#include "IR/Type.h"
#include <any>
#include "IR/CFG.h"

using namespace std;

antlrcpp::Any ASTVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	currentCFG = new CFG("main");
	cfgs.push_back(currentCFG);

	currentFunctionName = "main";

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any ASTVisitor::visitFunctiondef(ifccParser::FunctiondefContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	currentCFG = new CFG(funcName);
	cfgs.push_back(currentCFG);

	currentFunctionName = funcName;

	int size = ctx->defParams()->VAR().size();
	if (size > 6)
	{
		throw std::logic_error("error: function '" + funcName + "' can't have more than 6 arguments");
	}

	Operation *operation = new Rmem();
	for (int i = 0; i < size; i++)
	{
		string varName = funcName + "_" + ctx->defParams()->VAR(i)->getText();
		currentCFG->add_to_symbol_table(varName, Type(ctx->defParams()->type(i)->getText()));
		currentCFG->add_to_current_bb(operation, Type("void"), {registers[i], to_string(currentCFG->get_symbol_table_index()[varName])});
	}

	if (ctx->retType->getText() != "void")
	{
		currentCFG->add_to_symbol_table(funcName, Type(ctx->retType->getText()));
	}

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any ASTVisitor::visitCallexpr(ifccParser::CallexprContext *ctx)
{
	string funcName = visit(ctx->callFunction()).as<string>();
	if (currentCFG->get_symbol_table_type().find(funcName) == currentCFG->get_symbol_table_type().end())
	{
		throw std::logic_error("error: function '" + funcName + "' is of type void");
	}
	Type type = currentCFG->get_var_type(funcName);
	string returnVar = currentCFG->create_new_tempvar(type, currentFunctionName);
	Operation *operation = new Rmem();
	string returnVar_index = to_string(currentCFG->get_symbol_table_index()[returnVar]);
	currentCFG->add_to_current_bb(operation, type, {"eax", returnVar_index});

	return returnVar;
}

antlrcpp::Any ASTVisitor::visitCallFunction(ifccParser::CallFunctionContext *ctx)
{
	string funcName = ctx->VAR()->getText();

	int size = ctx->args()->expr().size();
	if (size > 6)
	{
		throw std::logic_error("error: a function can't have more than 6 arguments");
	}

	Operation *operation = new Wmem();
	for (int i = 0; i < size; i++)
	{
		string arg = visit(ctx->args()->expr(i)).as<string>();
		currentCFG->add_to_current_bb(operation, Type("void"), {to_string(currentCFG->get_symbol_table_index()[arg]), registers[i]});
	}
	currentCFG->add_to_current_bb(new Call(), Type("void"), {funcName});

	return funcName;
}

antlrcpp::Any ASTVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	if (ctx->expr())
	{
		string name = visit(ctx->expr()).as<string>();
		Type type = currentCFG->get_var_type(name);
		Operation *operation = new Return_();
		string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
		currentCFG->add_to_current_bb(operation, type, {name_index});
	}
	return 0;
}

antlrcpp::Any ASTVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	Type type = Type(ctx->type()->getText());

	for (int i = 0; i < size; i++)
	{
		currentCFG->add_to_symbol_table(currentFunctionName + "_" + ctx->VAR(i)->getText(), type);
	}
	if (ctx->expr())
	{
		string var = currentFunctionName + "_" + ctx->VAR(size - 1)->getText();
		string var_index = to_string(currentCFG->get_symbol_table_index()[var]);

		string rightExpr = visit(ctx->expr()).as<string>();
		string rightExpr_index = to_string(currentCFG->get_symbol_table_index()[rightExpr]);

		Operation *operation = new Copy();
		currentCFG->add_to_current_bb(operation, currentCFG->get_var_type(var), {var_index, rightExpr_index});
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string var = currentFunctionName + "_" + ctx->VAR()->getText();
	string var_index = to_string(currentCFG->get_symbol_table_index()[var]);

	string rightExpr = visit(ctx->expr()).as<string>();
	string rightExpr_index = to_string(currentCFG->get_symbol_table_index()[rightExpr]);

	if (currentCFG->get_var_type(var) != currentCFG->get_var_type(rightExpr))
	{
		throw std::logic_error("error: type mismatch");
	}

	Operation *operation = new Copy();
	currentCFG->add_to_current_bb(operation, currentCFG->get_var_type(var), {var_index, rightExpr_index});

	return 0;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	Type type = Type("int");
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);
	currentCFG->add_const_to_symbol_table(name, stoi(ctx->CONST()->getText()));
	Operation *operation = new Ldconst();
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string const_value = to_string(currentCFG->get_symbol_table_const()[name]);
	currentCFG->add_to_current_bb(operation, type, {name_index, const_value});
	return name;
}

antlrcpp::Any ASTVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	return currentFunctionName + "_" + ctx->VAR()->getText();
}

antlrcpp::Any ASTVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	// We assume that both expressions are of the same type
	Type type = currentCFG->get_var_type(left);

	string name = currentCFG->create_new_tempvar(type, currentFunctionName);

	Operation *operation;
	string OP = ctx->op->getText();
	if (OP == "+")
	{
		operation = new Add();
	}
	else
	{
		operation = new Sub();
	}
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string left_index = to_string(currentCFG->get_symbol_table_index()[left]);
	string right_index = to_string(currentCFG->get_symbol_table_index()[right]);

	currentCFG->add_to_current_bb(operation, type, {name_index, left_index, right_index});
	return name;
}

antlrcpp::Any ASTVisitor::visitMuldiv(ifccParser::MuldivContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = currentCFG->get_var_type(left);

	string name = currentCFG->create_new_tempvar(type, currentFunctionName);

	Operation *operation;
	string OP = ctx->op->getText();
	if (OP == "*")
	{
		operation = new Mul();
	}
	else if (OP == "/")
	{
		operation = new Div();
	}
	else
	{
		operation = new Mod();
	}

	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string left_index = to_string(currentCFG->get_symbol_table_index()[left]);
	string right_index = to_string(currentCFG->get_symbol_table_index()[right]);

	currentCFG->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

antlrcpp::Any ASTVisitor::visitParexpr(ifccParser::ParexprContext *ctx)
{
	return visit(ctx->expr()).as<string>();
}

antlrcpp::Any ASTVisitor::visitUnaryexpr(ifccParser::UnaryexprContext *ctx)
{
	Operation *operation;
	string expr = visit(ctx->expr()).as<string>();

	Type type = currentCFG->get_var_type(expr);
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);

	string OP = ctx->op->getText();
	if (OP == "-")
	{
		operation = new Unary_negate();
	}
	else
	{
		operation = new Unary_different();
	}
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string expr_index = to_string(currentCFG->get_symbol_table_index()[expr]);

	currentCFG->add_to_current_bb(operation, type, {name_index, expr_index});
	return name;
}

antlrcpp::Any ASTVisitor::visitBitexpr(ifccParser::BitexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = currentCFG->get_var_type(left);
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);

	Operation *operation;

	string OP = ctx->op->getText();
	if (OP == "&")
	{
		operation = new Bite_and();
	}
	else if (OP == "|")
	{
		operation = new Bite_or();
	}
	else
	{
		operation = new Bite_xor();
	}

	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string left_index = to_string(currentCFG->get_symbol_table_index()[left]);
	string right_index = to_string(currentCFG->get_symbol_table_index()[right]);

	currentCFG->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

antlrcpp::Any ASTVisitor::visitCompexpr(ifccParser::CompexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = currentCFG->get_var_type(left);
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);

	Operation *operation;

	string OP = ctx->op->getText();
	if (OP == ">")
	{
		operation = new Cmp_gt();
	}
	else if (OP == "<")
	{
		operation = new Cmp_lt();
	}
	else if (OP == "==")
	{
		operation = new Cmp_eq();
	}
	else if (OP == ">=")
	{
		operation = new Cmp_ge();
	}
	else if (OP == "<=")
	{
		operation = new Cmp_le();
	}
	else if (OP == "!=")
	{
		operation = new Cmp_ne();
	}

	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string left_index = to_string(currentCFG->get_symbol_table_index()[left]);
	string right_index = to_string(currentCFG->get_symbol_table_index()[right]);

	currentCFG->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

vector<CFG *> ASTVisitor::getCFGs()
{
	return cfgs;
}