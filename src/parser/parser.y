%{
#include <iostream>
#include <string>
#include <ast/expr/astexpnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/expr/astbinopexp.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astintegertype.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/type/astbooleantype.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/expr/astunopexp.h>
#include <ast/stmt/astifstmt.h>
#include <ast/top/asttopnode.h>
#include <ast/top/astfundefntop.h>
#include <ast/type/astfuntype.h>
#include <ast/type/astarg.h>
#include <ast/expr/astcallexp.h>
#include <ast/type/astvoidtype.h>
#include <ast/type/astptrtype.h>
#include <ast/stmt/astexprstmt.h>
#include <ast/stmt/astwhilestmt.h>

#define YYERROR_VERBOSE

int yylex(void);

int yywrap() {
    return 1;
}

void yyerror(ASTTopSeqNode **root, const char *str) {
    fprintf(stderr, "Error: %s\n", str);
}

%}

%parse-param { ASTTopSeqNode **root }

%union {
    ASTTopNode *top;
    ASTTopSeqNode *top_seq;
    ASTStmtNode *stmt;
    ASTStmtSeqNode *stmt_seq;
    ASTExpNode *exp;
    ASTExpSeqNode *exp_seq;
    ASTTypeNode *type;
    ASTArg *arg;
    ASTArgSeqNode *arg_seq;
    int number;
    char *string;
    bool boolean;
}

%token <number> NUMBER
%token <string> IDENT
%token <boolean> TRUE FALSE
%token PLUS MINUS DIV TIMES MOD SHL SHR AND OR BAND BOR BXOR NOT BNOT
%token ASSIGN SEMI COMMA
%token INT BOOL VOID
%token RETURN IF ELSE WHILE
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LEQ GEQ LT GT

%type <exp> exp
%type <type> type
%type <stmt> stmt simp
%type <stmt_seq> stmt_list
%type <stmt_seq> elseopt
%type <arg> param
%type <arg_seq> param_list param_list_follow
%type <top> top
%type <top_seq> top_list
%type <top> fundefn
%type <exp_seq> arg_list arg_list_follow

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

%start program

%%

top:
    fundefn                           { $$ = $1; }
  ;

top_list:
    /* empty */                       { $$ = NULL; }
  | top top_list                      { $$ = new ASTTopSeqNode($1, $2); }
  ;

program:
    top_list                          { *root = $1; }
  ;

stmt_list:
    /* empty */                       { $$ = NULL; }
  | stmt stmt_list                    { $$ = new ASTStmtSeqNode($1, $2); }
  ;

exp:
    NUMBER                            { $$ = new ASTIntegerExp($1); }
  | TRUE                              { $$ = new ASTBooleanExp(true); }
  | FALSE                             { $$ = new ASTBooleanExp(false); }
  | IDENT                             { $$ = new ASTIdentifierExp(std::string($1)); free($1); }
  | exp PLUS exp                      { $$ = new ASTBinopExp(ASTBinopExp::ADD, $1, $3); }
  | exp MINUS exp                     { $$ = new ASTBinopExp(ASTBinopExp::SUB, $1, $3); }
  | exp DIV exp                       { $$ = new ASTBinopExp(ASTBinopExp::DIV, $1, $3); }
  | exp TIMES exp                     { $$ = new ASTBinopExp(ASTBinopExp::MUL, $1, $3); }
  | exp MOD exp                       { $$ = new ASTBinopExp(ASTBinopExp::MOD, $1, $3); }
  | exp SHL exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHL, $1, $3); }
  | exp SHR exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHR, $1, $3); }
  | exp AND exp                       { $$ = new ASTBinopExp(ASTBinopExp::AND, $1, $3); }
  | exp OR exp                        { $$ = new ASTBinopExp(ASTBinopExp::OR, $1, $3); }
  | exp BAND exp                      { $$ = new ASTBinopExp(ASTBinopExp::BAND, $1, $3); }
  | exp BOR exp                       { $$ = new ASTBinopExp(ASTBinopExp::BOR, $1, $3); }
  | exp BXOR exp                      { $$ = new ASTBinopExp(ASTBinopExp::BXOR, $1, $3); }
  | exp EQ exp                        { $$ = new ASTBinopExp(ASTBinopExp::EQ, $1, $3); }
  | exp NEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::NEQ, $1, $3); }
  | exp GT exp                        { $$ = new ASTBinopExp(ASTBinopExp::GT, $1, $3); }
  | exp LT exp                        { $$ = new ASTBinopExp(ASTBinopExp::LT, $1, $3); }
  | exp GEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::GEQ, $1, $3); }
  | exp LEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::LEQ, $1, $3); }
  | NOT exp                           { $$ = new ASTUnopExp(ASTUnopExp::NOT, $2); }
  | BNOT exp                          { $$ = new ASTUnopExp(ASTUnopExp::BNOT, $2); }
  | MINUS exp %prec UMINUS            { $$ = new ASTUnopExp(ASTUnopExp::NEG, $2); }
  | LPAREN exp RPAREN                 { $$ = $2; }
  | IDENT LPAREN arg_list RPAREN      { $$ = new ASTCallExp($1, $3); }
  ;

type:
    INT                               { $$ = new ASTIntegerType(); }
  | BOOL                              { $$ = new ASTBooleanType(); }
  | VOID                              { $$ = new ASTVoidType(); }
  ;

simp:
    type IDENT                        { $$ = new ASTVarDeclStmt($1, std::string($2), NULL); free($2); }
  | type IDENT ASSIGN exp             { $$ = new ASTVarDeclStmt($1, std::string($2), $4); free($2); }
  | IDENT ASSIGN exp                  { $$ = new ASTVarDefnStmt(std::string($1), $3); free($1); } // TODO free
  | exp                               { $$ = new ASTExprStmt($1); }
  ;

stmt:
    simp SEMI                         { $$ = $1; }
  | RETURN exp SEMI                   { $$ = new ASTReturnStmt($2); }
  | RETURN SEMI                       { $$ = new ASTReturnStmt(NULL); }
  | LBRACE stmt_list RBRACE           { $$ = new ASTScopeStmt($2); }
  | IF LPAREN exp RPAREN stmt elseopt { $$ = new ASTIfStmt($3, new ASTStmtSeqNode($5, NULL), $6); }
  | WHILE LPAREN exp RPAREN stmt      { $$ = new ASTWhileStmt($3, new ASTStmtSeqNode($5, NULL))}
  ;

elseopt:
    /* empty */                       { $$ = NULL; }
  | ELSE stmt                         { $$ = new ASTStmtSeqNode($2, NULL); }
  ;

param:
    type IDENT                        { $$ = new ASTArg($1, std::string($2)); free($2); }
  ;

param_list_follow:
    /* empty */                       { $$ = NULL; }
  | COMMA param param_list_follow     { $$ = new ASTArgSeqNode($2, $3); }
  ;

param_list:
    /* empty */                       { $$ = NULL; }
  | param param_list_follow           { $$ = new ASTArgSeqNode($1, $2); }
  ;

fundefn:
    type IDENT LPAREN param_list RPAREN LBRACE stmt_list RBRACE
    { $$ = new ASTFunDefnTop($2, new ASTFunType($1, $4), $7); }
  ;

arg_list_follow:
    /* empty */                       { $$ = NULL; }
  | COMMA exp arg_list_follow         { $$ = new ASTExpSeqNode($2, $3); }
  ;

arg_list:
    /* empty */                       { $$ = NULL; }
  | exp arg_list_follow               { $$ = new ASTExpSeqNode($1, $2); }
  ;
