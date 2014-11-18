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
#include <ast/asttype.h>
#include <ast/astintegertype.h>
#include <ast/astvardeclstmt.h>
#include <ast/astvardefnstmt.h>

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
    ASTType *type;
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
%type <type> type

%start top

%%

top:
    stmt_list                  { *root = $1; }
  ;

stmt_list:
    /* emp */                  { $$ = NULL; }
  | stmt stmt_list             { $$ = new ASTSeqNode($1, $2); }
  ;

exp:
    NUMBER                     { $$ = new ASTInteger($1); }
  | IDENT                      { $$ = new ASTIdentifier(std::string($1)); free($1); }
  | exp PLUS exp               { $$ = new ASTBinop(ASTBinop::ADD, $1, $3); }
  | exp MINUS exp              { $$ = new ASTBinop(ASTBinop::SUB, $1, $3); }
  | exp DIV exp                { $$ = new ASTBinop(ASTBinop::DIV, $1, $3); }
  | exp TIMES exp              { $$ = new ASTBinop(ASTBinop::MUL, $1, $3); }
  ;

type:
    INT                        { $$ = new ASTIntegerType(); }
  ;

stmt:
    RETURN exp SEMI            { $$ = new ASTReturnStmt($2); }
  | type IDENT SEMI            { $$ = new ASTVarDeclStmt($1, std::string($2), NULL); free($2); }
  | type IDENT ASSIGN exp SEMI { $$ = new ASTVarDeclStmt($1, std::string($2), $4); free($2); }
  | IDENT ASSIGN exp SEMI      { $$ = new ASTVarDefnStmt(std::string($1), $3); free($1); } // TODO free
  ;
