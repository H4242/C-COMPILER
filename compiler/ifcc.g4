grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration | if_stmt)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration:
	type = ('int' | 'char') VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

if_stmt: 'if' condition_block 'else' stat_block;

condition_block: '(' expr ')' stat_block;

stat_block: '{' (assignment | declaration | if_stmt)* '}';

expr:
	op = ('-' | '!') expr						# unaryexpr
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
