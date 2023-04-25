grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration:
	//type = ('int' | 'char') VAR '*' ';'							# pointerdeclaration
	type = ('int' | 'char') VAR '[' expr ']' ';'				# arraydeclaration
	| type = ('int' | 'char') VAR (',' VAR)* ('=' expr)? ';'	# simpledeclaration;

assignment: lvalue '=' expr ';';

lvalue:
	VAR '[' expr ']' # arraylvalue
	//| '*' VAR			# ptrlvalue
	| VAR # varlvalue;

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
