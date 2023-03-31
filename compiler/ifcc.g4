grammar ifcc;

axiom: prog;

prog:
	'int' 'main' '(' ')' '{' (assignment | declaration)* returnstmt '}';

returnstmt: 'return' expr ';';

declaration: 'int' VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

expr:
	expr OPM expr	# muldiv
	| expr OPA expr	# addsub
	| expr OPB expr	# bitexpr
	| expr OPC expr	# compexpr
	| OPU expr		# unaryexpr
	| CONST			# constexpr
	| VAR			# varexpr
	| '(' expr ')'	# parexpr;

CONST: '-'? [0-9]+;
COMMENT: '/*' .*? '*/' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
OPM: ('*' | '/');
OPA: ('+' | '-');
OPB: ('&' | '|' | '^');
OPC: ('<' | '>' | '==' | '!=');
OPU: ('-' | '!');
