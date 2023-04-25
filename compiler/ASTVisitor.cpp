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

antlrcpp::Any ASTVisitor::visitSimpledeclaration(ifccParser::SimpledeclarationContext *ctx)
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

antlrcpp::Any ASTVisitor::visitArraydeclaration(ifccParser::ArraydeclarationContext *ctx)
{
	string type_str = ctx->type->getText();
	Type type = Type(type_str);

	string var = ctx->VAR()->getText(); // name of the array
	string var_index = to_string(cfg->get_symbol_table_index()[var]);

	string size_name = visit(ctx->expr()).as<string>();
	string size_index = to_string(cfg->get_symbol_table_index()[size_name]);

	string tmpVarSize_name = cfg->create_new_tempvar(Type("int"));
	cfg->add_const_to_symbol_table(tmpVarSize_name, type.getSize());
	string tmpVarSize_index = to_string(cfg->get_symbol_table_index()[tmpVarSize_name]);

	// Type type = Type(ctx->type->getText());
	// TODO gerer le type

	Operation *operation = new ArrayDeclaration();
	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, size_index, tmpVarSize_index, to_string(type.getSize())});
	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string var = visit(ctx->lvalue()).as<string>();
	string var_index = to_string(cfg->get_symbol_table_index()[var]);

	// simple assignment
	string rightExpr = visit(ctx->expr()).as<string>();
	string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);

	Operation *operation = new Copy();
	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {size_index, tmpVarSize_index, to_string(type.getSize())});

	return 0;
}

antlrcpp::Any ASTVisitor::visitVarlvalue(ifccParser::VarlvalueContext *ctx)
{
	return ctx->VAR()->getText();
}

antlrcpp::Any ASTVisitor::visitArraylvalue(ifccParser::ArraylvalueContext *ctx)
{
	string type_str = ctx->type->getText();
	Type type = Type(type_str);

	string var = ctx->VAR()->getText();
	string var_index = to_string(cfg->get_symbol_table_index()[var]);

	string size_name = visit(ctx->expr()).as<string>();
	string size_index = to_string(cfg->get_symbol_table_index()[size_name]);

	string indexExpr_name = cfg->create_new_tempvar(Type("int"));
	cfg->add_const_to_symbol_table(indexExpr_name, type.getSize());
	string indexExpr_index = to_string(cfg->get_symbol_table_index()[tmpVarSize_name]);

	Operation *operation = new ArrayLoad();

	cfg->add_to_current_bb(operation, type, {size_index, var_index, indexExpr_index, to_string(type.getSize())});
	return name;
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

antlrcpp::Any ASTVisitor::visitLvalueexpr(ifccParser::LvalueexprContext *ctx)
{
	return visit(ctx->lvalue());
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

CFG *ASTVisitor::getCFG()
{
	return cfg;
}