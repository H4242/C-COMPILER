grammar ifcc;

axiom: function* prog function*;

block:
	'{' (
		assignment
		| declaration
		| callFunction ';'
		| if_stmt
		| while_stmt
		| returnstmt
		| block
		| putchar
	)* returnstmt? '}';

function:
	type VAR '(' declParams? ')' ';'	# functiondecl
	| type VAR '(' defParams? ')' block	# functiondef;

declParams: type VAR (',' type VAR)*;
defParams: type VAR (',' type VAR)*;

callFunction: VAR '(' args? ')';
args: expr (',' expr)*;

putchar: 'putchar' '(' expr ')' ';';

prog:
	'int' 'main' '(' ')' '{' (
		assignment
		| declaration
		| callFunction ';'
		| if_stmt
		| while_stmt
		| returnstmt
		| block
		| putchar
	)* returnstmt '}';

returnstmt: 'return' expr? ';';

declaration: type VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

if_stmt:
	'if' '(' expr ')' block (
		'else' 'if' '(' expr ')' block
	)* ('else' block)?;

while_stmt: 'while' '(' expr ')' block;

expr:
	op = ('-' | '!') expr										# unaryexpr
	| expr op = ('*' | '/' | '%') expr							# muldiv
	| expr op = ('+' | '-') expr								# addsub
	| expr op = ('<' | '>' | '==' | '!=' | '<=' | '>=') expr	# compexpr
	| expr op = ('&' | '|' | '^') expr							# bitexpr
	| CONST														# constexpr
	| VAR														# varexpr
	| CHAR														# charexpr
	| '(' expr ')'												# parexpr
	| callFunction												# callexpr;

type: 'void' # voidtype | 'int' # inttype | 'char' # chartype;

CHAR: '\'' . '\'';
CONST: [0-9]+;
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
COMMENT: ('/*' .*? '*/' | '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
