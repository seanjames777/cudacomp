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
#include <ast/top/asttopnode.h>
#include <ast/top/astfundefn.h>
#include <ast/type/astfuntype.h>
#include <ast/type/astarg.h>

#include "parser.hpp"
%}

%%

"return"                { return RETURN; }
"int"                   { return INT; }
"bool"                  { return BOOL; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"true"                  { yylval.boolean = true; return TRUE; } // TODO: don't need the boolean
"false"                 { yylval.boolean = false; return FALSE; }

[0-9]+                  { yylval.number = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.string = strdup(yytext); return IDENT; }
","                     { return COMMA; }
"+"                     { return PLUS; }
"*"                     { return TIMES; }
"-"                     { return MINUS; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"<<"                    { return SHL; }
">>"                    { return SHR; }
"&&"                    { return AND; }
"||"                    { return OR; }
"&"                     { return BAND; }
"|"                     { return BOR; }
"^"                     { return BXOR; }
"~"                     { return BNOT; }
"!"                     { return NOT; }
"<="                    { return LEQ; }
"<"                     { return LT; }
">="                    { return GEQ; }
">"                     { return GT; }
"=="                    { return EQ; }
"!="                    { return NEQ; }
"="                     { return ASSIGN; }
";"                     { return SEMI; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }

[ \r\n\t]+              /* ignore whitespace */

%%
