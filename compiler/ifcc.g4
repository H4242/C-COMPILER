grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration:
	type = ('int' | 'char') VAR (',' VAR)* ('=' expr)? ';'	# simpledeclaration
	| type = ('int' | 'char') VAR '*' ';'					# pointerdeclaration;

assignment:
	lvalue '=' expr ';'					# simpleassignment
	| lvalue '[' expr ']' '=' expr ';'	# arrayassignment
	| lvalue '.' VAR '=' expr ';'		# structassignment
	| lvalue '->' VAR '=' expr ';'		# ptrstructassignment
	| lvalue '*' '=' expr ';'			# pointerassignment;

lvalue:
	VAR						# varlvalue
	| lvalue '[' expr ']'	# arraylvalue
	| lvalue '.' VAR		# structlvalue
	| lvalue '->' VAR		# ptrstructlvalue
	| '*' lvalue			# ptrlvalue;

expr:
	op = ('-' | '!') expr						# unaryexpr
	| expr op = ('*' | '/') expr				# muldiv
	| expr op = ('+' | '-') expr				# addsub
	| expr op = ('<' | '>' | '==' | '!=') expr	# compexpr
	| expr op = ('&' | '|' | '^') expr			# bitexpr
	| CONST										# constexpr
	| VAR										# varexpr
	| lvalue									# lvalueexpr
	| '(' expr ')'								# parexpr;

CONST: [0-9]+;
COMMENT: ('/*' .*? '*/' | '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
