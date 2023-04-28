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

	BasicBlock *last_block = currentCFG->get_last_bb();
	currentCFG->add_bb(last_block);

	return 0;
}

antlrcpp::Any ASTVisitor::visitFunctiondecl(ifccParser::FunctiondeclContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (functionReturnType.find(funcName) == functionReturnType.end())
	{
		functionReturnType[funcName] = Type(ctx->type()->getText());
	}

	return 0;
}
antlrcpp::Any ASTVisitor::visitFunctiondef(ifccParser::FunctiondefContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	currentCFG = new CFG(funcName);
	cfgs.push_back(currentCFG);

	currentFunctionName = funcName;
	if (functionReturnType.find(funcName) == functionReturnType.end())
	{
		functionReturnType[funcName] = Type(ctx->type()->getText());
	}

	Operation *operation = new Rmem();
	if (ctx->defParams())
	{
		int size = ctx->defParams()->VAR().size();
		for (int i = 0; i < size; i++)
		{
			functionReturnType[funcName] = Type(ctx->type()->getText());
		}
		operation = new Rmem();
		for (int i = 0; i < size; i++)
		{
			string varName = funcName + "_" + ctx->defParams()->VAR(i)->getText();
			currentCFG->add_to_symbol_table(varName, Type(ctx->defParams()->type(i)->getText()));
			currentCFG->add_to_current_bb(operation, Type("void"), {registers[i], to_string(currentCFG->get_symbol_table_index()[varName])});
		}
	}
	visitChildren(ctx);
	BasicBlock *last_block = currentCFG->get_last_bb();
	currentCFG->add_bb(last_block);
	return 0;
}

antlrcpp::Any ASTVisitor::visitCallFunction(ifccParser::CallFunctionContext *ctx)
{
	string funcName = ctx->VAR()->getText();
	if (ctx->args())
	{
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
	}
	currentCFG->add_to_current_bb(new Call(), Type("void"), {funcName});
	return funcName;
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

		string var_type = currentCFG->get_var_type(var).getType();
		string rightExpr_type = currentCFG->get_var_type(rightExpr).getType();

		if (rightExpr_type == "void")
		{
			throw std::logic_error("error: void type can't be assigned to a variable");
		}

		Operation *operation = new Copy();
		currentCFG->add_to_current_bb(operation, currentCFG->get_var_type(var), {var_type, var_index, rightExpr_type, rightExpr_index});
	}

	return 0;
}

antlrcpp::Any ASTVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
	string var = currentFunctionName + "_" + ctx->VAR()->getText();
	string var_index = to_string(currentCFG->get_symbol_table_index()[var]);

	string rightExpr = visit(ctx->expr()).as<string>();
	string rightExpr_index = to_string(currentCFG->get_symbol_table_index()[rightExpr]);

	string var_type = currentCFG->get_var_type(var).getType();
	string rightExpr_type = currentCFG->get_var_type(rightExpr).getType();

	if (rightExpr_type == "void")
	{
		throw std::logic_error("error: void type can't be assigned to a variable");
	}

	Operation *operation = new Copy();
	currentCFG->add_to_current_bb(operation, currentCFG->get_var_type(var), {var_type, var_index, rightExpr_type, rightExpr_index});

	return 0;
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

antlrcpp::Any ASTVisitor::visitCharexpr(ifccParser::CharexprContext *ctx)
{
	Type type = Type("int");
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);
	currentCFG->add_const_to_symbol_table(name, ctx->CHAR()->getText()[1]);
	Operation *operation = new Ldconst();
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string const_value = to_string(currentCFG->get_symbol_table_const()[name]);
	currentCFG->add_to_current_bb(operation, type, {name_index, const_value, type.getType()});
	return name;
}

antlrcpp::Any ASTVisitor::visitConstexpr(ifccParser::ConstexprContext *ctx)
{
	Type type = Type("int");
	string name = currentCFG->create_new_tempvar(type, currentFunctionName);
	currentCFG->add_const_to_symbol_table(name, stoi(ctx->CONST()->getText()));
	Operation *operation = new Ldconst();
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	string const_value = to_string(currentCFG->get_symbol_table_const()[name]);
	currentCFG->add_to_current_bb(operation, type, {name_index, const_value, type.getType()});
	return name;
}

