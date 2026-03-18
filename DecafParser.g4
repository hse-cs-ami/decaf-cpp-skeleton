parser grammar DecafParser;

options {
  tokenVocab = DecafScanner;
}

program: TK_LCURLY statement* TK_RCURLY EOF;

statement: IDENTIFIER TK_ASSIGN expr TK_SEMICOL;

// Арифметические выражения с приоритетом операций.
expr      : expr TK_PLUS  term
          | expr TK_MINUS term
          | term
          ;

term      : term TK_MUL atom
          | term TK_DIV atom
          | atom
          ;

atom      : INTLITERAL
          | IDENTIFIER
          | TK_LPAREN expr TK_RPAREN
          ;
