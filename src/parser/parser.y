%{
#include <iostream>
#include <string>
#include <ast/astseqnode.h>
#include <ast/expr/astbinopexp.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/expr/astcallexp.h>
#include <ast/expr/astexpnode.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astunopexp.h>
#include <ast/stmt/astexprstmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/top/astfundefntop.h>
#include <ast/top/asttopnode.h>
#include <ast/top/asttypedefntop.h>
#include <ast/type/astargnode.h>
#include <ast/type/astbooleantype.h>
#include <ast/type/astfuntype.h>
#include <ast/type/astidtype.h>
#include <ast/type/astintegertype.h>
#include <ast/type/astptrtype.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astvoidtype.h>
#include <parser/parse.h>

#define YYERROR_VERBOSE

int yylex(void);

int yywrap() {
    return 1;
}

void yyerror(std::shared_ptr<ASTTopSeqNode> *root, const char *str) {
    throw new Parser::ParseException(std::string(str));
}

std::unordered_map<std::string, ASTTypeNode *> typedefs;

%}

// Note: We can't return anything, so we need to use a pointer to a shared
// pointer. TODO: Could use a reference, but it's not much of an improvement.
%parse-param { std::shared_ptr<ASTTopSeqNode> *root }

// Note: We need to use regular pointers here, because otherwise we can't easily
// put them in a union. This makes the actual parser code a bit dense/ugly, but
// the alternative is to not use shared pointers at all.
%union {
    ASTTopNode *top;
    ASTTopSeqNode *top_seq;
    ASTStmtNode *stmt;
    ASTStmtSeqNode *stmt_seq;
    ASTExpNode *exp;
    ASTExpSeqNode *exp_seq;
    ASTTypeNode *type;
    ASTArgNode *arg;
    ASTArgSeqNode *arg_seq;
    int number;
    char *string;
    bool boolean;
}

%token <number> NUMBER
%token <string> IDENT IDTYPE
%token <boolean> TRUE FALSE
%token PLUS MINUS DIV TIMES MOD SHL SHR AND OR BAND BOR BXOR NOT BNOT
%token ASSIGN SEMI COMMA
%token INT BOOL VOID
%token RETURN IF ELSE TYPEDEF
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
%type <top> fundefn typedefn
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
  | typedefn                          { $$ = $1; }
  ;

top_list:
    /* empty */                       { $$ = nullptr; }
  | top top_list                      { $$ = new ASTTopSeqNode(std::shared_ptr<ASTTopNode>($1), std::shared_ptr<ASTTopSeqNode>($2)); }
  ;

program:
    top_list                          { *root = std::shared_ptr<ASTTopSeqNode>($1); }
  ;

stmt_list:
    /* empty */                       { $$ = nullptr; }
  | stmt stmt_list                    { $$ = new ASTStmtSeqNode(std::shared_ptr<ASTStmtNode>($1), std::shared_ptr<ASTStmtSeqNode>($2)); }
  ;

