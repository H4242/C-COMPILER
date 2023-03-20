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
		cout << ">> err: you want to return a variable which was not declared? looool" << endl;
	}
	int val = symbolTable->variableTable[variable].getValue();
	cout << "	movl	$" << val << ", %eax\n";
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	if (symbolTable->existingVariable(ctx->VAR()->getText()))
	{
		// return error
		cout << ">> err: do you have alzeimer? you declared same variable twice" << endl;
		return -1;
	}
	symbolTable->addVariable(ctx->VAR()->getText(), currentOffset -= 4);
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignconst(ifccParser::AssignconstContext *ctx)
{
	if (ctx->declaration())
	{
		visit(ctx->declaration());
	}
	cout << "	movl 	$" << ctx->CONST()->getText() << ", " << currentOffset << "(%rbp)\n";
	symbolTable->variableTable[ctx->declaration()->VAR()->getText()].setValue(stoi(ctx->CONST()->getText()));
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignvar(ifccParser::AssignvarContext *ctx)
{
	if (ctx->declaration())
	{
		visit(ctx->declaration());
		cout << "	movl 	" << symbolTable->variableTable[ctx->VAR(0)->getText()].getOffset() << "(%rbp), %eax\n";
		cout << "	movl 	%eax, " << symbolTable->variableTable[ctx->VAR(0)->getText()].getOffset() << "(%rbp)\n";
	}
	cout << "	movl 	" << symbolTable->variableTable[ctx->VAR(1)->getText()].getOffset() << "(%rbp), %eax\n";
	cout << "	movl 	%eax, " << symbolTable->variableTable[ctx->VAR(0)->getText()].getOffset() << "(%rbp)\n";
	// update value in the hashmap
	symbolTable->variableTable[ctx->VAR(0)->getText()].setValue(symbolTable->variableTable[ctx->VAR(1)->getText()].getValue());
	return 0;
}
