lexer grammar DecafScanner;

// Включите трассировку для отладки:
// $ antlr4 -trace DecafScanner.g4

TK_LCURLY  : '{';
TK_RCURLY  : '}';
TK_LPAREN  : '(';
TK_RPAREN  : ')';
TK_PLUS    : '+';
TK_MINUS   : '-';
TK_MUL     : '*';
TK_DIV     : '/';
TK_ASSIGN  : '=';
TK_SEMICOL : ';';

INTLITERAL : [0-9]+;
IDENTIFIER : [a-zA-Z_] [a-zA-Z0-9_]*;

// Пробельные символы и однострочные комментарии пропускаются.
WS         : [ \t\r\n]+ -> skip;
SL_COMMENT : '//' ~[\r\n]* -> skip;

CHARLITERAL  : '\'' (ESC_SEQ | CHAR_CHAR) '\'';
STRINGLITERAL: '"' (ESC_SEQ | STR_CHAR)* '"';

fragment CHAR_CHAR : ~['\\\n];
fragment STR_CHAR  : ~["\\\n];
fragment ESC_SEQ   : '\\' ['"\\n];