antlrcpp::Any ASTVisitor::visitVarexpr(ifccParser::VarexprContext *ctx)
{
	return currentFunctionName + "_" + ctx->VAR()->getText();
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

antlrcpp::Any ASTVisitor::visitCallexpr(ifccParser::CallexprContext *ctx)
{
	string funcName = visit(ctx->callFunction()).as<string>();
	Type type = functionReturnType[funcName];
	string returnVar = currentCFG->create_new_tempvar(type, currentFunctionName);
	Operation *operation = new Rmem();
	string returnVar_index = to_string(currentCFG->get_symbol_table_index()[returnVar]);
	currentCFG->add_to_current_bb(operation, type, {"eax", returnVar_index});

	return returnVar;
}

antlrcpp::Any ASTVisitor::visitStat_block(ifccParser::Stat_blockContext *ctx)
{
	visitChildren(ctx);
	return 0;
}

antlrcpp::Any ASTVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
	BasicBlock *test_bb = currentCFG->get_current_bb();
	BasicBlock *endif_bb = new BasicBlock(currentCFG->new_BB_name());
	auto exprs = ctx->expr();
	auto stat_blocks = ctx->stat_block();
	bool elseClause = false;
	for (int i = 0; i < stat_blocks.size(); i++)
	{
		auto stat_block = stat_blocks[i];
		string expr_name = "";
		if (i < exprs.size())
		{
			auto expr = exprs[i];
			currentCFG->set_current_bb(test_bb);
			expr_name = visit(expr).as<string>();
		}
		BasicBlock *then_bb = new BasicBlock(currentCFG->new_BB_name());
		then_bb->set_next_block(endif_bb);
		currentCFG->add_bb(then_bb);
		visit(stat_block);

		currentCFG->get_current_bb()->set_next_block(endif_bb);

		currentCFG->set_current_bb(then_bb);
		if (i < exprs.size())
		{
			Operation *operationCmp = new Cmp();
			string expr_name_index = to_string(currentCFG->get_symbol_table_index()[expr_name]);
			test_bb->add_IRInstr(operationCmp, Type("int"), {expr_name_index}); // cmp expr to 1 if eq jump
			Operation *operationJumpEqual = new JumpEqual();
			test_bb->add_IRInstr(operationJumpEqual, Type("int"), {then_bb->get_label()});
		}
		else
		{
			test_bb->set_next_block(currentCFG->get_current_bb());
			elseClause = true;
		}
	}
	if (!elseClause)
	{
		test_bb->set_next_block(endif_bb);
	}
	currentCFG->add_bb(endif_bb);
	return 0;
}

antlrcpp::Any ASTVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
	BasicBlock *test_bb = currentCFG->get_current_bb();
	BasicBlock *endwhile_bb = new BasicBlock(currentCFG->new_BB_name());

	auto expr = ctx->expr();
	auto stat_block = ctx->stat_block();

	BasicBlock *then_bb = new BasicBlock(currentCFG->new_BB_name());
	currentCFG->add_bb(then_bb);
	visit(stat_block);

	currentCFG->get_current_bb()->set_next_block(endwhile_bb);
	currentCFG->set_current_bb(endwhile_bb);

	string expr_name = visit(expr).as<string>();
	string expr_name_index = to_string(currentCFG->get_symbol_table_index()[expr_name]);
	Operation *operationCmp = new Cmp();
	endwhile_bb->add_IRInstr(operationCmp, Type("int"), {expr_name_index}); // cmp expr to 1 if eq jump

	Operation *operationJumpEqual = new JumpEqual();
	endwhile_bb->add_IRInstr(operationJumpEqual, Type("int"), {then_bb->get_label()});

	test_bb->set_next_block(endwhile_bb);
	currentCFG->add_bb(endwhile_bb);
	return 0;
}

antlrcpp::Any ASTVisitor::visitReturnstmt(ifccParser::ReturnstmtContext *ctx)
{
	if (ctx->expr())
	{
		string name = visit(ctx->expr()).as<string>();
		Type type = currentCFG->get_var_type(name);
		string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
		BasicBlock *last_block = currentCFG->get_last_bb();
		Operation *operation = new Return_();
		currentCFG->add_to_current_bb(operation, type, {name_index, last_block->get_label(), type.getType()});
	}
	return 0;
}

antlrcpp::Any ASTVisitor::visitPutchar(ifccParser::PutcharContext *ctx)
{
	string name = visit(ctx->expr()).as<string>();
	string name_index = to_string(currentCFG->get_symbol_table_index()[name]);
	Operation *operation = new PutChar();
	currentCFG->add_to_current_bb(operation, Type("int"), {name_index});
	currentCFG->set_putcharCall();
	return 0;
}

vector<CFG *> ASTVisitor::getCFGs()
{
	return cfgs;
}