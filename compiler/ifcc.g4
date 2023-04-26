grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (bracesassignment | assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration:
	type = ('int' | 'char') VAR (',' VAR)* ('=' expr)? ';'	# simpledeclaration
	| type = ('int' | 'char') VAR '[' expr ']' ';'			# arraydeclaration;

bracesassignment:
	lvalue '=' '{' expr (',' expr)* '}' ';';

assignment: lvalue '=' expr ';';

lvalue:
	VAR # varlvalue
	| VAR '[' expr ']' # arraylvalue;

expr:
	lvalue										# lvalueexpr
	| op = ('-' | '!') expr						# unaryexpr
	| expr op = ('*' | '/') expr				# muldiv
	| expr op = ('+' | '-') expr				# addsub
	| expr op = ('<' | '>' | '==' | '!=') expr	# compexpr
	| expr op = ('&' | '|' | '^') expr			# bitexpr
	| CONST										# constexpr
	| VAR										# varexpr
	| '(' expr ')'								# parexpr;

CONST: [0-9]+;
COMMENT: ('/*' .*? '*/' | '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
