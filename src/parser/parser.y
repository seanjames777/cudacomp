%{
#include <iostream>
#include <string>
#include <ast/ast.h>
#include <parser/parse.h>

#define YYERROR_VERBOSE
#define YYMAXDEPTH 100000

int line_num = 1;
std::unordered_map<std::string, ASTTypeNode *> typedefs;

int yylex(void);

int yywrap() {
    return 1;
}

void yyerror(Parser::ParserArgs *args, const char *str) {
    std::stringstream ss;
    ss << "Line " << line_num << ": " << str;
    throw Parser::ParseException(ss.str());
}

%}

// Note: We can't return anything, so we need to use a pointer to a shared
// pointer. TODO: Could use a reference, but it's not much of an improvement.

%parse-param {Parser::ParserArgs *args}

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
    int int32;
    float float32;
    char *string;
    bool boolean;
}

%token <int32> INT32
%token <float32> FLOAT32
%token <string> IDENT IDTYPE
%token <boolean> TRUE FALSE
%token PLUS MINUS DIV TIMES MOD SHL SHR AND OR BAND BOR BXOR NOT BNOT UNARY
%token ASSIGN SEMI COMMA LBRACKET RBRACKET INCR DECR DOT ARROW
%token INT BOOL VOID FLOAT
%token RETURN IF ELSE TYPEDEF WHILE EXTERN ALLOC_ARRAY COLON FOR ALLOC QUESTION
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LEQ GEQ LT GT
%token PLUSEQ MINUSEQ TIMESEQ DIVEQ MODEQ SALEQ SAREQ OREQ ANDEQ XOREQ
%token STRUCT BREAK CONTINUE ASSERT KWNULL CHAR STRING DIML DIMR

%type <exp> exp
%type <type> type
%type <stmt> stmt simp simpopt
%type <stmt_seq> stmt_list elseopt
%type <arg> param field
%type <arg_seq> param_list param_list_follow dim_param_list_opt field_list field_list_follow
%type <top> top
%type <top_seq> top_list
%type <top> fundecl typedecl structdecl
%type <exp_seq> arg_list arg_list_follow dim_arg_list_opt
%type <linkage> linkage

%right ASSIGN PLUSEQ MINUSEQ TIMESEQ DIVEQ MODEQ SALEQ SAREQ OREQ ANDEQ XOREQ
%left TO
%right QUESTION COLON
%left OR
%left AND
%left BOR
%left BXOR
%left BAND
%left EQ NEQ
%left LEQ GEQ LT GT
%left SHL SHR
%left PLUS MINUS
%left STAR DIV MOD
%right NOT BNOT UNARY
%nonassoc LPAREN RPAREN LBRACKET RBRACKET DOT ARROW

%start program

%%

top:
    fundecl                           { $$ = $1; }
  | typedecl                          { $$ = $1; }
  | structdecl                        { $$ = $1; }
  ;

top_list:
    /* empty */                       { $$ = nullptr; }
  | top top_list                      { $$ = new ASTDeclSeqNode(std::shared_ptr<ASTDeclNode>($1), std::shared_ptr<ASTDeclSeqNode>($2)); }
  ;

program:
    top_list                          { args->root = std::shared_ptr<ASTDeclSeqNode>($1); }
  ;

stmt_list:
    /* empty */                       { $$ = nullptr; }
  | stmt stmt_list                    { $$ = new ASTStmtSeqNode(std::shared_ptr<ASTStmtNode>($1), std::shared_ptr<ASTStmtSeqNode>($2)); }
  ;

