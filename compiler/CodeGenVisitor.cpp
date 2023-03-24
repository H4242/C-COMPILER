#include "CodeGenVisitor.h"
#include "SymbolTable.h"

// extern SymbolTable *ptr;
antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	std::cout << ".globl	main\n"
				 " main: \n"
				 // prologue
				 "	pushq 	%rbp\n"
				 "	movq 	%rsp, %rbp\n";
	visitChildren(ctx);
	// epilogue
	std::cout
		<< "	popq 	%rbp\n"
		   " 	ret\n";

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnconst(ifccParser::ReturnconstContext *ctx)
{
	int retval = stoi(ctx->CONST()->getText());

	std::cout << " 	movl	$"
			  << retval << ", %eax\n";

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnvar(ifccParser::ReturnvarContext *ctx)
{
	string variable = ctx->VAR()->getText();
	// check if the variable doesn't exist
	if (!symbolTable->existingVariable(variable))
	{
		throw std::logic_error("you want to return a variable which was not declared");
	}
	cout << "	movl	" << symbolTable->variableTable[variable].getOffset() << "(%rbp), %eax\n";
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	if (symbolTable->existingVariable(ctx->VAR()->getText()))
	{
		// return error
		throw std::logic_error("variable declared twice");
	}
	symbolTable->addVariable(ctx->VAR()->getText(), currentOffset -= 4);
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignconst(ifccParser::AssignconstContext *ctx)
{
	string var;
	if (ctx->declaration())
	{
		visit(ctx->declaration());
		var = ctx->declaration()->VAR()->getText();
	}
	else
	{
		if (!symbolTable->existingVariable(ctx->VAR()->getText()))
		{
			// return error
			throw std::logic_error("assignment of undeclared variable");
		}
		var = ctx->VAR()->getText();
	}

	cout << "	movl 	$" << ctx->CONST()->getText() << ", " << currentOffset << "(%rbp)\n";
	symbolTable->variableTable[var].setValue(stoi(ctx->CONST()->getText()));

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignvar(ifccParser::AssignvarContext *ctx)
{
	if (symbolTable->variableTable.find(ctx->VAR(0)->getText()) == symbolTable->variableTable.end())
	{
		throw std::logic_error("variable not declared");
	}

	if (ctx->declaration())
	{
		visit(ctx->declaration());
		cout << "	movl 	" << symbolTable->variableTable[ctx->VAR(0)->getText()].getOffset() << "(%rbp), %eax\n";
		cout << "	movl 	%eax, " << symbolTable->variableTable[ctx->declaration()->VAR()->getText()].getOffset() << "(%rbp)\n";
		return 0;
	}
	cout << "	movl 	" << symbolTable->variableTable[ctx->VAR(1)->getText()].getOffset() << "(%rbp), %eax\n";
	cout << "	movl 	%eax, " << symbolTable->variableTable[ctx->VAR(0)->getText()].getOffset() << "(%rbp)\n";
	// update value in the hashmap
	symbolTable->variableTable[ctx->VAR(0)->getText()].setValue(symbolTable->variableTable[ctx->VAR(1)->getText()].getValue());
	return 0;
}