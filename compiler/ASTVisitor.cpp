#include "ASTVisitor.h"
#include "IR/Type.h"
#include <any>
#include "IR/CFG.h"

using namespace std;

// definitions
antlrcpp::Any ASTVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	cfg = new CFG("main");

	visitChildren(ctx);

	return 0;
}

antlrcpp::Any ASTVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	string name = visit(ctx->expr()).as<string>();
	Type type = cfg->get_var_type(name);
	Operation *operation = new Return_();
	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	cfg->add_to_current_bb(operation, type, {name_index});
	return 0;
}

antlrcpp::Any ASTVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	Type type = Type(ctx->type->getText());

	for (int i = 0; i < size; i++)
	{
		cfg->add_to_symbol_table(ctx->VAR(i)->getText(), type);
	}
	if (ctx->expr())
	{
		string var = ctx->VAR(size - 1)->getText();
		string var_index = to_string(cfg->get_symbol_table_index()[var]);

		string rightExpr = visit(ctx->expr()).as<string>();
		string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);

		Operation *operation = new Copy();
		cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_index});
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string var = ctx->VAR()->getText();
	string var_index = to_string(cfg->get_symbol_table_index()[var]);

	string rightExpr = visit(ctx->expr()).as<string>();
	string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);

	Operation *operation = new Copy();
	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_index});

	return 0;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	Type type = Type("int");
	string name = cfg->create_new_tempvar(type);
	cfg->add_const_to_symbol_table(name, stoi(ctx->CONST()->getText()));
	Operation *operation = new Ldconst();
	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string const_value = to_string(cfg->get_symbol_table_const()[name]);
	cfg->add_to_current_bb(operation, type, {name_index, const_value});
	return name;
}

antlrcpp::Any ASTVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	return ctx->VAR()->getText();
}

antlrcpp::Any ASTVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	// We assume that both expressions are of the same type
	Type type = cfg->get_var_type(left);

	string name = cfg->create_new_tempvar(type);

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
	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string left_index = to_string(cfg->get_symbol_table_index()[left]);
	string right_index = to_string(cfg->get_symbol_table_index()[right]);

	cfg->add_to_current_bb(operation, type, {name_index, left_index, right_index});
	return name;
}

antlrcpp::Any ASTVisitor::visitMuldiv(ifccParser::MuldivContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = cfg->get_var_type(left);

	string name = cfg->create_new_tempvar(type);

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

	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string left_index = to_string(cfg->get_symbol_table_index()[left]);
	string right_index = to_string(cfg->get_symbol_table_index()[right]);

	cfg->add_to_current_bb(operation, type, {name_index, left_index, right_index});

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

	Type type = cfg->get_var_type(expr);
	string name = cfg->create_new_tempvar(type);

	string OP = ctx->op->getText();
	if (OP == "-")
	{
		operation = new Unary_negate();
	}
	else
	{
		operation = new Unary_different();
	}
	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string expr_index = to_string(cfg->get_symbol_table_index()[expr]);

	cfg->add_to_current_bb(operation, type, {name_index, expr_index});
	return name;
}

antlrcpp::Any ASTVisitor::visitBitexpr(ifccParser::BitexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = cfg->get_var_type(left);
	string name = cfg->create_new_tempvar(type);

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

	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string left_index = to_string(cfg->get_symbol_table_index()[left]);
	string right_index = to_string(cfg->get_symbol_table_index()[right]);

	cfg->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

antlrcpp::Any ASTVisitor::visitCompexpr(ifccParser::CompexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = cfg->get_var_type(left);
	string name = cfg->create_new_tempvar(type);

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

	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string left_index = to_string(cfg->get_symbol_table_index()[left]);
	string right_index = to_string(cfg->get_symbol_table_index()[right]);

	cfg->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

antlrcpp::Any ASTVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
	BasicBlock *test_bb = cfg->get_current_bb();
	BasicBlock *then_bb = new BasicBlock(cfg->new_BB_name());
	BasicBlock *then_last_bb = cfg->get_current_bb();
	BasicBlock *else_bb = new BasicBlock(cfg->new_BB_name());
	BasicBlock *else_last_bb = cfg->get_current_bb();

	BasicBlock *endif_bb = new BasicBlock(cfg->new_BB_name());

	string expr_condition = visit(ctx->condition_block()->expr()).as<string>();
	// Write the condition in the test_bb

	test_bb->set_exit_true(then_bb);
	test_bb->set_exit_false(else_bb);

	then_last_bb->set_exit_true(endif_bb);
	else_last_bb->set_exit_true(endif_bb);

	then_last_bb->set_exit_false(NULL);
	else_last_bb->set_exit_false(NULL);

	cfg->set_current_bb(endif_bb);

	cfg->add_bb(then_bb);
	cfg->add_bb(else_bb);
	cfg->add_bb(endif_bb);

	string result = visit(ctx->condition_block()->stat_block());
	return result;
	// TODO: check if the return is correct
}

CFG *ASTVisitor::getCFG()
{
	return cfg;
}