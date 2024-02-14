parser grammar ExprParser;
options { tokenVocab=ExprLexer; }

program
    : SYNC AWAIT ID (',' ID)* EOF
    ;







