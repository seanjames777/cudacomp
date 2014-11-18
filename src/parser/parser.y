%{
#include <iostream>
#include <string>
#include <ast/astnode.h>
#include <ast/astinteger.h>
#include <ast/astidentifier.h>
#include <ast/astbinop.h>

int yylex(void);

    int yywrap() {
        return 1;
    }

    void yyerror(ASTNode **root, const char *str) {
        fprintf(stderr, "Error: %s\n", str);
    }

%}

%parse-param { ASTNode **root }

%union {
    ASTNode *node;
    int number;
    char *string;
}

%token <number> NUMBER
%token <string> IDENT
%token PLUS MINUS DIV TIMES

%type <node> top exp

%start top

%%

top:
    exp             { *root = $1; }
  ;

exp:
    NUMBER          { $$ = new ASTInteger($1); }
  | IDENT           { $$ = new ASTIdentifier($1); }
  | exp PLUS exp    { $$ = new ASTBinop(ASTBinop::ADD, $1, $3); }
  | exp MINUS exp   { $$ = new ASTBinop(ASTBinop::SUB, $1, $3); }
  | exp DIV exp     { $$ = new ASTBinop(ASTBinop::DIV, $1, $3); }
  | exp TIMES exp   { $$ = new ASTBinop(ASTBinop::MUL, $1, $3); }
  ;
