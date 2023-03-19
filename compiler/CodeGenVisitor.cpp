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

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
	if (symbolTable->existingVariable(ctx->VAR()->getText()))
	{
		// return error
		cout << "variable already declared ;)" << endl;
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
	return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssignvar(ifccParser::AssignvarContext *ctx)
{
	if (ctx->declaration())
	{
		visit(ctx->declaration());
		cout << "	movl 	" << symbolTable->getOffset(ctx->VAR(0)->getText()) << "(%rbp), %eax\n";
		cout << "	movl 	%eax, " << symbolTable->getOffset(ctx->declaration()->VAR()->getText()) << "(%rbp)\n";
	}
	cout << "	movl 	" << symbolTable->getOffset(ctx->VAR(0)->getText()) << "(%rbp), %eax\n";
	cout << "	movl 	%eax, " << symbolTable->getOffset(ctx->VAR(1)->getText()) << "(%rbp)\n";
	return 0;
}
