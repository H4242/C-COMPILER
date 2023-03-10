grammar ifcc;

axiom: prog;

prog: 'int' 'main' '(' ')' '{' return_stmt ';' '}';

return_stmt: RETURN CONST;

RETURN: 'return';
CONST: [0-9]+;
COMMENT: '/*' .*? '*/' -> skip;
EOLCOMMENT: '//' .*? '\n' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
