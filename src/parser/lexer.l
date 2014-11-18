%{
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

#include "parser.hpp"
%}

%%

"return"                { return RETURN; }
"int"                   { return INT; }

[0-9]+                  { yylval.number = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.string = strdup(yytext); return IDENT; }
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
"="                     { return ASSIGN; }
";"                     { return SEMI; }

[ \r\n\t]+              /* ignore whitespace */

%%
