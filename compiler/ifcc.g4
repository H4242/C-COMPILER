grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration: 'int' VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

expr:
	expr OP expr	# muldiv
	| expr '+' expr	# add
	| expr '-' expr	# sub
	| CONST			# exprconst
	| VAR			# exprvar
	| '(' expr ')'	# exprpar;

CONST: '-'? [0-9]+;
COMMENT: '/*' .*? '*/' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
OP: ('*' | '/');
