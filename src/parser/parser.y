%{
#include <iostream>
#include <string>
#include <ast/astexpnode.h>
#include <ast/aststmtnode.h>
#include <ast/astinteger.h>
#include <ast/astidentifier.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>

#define YYERROR_VERBOSE

int yylex(void);

    int yywrap() {
        return 1;
    }

    void yyerror(ASTStmtNode **root, const char *str) {
        fprintf(stderr, "Error: %s\n", str);
    }

%}

%parse-param { ASTStmtNode **root }

%union {
    ASTStmtNode *stmt;
    ASTSeqNode *seq;
    ASTExpNode *exp;
    int number;
    char *string;
}

%token <number> NUMBER
%token <string> IDENT
%token PLUS MINUS DIV TIMES
%token ASSIGN SEMI
%token INT
%token RETURN

%type <exp> exp
%type <stmt> stmt
%type <seq> stmt_list

%start top

%%

top:
    stmt_list                 { *root = $1; }
  ;

stmt_list:
    /* emp */                 { $$ = NULL; }
  | stmt stmt_list            { $$ = new ASTSeqNode($1, $2); }
  ;

exp:
    NUMBER                    { $$ = new ASTInteger($1); }
  | IDENT                     { $$ = new ASTIdentifier($1); }
  | exp PLUS exp              { $$ = new ASTBinop(ASTBinop::ADD, $1, $3); }
  | exp MINUS exp             { $$ = new ASTBinop(ASTBinop::SUB, $1, $3); }
  | exp DIV exp               { $$ = new ASTBinop(ASTBinop::DIV, $1, $3); }
  | exp TIMES exp             { $$ = new ASTBinop(ASTBinop::MUL, $1, $3); }
  ;

stmt:
    RETURN exp SEMI           { $$ = new ASTReturnStmt($2); }
  ;
