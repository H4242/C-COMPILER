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

	for (auto &var : cfg->get_symbol_table_used())
	{
		if (!var.second)
		{
			cerr << "warning: variable " << var.first << " declared but not used\n";
		}
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	string name = visit(ctx->expr()).as<string>();
	Type type = cfg->get_var_type(name);
	Operation *operation = new Return_();
	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	cfg->add_to_current_bb(operation, type, {name_index});
	return 0; // Dummy return
}

antlrcpp::Any ASTVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	Type type = Type(ctx->type->getText());

	for (int i = 0; i < size; i++)
	{
		if (cfg->is_in_symbol_table(ctx->VAR(i)->getText()))
		{
			throw std::logic_error("error: variable declared twice");
		}
		cfg->add_to_symbol_table(ctx->VAR(i)->getText(), type);
	}
	if (ctx->expr())
	{
		string var = ctx->VAR(size - 1)->getText();
		string var_index = to_string(cfg->get_symbol_table_index()[var]);

		string rightExpr = visit(ctx->expr()).as<string>();

		Operation *operation;
		if (ifccParser::ConstexprContext *v = dynamic_cast<ifccParser::ConstexprContext *>(ctx->expr()))
		{
			operation = new Ldconst();
			string rightExpr_value = to_string(cfg->get_symbol_table_const()[rightExpr]);
			cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_value});
		}
		else
		{
			operation = new Copy();
			string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);
			cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_index});
		}
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	if (!cfg->is_in_symbol_table(ctx->VAR()->getText()))
	{
		throw std::logic_error("error: assignment of undeclared variable");
	}

	string var = ctx->VAR()->getText();
	string rightExpr = visit(ctx->expr()).as<string>();
	Operation *operation;
	if (ifccParser::ConstexprContext *v = dynamic_cast<ifccParser::ConstexprContext *>(ctx->expr()))
	{
		operation = new Ldconst();
	}
	else
	{
		operation = new Copy();
	}

	string var_index = to_string(cfg->get_symbol_table_index()[var]);
	string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);

	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_index});

	return 0;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	Type type = Type("int");
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_const_symbol(name, stoi(ctx->CONST()->getText()));
	return name;
}

antlrcpp::Any ASTVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	string name = ctx->VAR()->getText();
	if (!cfg->is_in_symbol_table(name))
	{
		throw logic_error("error: undeclared variable");
	}
	cfg->set_var_used(name);
	return name;
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
		operation = new Cmp_gt(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "<")
	{
		operation = new Cmp_lt(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "==")
	{
		operation = new Cmp_eq(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == ">=")
	{
		operation = new Cmp_ge(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "<=")
	{
		operation = new Cmp_le(); // %al is 0 or 1 (8 bits)
	}

	string name_index = to_string(cfg->get_symbol_table_index()[name]);
	string left_index = to_string(cfg->get_symbol_table_index()[left]);
	string right_index = to_string(cfg->get_symbol_table_index()[right]);

	cfg->add_to_current_bb(operation, type, {name_index, left_index, right_index});

	return name;
}

CFG *ASTVisitor::getCFG()
{
	return cfg;
}