%{
#include <ast/ast.h>
#include "parser.hpp"
%}

%%

[0-9]+                  { yylval.number = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.string = strdup(yytext); return IDENT; }
"+"                     { return PLUS; }
"*"                     { return TIMES; }
"-"                     { return MINUS; }
"/"                     { return DIV; }
[ \r\n\t]+              /* ignore whitespace */

%%