exp:
    NUMBER                            { $$ = new ASTIntegerExp($1); }
  | TRUE                              { $$ = new ASTBooleanExp(true); }
  | FALSE                             { $$ = new ASTBooleanExp(false); }
  | IDENT                             { $$ = new ASTIdentifierExp(std::string($1)); free($1); }
  | exp PLUS exp                      { $$ = new ASTBinopExp(ASTBinopExp::ADD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MINUS exp                     { $$ = new ASTBinopExp(ASTBinopExp::SUB, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp DIV exp                       { $$ = new ASTBinopExp(ASTBinopExp::DIV, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp TIMES exp                     { $$ = new ASTBinopExp(ASTBinopExp::MUL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MOD exp                       { $$ = new ASTBinopExp(ASTBinopExp::MOD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SHL exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SHR exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp AND exp                       { $$ = new ASTBinopExp(ASTBinopExp::AND, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp OR exp                        { $$ = new ASTBinopExp(ASTBinopExp::OR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp BAND exp                      { $$ = new ASTBinopExp(ASTBinopExp::BAND, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp BOR exp                       { $$ = new ASTBinopExp(ASTBinopExp::BOR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp BXOR exp                      { $$ = new ASTBinopExp(ASTBinopExp::BXOR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp EQ exp                        { $$ = new ASTBinopExp(ASTBinopExp::EQ, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp NEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::NEQ, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp GT exp                        { $$ = new ASTBinopExp(ASTBinopExp::GT, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp LT exp                        { $$ = new ASTBinopExp(ASTBinopExp::LT, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp GEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::GEQ, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp LEQ exp                       { $$ = new ASTBinopExp(ASTBinopExp::LEQ, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | NOT exp                           { $$ = new ASTUnopExp(ASTUnopExp::NOT, std::shared_ptr<ASTExpNode>($2)); }
  | BNOT exp                          { $$ = new ASTUnopExp(ASTUnopExp::BNOT, std::shared_ptr<ASTExpNode>($2)); }
  | MINUS exp %prec UMINUS            { $$ = new ASTUnopExp(ASTUnopExp::NEG, std::shared_ptr<ASTExpNode>($2)); }
  | LPAREN exp RPAREN                 { $$ = $2; }
  | IDENT LPAREN arg_list RPAREN      { $$ = new ASTCallExp($1, std::shared_ptr<ASTExpSeqNode>($3)); }
  ;

type:
    INT                               { $$ = new ASTIntegerType(); }
  | BOOL                              { $$ = new ASTBooleanType(); }
  | VOID                              { $$ = new ASTVoidType(); }
  | IDTYPE                            { $$ = new ASTIdType(std::string($1)); free($1); }
  ;

simp:
    type IDENT                        { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), nullptr); free($2); }
  | type IDENT ASSIGN exp             { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), std::shared_ptr<ASTExpNode>($4)); free($2); }
  | IDENT ASSIGN exp                  { $$ = new ASTVarDefnStmt(std::string($1), std::shared_ptr<ASTExpNode>($3)); free($1); } // TODO free
  | exp                               { $$ = new ASTExprStmt(std::shared_ptr<ASTExpNode>($1)); }
  ;

stmt:
    simp SEMI                         { $$ = $1; }
  | RETURN exp SEMI                   { $$ = new ASTReturnStmt(std::shared_ptr<ASTExpNode>($2)); }
  | RETURN SEMI                       { $$ = new ASTReturnStmt(nullptr); }
  | LBRACE stmt_list RBRACE           { $$ = new ASTScopeStmt(std::shared_ptr<ASTStmtSeqNode>($2)); }
  | IF LPAREN exp RPAREN stmt elseopt { $$ = new ASTIfStmt(std::shared_ptr<ASTExpNode>($3), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($5), nullptr), std::shared_ptr<ASTStmtSeqNode>($6)); }
  ;

elseopt:
    /* empty */                       { $$ = nullptr; }
  | ELSE stmt                         { $$ = new ASTStmtSeqNode(std::shared_ptr<ASTStmtNode>($2), nullptr); }
  ;

param:
    type IDENT                        { $$ = new ASTArgNode(std::shared_ptr<ASTTypeNode>($1), std::string($2)); free($2); }
  ;

param_list_follow:
    /* empty */                       { $$ = nullptr; }
  | COMMA param param_list_follow     { $$ = new ASTArgSeqNode(std::shared_ptr<ASTArgNode>($2), std::shared_ptr<ASTArgSeqNode>($3)); }
  ;

param_list:
    /* empty */                       { $$ = nullptr; }
  | param param_list_follow           { $$ = new ASTArgSeqNode(std::shared_ptr<ASTArgNode>($1), std::shared_ptr<ASTArgSeqNode>($2)); }
  ;

typedefn:
    TYPEDEF type IDENT SEMI
    {
        std::string id = std::string($3);
        $$ = new ASTTypeDefnTop(id, std::shared_ptr<ASTTypeNode>($2));
        typedefs[id] = $2;
        free($3);
    }
  ;

fundefn:
    type IDENT LPAREN param_list RPAREN LBRACE stmt_list RBRACE
    { $$ = new ASTFunDefnTop($2, std::make_shared<ASTFunType>(std::shared_ptr<ASTTypeNode>($1), std::shared_ptr<ASTArgSeqNode>($4)), std::shared_ptr<ASTStmtSeqNode>($7)); }
  ;

arg_list_follow:
    /* empty */                       { $$ = nullptr; }
  | COMMA exp arg_list_follow         { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($2), std::shared_ptr<ASTExpSeqNode>($3)); }
  ;

arg_list:
    /* empty */                       { $$ = nullptr; }
  | exp arg_list_follow               { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpSeqNode>($2)); }
  ;
