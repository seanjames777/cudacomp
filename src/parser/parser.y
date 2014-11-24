%{
#include <iostream>
#include <string>
#include <ast/ast.h>
#include <parser/parse.h>

#define YYERROR_VERBOSE

int yylex(void);

int yywrap() {
    return 1;
}

void yyerror(std::shared_ptr<ASTDeclSeqNode> *root, const char *str) {
    throw Parser::ParseException(std::string(str));
}

std::unordered_map<std::string, ASTTypeNode *> typedefs;

%}

// Note: We can't return anything, so we need to use a pointer to a shared
// pointer. TODO: Could use a reference, but it's not much of an improvement.
%parse-param { std::shared_ptr<ASTDeclSeqNode> *root }

// Note: We need to use regular pointers here, because otherwise we can't easily
// put them in a union. This makes the actual parser code a bit dense/ugly, but
// the alternative is to not use shared pointers at all.
%union {
    ASTDeclNode *top;
    ASTDeclSeqNode *top_seq;
    ASTStmtNode *stmt;
    ASTStmtSeqNode *stmt_seq;
    ASTExpNode *exp;
    ASTExpSeqNode *exp_seq;
    ASTTypeNode *type;
    ASTArgNode *arg;
    ASTArgSeqNode *arg_seq;
    enum ASTDeclNode::Linkage linkage;
    int number;
    char *string;
    bool boolean;
}

%token <number> NUMBER
%token <string> IDENT IDTYPE
%token <boolean> TRUE FALSE
%token PLUS MINUS DIV TIMES MOD SHL SHR AND OR BAND BOR BXOR NOT BNOT
%token ASSIGN SEMI COMMA LBRACKET RBRACKET
%token INT BOOL VOID
%token RETURN IF ELSE TYPEDEF WHILE EXTERN
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
%type <top> fundecl typedecl
%type <exp_seq> arg_list arg_list_follow
%type <linkage> linkage

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
    fundecl                           { $$ = $1; }
  | typedecl                          { $$ = $1; }
  ;

top_list:
    /* empty */                       { $$ = nullptr; }
  | top top_list                      { $$ = new ASTDeclSeqNode(std::shared_ptr<ASTDeclNode>($1), std::shared_ptr<ASTDeclSeqNode>($2)); }
  ;

program:
    top_list                          { *root = std::shared_ptr<ASTDeclSeqNode>($1); }
  ;

stmt_list:
    /* empty */                       { $$ = nullptr; }
  | stmt stmt_list                    { $$ = new ASTStmtSeqNode(std::shared_ptr<ASTStmtNode>($1), std::shared_ptr<ASTStmtSeqNode>($2)); }
  ;

exp:
    NUMBER                            { $$ = new ASTIntegerExp($1); }
  | TRUE                              { $$ = new ASTBooleanExp(true); }
  | FALSE                             { $$ = new ASTBooleanExp(false); }
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
  | IDENT LPAREN arg_list RPAREN      { $$ = new ASTCallExp($1, std::shared_ptr<ASTExpSeqNode>($3)); }
  | IDENT                             { $$ = new ASTIdentifierExp(std::string($1)); free($1); }
  | LPAREN exp RPAREN                 { $$ = $2; }
  | exp LBRACKET exp RBRACKET         { $$ = new ASTIndexExp(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  ;

type:
    INT                               { $$ = new ASTIntegerType(); }
  | BOOL                              { $$ = new ASTBooleanType(); }
  | VOID                              { $$ = new ASTVoidType(); }
  | IDTYPE                            { $$ = new ASTIdType(std::string($1)); free($1); }
  | type LBRACKET RBRACKET            { $$ = new ASTArrType(std::shared_ptr<ASTTypeNode>($1)); }
  ;

simp:
    type IDENT                        { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), nullptr); free($2); }
  | type IDENT ASSIGN exp             { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), std::shared_ptr<ASTExpNode>($4)); free($2); }
  | exp ASSIGN exp                    { $$ = new ASTAssignStmt(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp                               { $$ = new ASTExprStmt(std::shared_ptr<ASTExpNode>($1)); }
  ;

stmt:
    simp SEMI                         { $$ = $1; }
  | RETURN exp SEMI                   { $$ = new ASTReturnStmt(std::shared_ptr<ASTExpNode>($2)); }
  | RETURN SEMI                       { $$ = new ASTReturnStmt(nullptr); }
  | LBRACE stmt_list RBRACE           { $$ = new ASTScopeStmt(std::shared_ptr<ASTStmtSeqNode>($2)); }
  | IF LPAREN exp RPAREN stmt elseopt { $$ = new ASTIfStmt(std::shared_ptr<ASTExpNode>($3), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($5), nullptr), std::shared_ptr<ASTStmtSeqNode>($6)); }
  | WHILE LPAREN exp RPAREN stmt      { $$ = new ASTWhileStmt(std::shared_ptr<ASTExpNode>($3), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($5), nullptr)); }
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

typedecl:
    TYPEDEF type IDENT SEMI
    {
        std::string id = std::string($3);
        $$ = new ASTTypeDecl(id, std::shared_ptr<ASTTypeNode>($2));
        typedefs[id] = $2;
        free($3);
    }
  ;

linkage:
    /* empty */                       { $$ = ASTDeclNode::Internal; }
  | EXTERN                            { $$ = ASTDeclNode::External; }
  ;

fundecl:
    linkage type IDENT LPAREN param_list RPAREN LBRACE stmt_list RBRACE
    { $$ = new ASTFunDecl($3, std::make_shared<ASTFunType>(std::shared_ptr<ASTTypeNode>($2), std::shared_ptr<ASTArgSeqNode>($5)), true, $1, std::shared_ptr<ASTStmtSeqNode>($8)); }
  | linkage type IDENT LPAREN param_list RPAREN SEMI
    { $$ = new ASTFunDecl($3, std::make_shared<ASTFunType>(std::shared_ptr<ASTTypeNode>($2), std::shared_ptr<ASTArgSeqNode>($5)), false, $1, nullptr); }
  ;

arg_list_follow:
    /* empty */                       { $$ = nullptr; }
  | COMMA exp arg_list_follow         { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($2), std::shared_ptr<ASTExpSeqNode>($3)); }
  ;

arg_list:
    /* empty */                       { $$ = nullptr; }
  | exp arg_list_follow               { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpSeqNode>($2)); }
  ;
