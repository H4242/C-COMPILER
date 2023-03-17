grammar ifcc;

axiom : prog ;

prog : 'int' 'main' '(' ')' '{' affect return_stmt ';' '}' ;

return_stmt 
    : RETURN CONST #returnconst
    | RETURN VAR #returnvar
    ;

declaration : 'int' VAR;

affect 
    : declaration '=' CONST ';'
    | declaration '=' VAR ';'
    | VAR '=' VAR ';'
    ;

RETURN : 'return' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
VAR : ([a-zA-Z_][a-zA-Z0-9_]*);
