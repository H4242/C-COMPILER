#include "ASTVisitor.h"
#include <typeinfo>
#include <any>

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
	Type type = Type(typeid(ctx->expr()).name());
	string name = visit(ctx->expr()).as<string>();
	Operation operation = Return_();
	cfg->add_to_current_bb(operation, type, {name});
	return 0; // Dummy return
}

antlrcpp::Any ASTVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	int size = ctx->VAR().size();
	Type type = Type(typeid(ctx->VAR(0)).name());

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

		string rightExpr = visit(ctx->expr()).as<string>();

		Operation operation;
		if (ifccParser::ConstexprContext *v = dynamic_cast<ifccParser::ConstexprContext *>(ctx->expr()))
		{
			operation = Ldconst();
		}
		else
		{
			operation = Copy();
		}

		cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var, rightExpr});
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
	Operation operation;
	if (ifccParser::ConstexprContext *v = dynamic_cast<ifccParser::ConstexprContext *>(ctx->expr()))
	{
		operation = Ldconst();
	}
	else
	{
		operation = Copy();
	}

	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var, rightExpr});
	return 0;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	Type type = Type(typeid(ctx->CONST()).name());
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

	Type type = Type(typeid(ctx->expr(0)).name());
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_symbol_table(name, type);
	Operation operation;
	string OP = ctx->OPA()->getText();
	if (OP == "+")
	{
		operation = Add();
	}
	else
	{
		operation = Sub();
	}
	cfg->add_to_current_bb(operation, type, {name, left, right});
	return name;
}

antlrcpp::Any ASTVisitor::visitMuldiv(ifccParser::MuldivContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = Type(typeid(ctx->expr(0)).name());
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_symbol_table(name, type);
	Operation operation;
	string OP = ctx->OPM()->getText();
	if (OP == "*")
	{
		operation = Mul();
	}
	else if (OP == "/")
	{
		operation = Div();
	}
	else
	{
		operation = Mod();
	}
	cfg->add_to_current_bb(operation, type, {name, left, right});

	return name;
}

antlrcpp::Any ASTVisitor::visitParexpr(ifccParser::ParexprContext *ctx)
{
	return visit(ctx->expr()).as<string>();
}

antlrcpp::Any ASTVisitor::visitUnaryexpr(ifccParser::UnaryexprContext *ctx)
{
	Type type = Type(typeid(ctx->expr()).name());
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_symbol_table(name, type);

	Operation operation;
	string expr = visit(ctx->expr()).as<string>();

	string OP = ctx->OPU()->getText();
	if (OP == "-")
	{
		operation = Unary_negate();
	}
	else
	{
		operation = Unary_different();
	}
	cfg->add_to_current_bb(operation, type, {name, expr});

	return name;
}

antlrcpp::Any ASTVisitor::visitBitexpr(ifccParser::BitexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = Type(typeid(ctx->expr()).name());
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_symbol_table(name, type);

	Operation operation;

	string OP = ctx->OPB()->getText();
	if (OP == "&")
	{
		operation = Bite_and();
	}
	else if (OP == "|")
	{
		operation = Bite_or();
	}
	else
	{
		operation = Bite_xor();
	}

	cfg->add_to_current_bb(operation, type, {name, left, right});

	return name;
}

antlrcpp::Any ASTVisitor::visitCompexpr(ifccParser::CompexprContext *ctx)
{
	string left = visit(ctx->expr(0)).as<string>();
	string right = visit(ctx->expr(1)).as<string>();

	Type type = Type(typeid(ctx->expr(0)).name());
	string name = cfg->create_new_tempvar(type);
	cfg->add_to_symbol_table(name, type);

	Operation operation;

	string OP = ctx->OPC()->getText();
	if (OP == ">")
	{
		operation = Cmp_gt(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "<")
	{
		operation = Cmp_lt(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "==")
	{
		operation = Cmp_eq(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == ">=")
	{
		operation = Cmp_ge(); // %al is 0 or 1 (8 bits)
	}
	else if (OP == "<=")
	{
		operation = Cmp_le(); // %al is 0 or 1 (8 bits)
	}
	cfg->add_to_current_bb(operation, type, {name, left, right});

	return name;
}
