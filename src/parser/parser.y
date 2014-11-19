%{
#include <iostream>
#include <string>
#include <ast/expr/astexpnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astinteger.h>
#include <ast/expr/astidentifier.h>
#include <ast/expr/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/type/asttype.h>
#include <ast/type/astintegertype.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/type/astbooleantype.h>
#include <ast/expr/astboolean.h>
#include <ast/stmt/astscope.h>
#include <ast/expr/astunop.h>
#include <ast/stmt/astifstmt.h>

#define YYERROR_VERBOSE

int yylex(void);

    int yywrap() {
        return 1;
    }

    void yyerror(ASTStmtSeqNode **root, const char *str) {
        fprintf(stderr, "Error: %s\n", str);
    }

%}

%parse-param { ASTStmtSeqNode **root }

%union {
    ASTStmtNode *stmt;
    ASTStmtSeqNode *seq;
    ASTExpNode *exp;
    ASTType *type;
    int number;
    char *string;
    bool boolean;
}

%token <number> NUMBER
%token <string> IDENT
%token <boolean> TRUE FALSE
%token PLUS MINUS DIV TIMES MOD SHL SHR AND OR BAND BOR BXOR NOT BNOT
%token ASSIGN SEMI
%token INT BOOL
%token RETURN IF ELSE
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LEQ GEQ LT GT

%type <exp> exp
%type <stmt> stmt
%type <seq> stmt_list
%type <type> type
%type <seq> elseopt

%right ASSIGN
%left OR
%left AND
%left BOR
%left BXOR
%left BAND
%left EQ NEQ
%left LEQ GEQ LT GT
%left SHL SHR
%left PLUS MINUS
%left TIMES DIV MOD
%right NOT BNOT UMINUS
%nonassoc LPAREN RPAREN

%start top

%%

top:
    stmt_list                         { *root = $1; }
  ;

stmt_list:
    /* empty */                       { $$ = NULL; }
  | stmt stmt_list                    { $$ = new ASTStmtSeqNode($1, $2); }
  ;

exp:
    NUMBER                            { $$ = new ASTInteger($1); }
  | TRUE                              { $$ = new ASTBoolean(true); }
  | FALSE                             { $$ = new ASTBoolean(false); }
  | IDENT                             { $$ = new ASTIdentifier(std::string($1)); free($1); }
  | exp PLUS exp                      { $$ = new ASTBinop(ASTBinop::ADD, $1, $3); }
  | exp MINUS exp                     { $$ = new ASTBinop(ASTBinop::SUB, $1, $3); }
  | exp DIV exp                       { $$ = new ASTBinop(ASTBinop::DIV, $1, $3); }
  | exp TIMES exp                     { $$ = new ASTBinop(ASTBinop::MUL, $1, $3); }
  | exp MOD exp                       { $$ = new ASTBinop(ASTBinop::MOD, $1, $3); }
  | exp SHL exp                       { $$ = new ASTBinop(ASTBinop::SHL, $1, $3); }
  | exp SHR exp                       { $$ = new ASTBinop(ASTBinop::SHR, $1, $3); }
  | exp AND exp                       { $$ = new ASTBinop(ASTBinop::AND, $1, $3); }
  | exp OR exp                        { $$ = new ASTBinop(ASTBinop::OR, $1, $3); }
  | exp BAND exp                      { $$ = new ASTBinop(ASTBinop::BAND, $1, $3); }
  | exp BOR exp                       { $$ = new ASTBinop(ASTBinop::BOR, $1, $3); }
  | exp BXOR exp                      { $$ = new ASTBinop(ASTBinop::BXOR, $1, $3); }
  | exp EQ exp                        { $$ = new ASTBinop(ASTBinop::EQ, $1, $3); }
  | exp NEQ exp                       { $$ = new ASTBinop(ASTBinop::NEQ, $1, $3); }
  | exp GT exp                        { $$ = new ASTBinop(ASTBinop::GT, $1, $3); }
  | exp LT exp                        { $$ = new ASTBinop(ASTBinop::LT, $1, $3); }
  | exp GEQ exp                       { $$ = new ASTBinop(ASTBinop::GEQ, $1, $3); }
  | exp LEQ exp                       { $$ = new ASTBinop(ASTBinop::LEQ, $1, $3); }
  | NOT exp                           { $$ = new ASTUnop(ASTUnop::NOT, $2); }
  | BNOT exp                          { $$ = new ASTUnop(ASTUnop::BNOT, $2); }
  | MINUS exp %prec UMINUS            { $$ = new ASTUnop(ASTUnop::NEG, $2); }
  | LPAREN exp RPAREN                 { $$ = $2; }
  ;

type:
    INT                               { $$ = new ASTIntegerType(); }
  | BOOL                              { $$ = new ASTBooleanType(); }
  ;

stmt:
    RETURN exp SEMI                   { $$ = new ASTReturnStmt($2); }
  | type IDENT SEMI                   { $$ = new ASTVarDeclStmt($1, std::string($2), NULL); free($2); }
  | type IDENT ASSIGN exp SEMI        { $$ = new ASTVarDeclStmt($1, std::string($2), $4); free($2); }
  | IDENT ASSIGN exp SEMI             { $$ = new ASTVarDefnStmt(std::string($1), $3); free($1); } // TODO free
  | LBRACE stmt_list RBRACE           { $$ = new ASTScope($2); }
  | IF LPAREN exp RPAREN stmt elseopt { $$ = new ASTIfStmt($3, new ASTStmtSeqNode($5, NULL), $6); }
  ;

elseopt:
    /* empty */                       { $$ = NULL; }
  | ELSE stmt                         { $$ = new ASTStmtSeqNode($2, NULL); }
  ;
