grammar ifcc;

axiom: function* prog function*;

block : '{' (assignment | declaration | callFunction)* returnstmt? '}';

function : retType=('int'|'char'|'void') VAR '(' declParams? ')' ';'	#functiondecl
 		| retType=('int'|'char'|'void') VAR '(' defParams? ')' block 	#functiondef
		;

declParams : type VAR (',' type VAR)*;
defParams : type VAR (',' type VAR)*;

callFunction: VAR '(' args? ')';

args: expr (',' expr)*;

prog: 'int' 'main' '(' ')' '{' (assignment | declaration | callFunction ';' | if_stmt | while_stmt | returnstmt)* returnstmt '}';

returnstmt: 'return' expr? ';';

declaration: type VAR (',' VAR)* ('=' expr)? ';';

assignment: VAR '=' expr ';';

if_stmt:
	'if' '(' expr ')' stat_block (
		'else if' '(' expr ')' stat_block
	)* ('else' stat_block)?;

stat_block:
	'{' (
		assignment
		| declaration
		| if_stmt
		| while_stmt
		| returnstmt
	)* '}';

while_stmt: 'while' '(' expr ')' stat_block;

expr:
	op=('-' | '!') expr										# unaryexpr
	|expr op=('*' | '/') expr								# muldiv
	| expr op=('+' | '-') expr								# addsub
	| expr op=('<' | '>' | '==' | '!=' | '<=' | '>=') expr	# compexpr
	| expr op=('&' | '|' | '^') expr						# bitexpr
	| CONST													# constexpr
	| VAR													# varexpr
	| '(' expr ')'											# parexpr
	| callFunction											# callexpr;

type : 'int' # inttype
	| 'char' # chartype
	;

CONST: [0-9]+;
COMMENT: ('/*' .*? '*/' | '//' .*? '\n') -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
