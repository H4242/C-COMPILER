#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
	// int retval = stoi(ctx->CONST()->getText());
	std::cout << ".globl	main\n"
				 " main: \n";

	//		" 	movl	$"<<retval<<", %eax\n"

	this->visit(ctx->return_stmt());

	std::cout << " 	ret\n";

	return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
	std::cout << "hello\n"
			  << ctx->CONST()->getText() << std::endl;

	return 0;
}
