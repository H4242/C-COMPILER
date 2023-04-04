grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration: 'int' VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

expr:
	op=('-' | '!') expr							# unaryexpr
	|expr op=('*' | '/') expr					# muldiv
	| expr op=('+' | '-') expr					# addsub
	| expr op=('<' | '>' | '==' | '!=') expr	# compexpr
	| expr op=('&' | '|' | '^') expr			# bitexpr
	| CONST										# constexpr
	| VAR										# varexpr
	| '(' expr ')'								# parexpr;

CONST: [0-9]+;
COMMENT: ('/*' .*? '*/'| '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);