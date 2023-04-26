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
	int size = ctx->expr().size();
	if (size == 1)
	{
		string var = ctx->VAR()->getText(); // name of the array

		Type var_type = cfg->get_var_type(var);
		cfg->add_to_symbol_table(var, var_type);
		string var_index = to_string(cfg->get_symbol_table_index()[var]);
		string varsize = (var_type.getType() == INT) ? "4" : "1";

		// visit expr inside the brackets and get the name of the tmp var*
		string indexexpr_name = visit(ctx->expr(0)).as<string>();
		string indexexpr_index = to_string(cfg->get_symbol_table_index()[indexexpr_name]);

		// string size_name = visit(ctx->expr()).as<string>();
		// string size_index = to_string(cfg->get_symbol_table_index()[size_name]);

		Operation *operation = new ArrayDeclaration();
		cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, varsize, indexexpr_index});
	}
	else
	{
		antlrcpp::Any lvalue = visit(ctx->lvalue());
		string var;
		if (auto varlvalue = dynamic_cast<ifccParser::VarlvalueContext *>(lvalue.as<ifccParser::VarlvalueContext *>()))
		{
			var = varlvalue->VAR()->getText();
		}
		else if (auto arraylvalue = dynamic_cast<ifccParser::ArraylvalueContext *>(lvalue.as<ifccParser::VarlvalueContext *>()))
		{
			var = arraylvalue->VAR()->getText();
		}
		else
		{
			// handle error
		}

		string var_index = to_string(cfg->get_symbol_table_index()[var]);
		Type var_type = cfg->get_var_type(var);
		int offset = (var_type.getType() == INT) ? 4 : 1;
		int currOffset = 0;

		for (int i = 0; i < size; i++)
		{
			string stroffset = to_string(stoi(var_index) + currOffset);

			string expr = visit(ctx->expr(i)).as<string>();
			string expr_index = to_string(cfg->get_symbol_table_index()[expr]);

			Operation *operation = new Copy();
			cfg->add_to_current_bb(operation, cfg->get_var_type(var), {stroffset, expr_index});
			currOffset -= 4;
		}
	}
	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{

	// simple assignment
	string var = visit(ctx->lvalue()).as<string>();
	string var_index = to_string(cfg->get_symbol_table_index()[var]);

	string rightExpr = visit(ctx->expr()).as<string>();
	string rightExpr_index = to_string(cfg->get_symbol_table_index()[rightExpr]);

	Operation *operation = new Copy();
	cfg->add_to_current_bb(operation, cfg->get_var_type(var), {var_index, rightExpr_index});

	return 0;
}

antlrcpp::Any ASTVisitor::visitBracesassignment(ifccParser::BracesassignmentContext *ctx)
{
	antlrcpp::Any lvalue = visit(ctx->lvalue());
	string var;
	if (auto varlvalue = dynamic_cast<ifccParser::VarlvalueContext *>(lvalue.as<ifccParser::VarlvalueContext *>()))
	{
		var = varlvalue->VAR()->getText();
	}
	else if (auto arraylvalue = dynamic_cast<ifccParser::ArraylvalueContext *>(lvalue.as<ifccParser::VarlvalueContext *>()))
	{
		var = arraylvalue->VAR()->getText();
	}
	else
	{
		// handle error
	}

	string var_index = to_string(cfg->get_symbol_table_index()[var]);
	Type var_type = cfg->get_var_type(var);
	int offset = (var_type.getType() == INT) ? 4 : 1;
	int currOffset = 0;
	int size = ctx->expr().size();

	for (int i = 0; i < size; i++)
	{
		string stroffset = to_string(stoi(var_index) + currOffset);

		string expr = visit(ctx->expr(i)).as<string>();
		string expr_index = to_string(cfg->get_symbol_table_index()[expr]);

		Operation *operation = new Copy();
		cfg->add_to_current_bb(operation, cfg->get_var_type(var), {stroffset, expr_index});
		currOffset -= 4;
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitVarlvalue(ifccParser::VarlvalueContext *ctx)
{
	return ctx->VAR()->getText();
}

antlrcpp::Any ASTVisitor::visitArraylvalue(ifccParser::ArraylvalueContext *ctx)
{
	// get name and offset and type of the array
	string var = ctx->VAR()->getText();
	string var_index = to_string(cfg->get_symbol_table_index()[var]);
	Type var_type = cfg->get_var_type(var);

	// create a tmp var with the size of the type as value
	string sizevar_tmp = cfg->create_new_tempvar(Type("int"));
	// get the offset of this tmp var
	string sizevartmp_index = to_string(cfg->get_symbol_table_index()[sizevar_tmp]);

	// visit expr inside the brackets and get the name of the tmp var
	string indexexpr_name = visit(ctx->expr()).as<string>();
	// what does the next line do ?
	// cfg->add_const_to_symbol_table(indexExpr_name, var_type.getSize());
	// get offset of the tmp var
	string indexexpr_index = to_string(cfg->get_symbol_table_index()[indexexpr_name]);

	// create tmp that store result of var[expr]
	string name = cfg->create_new_tempvar(var_type);
	string name_index = to_string(cfg->get_symbol_table_index()[name]);

	Operation *operation = new ArrayLoad();

	cfg->add_to_current_bb(operation, var_type, {name_index, sizevartmp_index, var_index, indexexpr_index, to_string(var_type.getSize())});
	return var;
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