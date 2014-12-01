%{
#include <iostream>
#include <string>
#include <ast/ast.h>

#define YY_STACK_USED
#include "parser.hpp"

// TODO: Might be able to get the line number more easily
extern std::unordered_map<std::string, ASTTypeNode *> typedefs;
extern int line_num;

int comment_depth = 0;

// TODO: should freak out when we can't fit a number in a type

%}

%x COMMENT
%x LINE_COMMENT

%%

"/*"                    { BEGIN(COMMENT); comment_depth++; }
<COMMENT>[^\n*/]*         /* ignore */
<COMMENT>"\n"           { line_num++; }
<COMMENT>"/*"           { comment_depth++; }
<COMMENT>"*/"           { if (--comment_depth == 0) { BEGIN(INITIAL); } }
<COMMENT>[*/]           /* ignore */

"//"                    { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>[^\n]*    /* ignore */
<LINE_COMMENT>"\n"      { BEGIN(INITIAL); line_num++; }

"return"                { return RETURN; }
"while"                 { return WHILE; }
"int"                   { return INT; }
"bool"                  { return BOOL; }
"float"                 { return FLOAT; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"true"                  { yylval.boolean = true; return TRUE; } // TODO: don't need the boolean
"false"                 { yylval.boolean = false; return FALSE; }
"void"                  { return VOID; }
"typedef"               { return TYPEDEF; }
"extern"                { return EXTERN; }
"alloc_array"           { return ALLOC_ARRAY; }
":"                     { return COLON; }
"to"                    { return TO; }
"for"                   { return FOR; }
"@device"               { return DEVICE; }

[0-9]+                  { yylval.int32 = strtol(yytext, NULL, 10); return INT32; }
0[xX][0-9A-Fa-f]+       { yylval.int32 = strtol(yytext, NULL, 16); return INT32; }
[0-9]*\.[0-9]+           { yylval.float32 = strtod(yytext, NULL); return FLOAT32; }
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
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }

[ \r\t]+                /* ignore whitespace */
"\n"                    { line_num++; }

%%
