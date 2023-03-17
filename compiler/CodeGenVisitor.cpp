#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	std::cout << ".global	main\n"
				 " main: \n"
				 // prologue
				 "	pushq %rbp\n"
				 "	movq %rsp, %rbp\n";
	this->visit(ctx->return_stmt());
	// epilogue
	std::cout
		<< "	popq %rbp\n"
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
	std::cout << "hello";

	return 0;
}

// antlrcpp::Any CodeGenVisitor::visitAffect(ifccParser::AffectContext *ctx)
// {

// }

// antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
// {
// }
