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
#include <ast/decl/astdeclnode.h>
#include <ast/decl/astfundecl.h>
#include <ast/type/astfuntype.h>
#include <ast/type/astargnode.h>

#include "parser.hpp"

extern std::unordered_map<std::string, ASTTypeNode *> typedefs;

%}

%%

"return"                { return RETURN; }
"int"                   { return INT; }
"bool"                  { return BOOL; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"true"                  { yylval.boolean = true; return TRUE; } // TODO: don't need the boolean
"false"                 { yylval.boolean = false; return FALSE; }
"void"                  { return VOID; }
"typedef"               { return TYPEDEF; }

[0-9]+                  { yylval.number = atoi(yytext); return NUMBER; }
[_a-zA-Z][_a-zA-Z0-9]*  {
                            yylval.string = strdup(yytext);

                            if (typedefs.find(std::string(yytext)) != typedefs.end())
                                return IDTYPE;
                            else
                                return IDENT;
                        }

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
