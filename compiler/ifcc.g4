grammar ifcc;

axiom: function* prog function*;

block : '{' (assignment | declaration | callFunction)* returnstmt? '}';

function : type VAR '(' params? ')' ';'	 		#functiondecl
 		| type VAR '(' params? ')' block 		#functiondef
		;

params : type VAR (',' type VAR)*;

callFunction: VAR '(' args? ')' ';';

args: expr (',' expr)*;

prog:
	'int' 'main' '(' ')' block;

returnstmt: 'return' expr ';';

declaration: type VAR (',' VAR)* ('=' expr)? ';';

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

type : 'int' # inttype
	| 'char' # chartype
	;

CONST: [0-9]+;
COMMENT: ('/*' .*? '*/'| '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
