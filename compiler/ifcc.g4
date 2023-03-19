grammar ifcc;

axiom: prog;

prog: 'int' 'main' '(' ')' '{' (assignment ';'|declaration ';')*  return_stmt ';' '}';

return_stmt:
	RETURN CONST	# returnconst
	| RETURN VAR	# returnvar;

declaration: 'int' VAR;

assignment:
	declaration '=' CONST # assignconst
	| VAR '=' CONST		  # assignconst
	| declaration '=' VAR # assignvar
	| VAR '=' VAR		  # assignvar
	;

RETURN: 'return';
CONST: [0-9]+;
COMMENT: '/*' .*? '*/' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
VAR: ([a-zA-Z_][a-zA-Z0-9_]*);
