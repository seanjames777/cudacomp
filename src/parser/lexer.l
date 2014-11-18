%{
#include <ast/astexpnode.h>
#include <ast/aststmtnode.h>
#include <ast/astinteger.h>
#include <ast/astidentifier.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>

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
"="                     { return ASSIGN; }
";"                     { return SEMI; }

[ \r\n\t]+              /* ignore whitespace */

%%