exp:
    INT32                             { $$ = new ASTIntegerExp($1); }
  | FLOAT32                           { $$ = new ASTFloatExp($1); }
  | TRUE                              { $$ = new ASTBooleanExp(true); }
  | FALSE                             { $$ = new ASTBooleanExp(false); }
  | exp PLUS exp                      { $$ = new ASTBinopExp(ASTBinopExp::ADD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MINUS exp                     { $$ = new ASTBinopExp(ASTBinopExp::SUB, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp DIV exp                       { $$ = new ASTBinopExp(ASTBinopExp::DIV, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp STAR exp                      { $$ = new ASTBinopExp(ASTBinopExp::MUL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MOD exp                       { $$ = new ASTBinopExp(ASTBinopExp::MOD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SHL exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SHR exp                       { $$ = new ASTBinopExp(ASTBinopExp::SHR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp AND exp                       { $$ = new ASTTernopExp(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3), std::shared_ptr<ASTExpNode>(new ASTBooleanExp(false))); }
  | exp OR exp                        { $$ = new ASTTernopExp(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>(new ASTBooleanExp(true)), std::shared_ptr<ASTExpNode>($3)); }
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
  | STAR exp %prec UNARY              { $$ = new ASTDerefExp(std::shared_ptr<ASTExpNode>($2)); }
  | MINUS exp %prec UNARY             { $$ = new ASTUnopExp(ASTUnopExp::NEG, std::shared_ptr<ASTExpNode>($2)); }
  | IDENT dim_arg_list_opt LPAREN arg_list RPAREN
    { $$ = new ASTCallExp(std::string($1), std::shared_ptr<ASTExpSeqNode>($2), std::shared_ptr<ASTExpSeqNode>($4)); free($1); }
  | IDENT                             { $$ = new ASTIdentifierExp(std::string($1)); free($1); }
  | LPAREN exp RPAREN                 { $$ = $2; }
  | exp LBRACKET exp RBRACKET         { $$ = new ASTIndexExp(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | ALLOC_ARRAY LPAREN type COMMA exp RPAREN
    { $$ = new ASTAllocArrayExp(std::shared_ptr<ASTTypeNode>($3), std::shared_ptr<ASTExpNode>($5)); }
  | ALLOC LPAREN type RPAREN          { $$ = new ASTAllocExp(std::shared_ptr<ASTTypeNode>($3)); }
  | exp DOT IDENT
  { $$ = new ASTRecordAccessExp(std::shared_ptr<ASTExpNode>($1), std::string($3) ); free($3); }
  | exp DOT IDTYPE
  { $$ = new ASTRecordAccessExp(std::shared_ptr<ASTExpNode>($1), std::string($3) ); free($3); }
  | exp ARROW IDENT
  { $$ = new ASTRecordAccessExp(std::shared_ptr<ASTExpNode>( new ASTDerefExp( std::shared_ptr<ASTExpNode>($1))), std::string($3) ); free($3); }
  | exp ARROW IDTYPE
  { $$ = new ASTRecordAccessExp(std::shared_ptr<ASTExpNode>( new ASTDerefExp( std::shared_ptr<ASTExpNode>($1))), std::string($3) ); free($3); }
  | exp QUESTION exp COLON exp        { $$ = new ASTTernopExp(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3), std::shared_ptr<ASTExpNode>($5)); }
  ;

type:
    INT                               { $$ = new ASTIntegerType(); }
  | BOOL                              { $$ = new ASTBooleanType(); }
  | FLOAT                             { $$ = new ASTFloatType(); }
  | VOID                              { $$ = new ASTVoidType(); }
  | IDTYPE                            { $$ = new ASTIdType(std::string($1)); free($1); }
  | STRUCT IDENT                      { $$ = new ASTRecordType(std::string($2), nullptr); free($2); }
  | STRUCT IDTYPE                     { $$ = new ASTRecordType(std::string($2), nullptr); free($2); }
  | type LBRACKET RBRACKET            { $$ = new ASTArrType(std::shared_ptr<ASTTypeNode>($1)); }
  | type STAR                         { $$ = new ASTPtrType(std::shared_ptr<ASTTypeNode>($1)); }
  ;

simpopt:
    /* empty */                       { $$ = nullptr; }
  | simp                              { $$ = $1; }

simp:
    type IDENT                        { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), nullptr); free($2); }
  | type IDENT ASSIGN exp             { $$ = new ASTVarDeclStmt(std::shared_ptr<ASTTypeNode>($1), std::string($2), std::shared_ptr<ASTExpNode>($4)); free($2); }
  | exp ASSIGN exp                    { $$ = new ASTAssignStmt(ASTBinopExp::NONE, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp PLUSEQ exp                    { $$ = new ASTAssignStmt(ASTBinopExp::ADD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MINUSEQ exp                   { $$ = new ASTAssignStmt(ASTBinopExp::SUB, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp TIMESEQ exp                   { $$ = new ASTAssignStmt(ASTBinopExp::MUL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp DIVEQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::DIV, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp MODEQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::MOD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SALEQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::SHL, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp SAREQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::SHR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp ANDEQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::BAND, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp OREQ exp                      { $$ = new ASTAssignStmt(ASTBinopExp::BOR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp XOREQ exp                     { $$ = new ASTAssignStmt(ASTBinopExp::BXOR, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>($3)); }
  | exp INCR                          { $$ = new ASTAssignStmt(ASTBinopExp::ADD, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>(new ASTIntegerExp(1))); }
  | exp DECR                          { $$ = new ASTAssignStmt(ASTBinopExp::SUB, std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpNode>(new ASTIntegerExp(1))); }
  | exp                               { $$ = new ASTExprStmt(std::shared_ptr<ASTExpNode>($1)); }
  ;

stmt:
    simp SEMI                         { $$ = $1; }
  | RETURN exp SEMI                   { $$ = new ASTReturnStmt(std::shared_ptr<ASTExpNode>($2)); }
  | RETURN SEMI                       { $$ = new ASTReturnStmt(nullptr); }
  | LBRACE stmt_list RBRACE           { $$ = new ASTScopeStmt(std::shared_ptr<ASTStmtSeqNode>($2)); }
  | IF LPAREN exp RPAREN stmt elseopt { $$ = new ASTIfStmt(std::shared_ptr<ASTExpNode>($3), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($5), nullptr), std::shared_ptr<ASTStmtSeqNode>($6)); }
  | WHILE LPAREN exp RPAREN stmt      { $$ = new ASTWhileStmt(std::shared_ptr<ASTExpNode>($3), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($5), nullptr)); }
  | FOR LPAREN simpopt SEMI exp SEMI simpopt RPAREN stmt
    { $$ = new ASTForStmt(std::shared_ptr<ASTStmtNode>($3), std::shared_ptr<ASTExpNode>($5), std::shared_ptr<ASTStmtNode>($7), std::make_shared<ASTStmtSeqNode>(std::shared_ptr<ASTStmtNode>($9), nullptr)); }
  | ASSERT LPAREN exp RPAREN SEMI     { $$ = new ASTAssertStmt(std::shared_ptr<ASTExpNode>($3)); }
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
    linkage type IDENT dim_param_list_opt LPAREN param_list RPAREN LBRACE stmt_list RBRACE
    {
        $$ = new ASTFunDecl(std::string($3),
                 std::make_shared<ASTFunType>(std::shared_ptr<ASTTypeNode>($2),
                 std::shared_ptr<ASTArgSeqNode>($4),
                 std::shared_ptr<ASTArgSeqNode>($6)),
                 true,
                 args->header ? ASTDeclNode::External : $1,
                 std::shared_ptr<ASTStmtSeqNode>($9));
        free($3);
    }
  | linkage type IDENT dim_param_list_opt LPAREN param_list RPAREN SEMI
    {
        $$ = new ASTFunDecl(
                 std::string($3),
                 std::make_shared<ASTFunType>(std::shared_ptr<ASTTypeNode>($2),
                 std::shared_ptr<ASTArgSeqNode>($4),
                 std::shared_ptr<ASTArgSeqNode>($6)),
                 false,
                 args->header ? ASTDeclNode::External : $1,
                 nullptr);
                 free($3);
    }
  ;

dim_param_list_opt:
    /* empty */                       { $$ = nullptr; }
  | DIML param_list DIMR              { $$ = $2; }
  ;

dim_arg_list_opt:
    /* empty */                       { $$ = nullptr; }
  | DIML arg_list DIMR                { $$ = $2; }
  ;

arg_list_follow:
    /* empty */                       { $$ = nullptr; }
  | COMMA exp arg_list_follow         { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($2), std::shared_ptr<ASTExpSeqNode>($3)); }
  ;

arg_list:
    /* empty */                       { $$ = nullptr; }
  | exp arg_list_follow               { $$ = new ASTExpSeqNode(std::shared_ptr<ASTExpNode>($1), std::shared_ptr<ASTExpSeqNode>($2)); }
  ;

structdecl:
    STRUCT IDENT field_list SEMI
    { $$ = new ASTRecordDecl(std::string($2), std::make_shared<ASTRecordType>(std::string($2),std::shared_ptr<ASTArgSeqNode>($3)), true); free($2); }
  | STRUCT IDTYPE field_list SEMI
    { $$ = new ASTRecordDecl(std::string($2), std::make_shared<ASTRecordType>(std::string($2),std::shared_ptr<ASTArgSeqNode>($3)), true); free($2); }
  | STRUCT IDENT SEMI                 { $$ = new ASTRecordDecl(std::string($2), nullptr, false); free($2); }
  | STRUCT IDTYPE SEMI                { $$ = new ASTRecordDecl(std::string($2), nullptr, false); free($2); }
  ;

field:
    type IDENT SEMI                   { $$ = new ASTArgNode(std::shared_ptr<ASTTypeNode>($1), std::string($2)); free($2); }
  | type IDTYPE SEMI                  { $$ = new ASTArgNode(std::shared_ptr<ASTTypeNode>($1), std::string($2)); free($2); }
  ;

field_list_follow:
    /* empty */                       { $$ = nullptr; }
  | field field_list_follow           { $$ = new ASTArgSeqNode(std::shared_ptr<ASTArgNode>($1), std::shared_ptr<ASTArgSeqNode>($2)); }
  ;

field_list:
    LBRACE field_list_follow RBRACE   { $$ = $2; }
  ;
