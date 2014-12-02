%{
#include <iostream>
#include <string>
#include <ast/ast.h>
#include <parser/parse.h>

#define YY_STACK_USED
#include "parser.hpp"

// TODO: Might be able to get the line number more easily
extern std::unordered_map<std::string, ASTTypeNode *> typedefs;
extern int line_num;

int comment_depth = 0;

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
<COMMENT><<EOF>>        {
                            std::stringstream ss;
                            ss << "Line " << line_num << ": Unterminated comment";
                            throw Parser::ParseException(ss.str());
                        }

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
"for"                   { return FOR; }
"struct"                { return STRUCT; }
"continue"              { return CONTINUE; }
"break"                 { return BREAK; }
"assert"                { return ASSERT; }
"NULL"                  { return KWNULL; }
"alloc"                 { return ALLOC; }
"char"                  { return CHAR; }
"string"                { return STRING; }

0|[1-9][0-9]*           {
                            unsigned long long int val = strtoull(yytext, NULL, 10);
                            if (val > (1UL << 31)) {
                                std::stringstream ss;
                                ss << "Line " << line_num << ": Constant too large: " << yytext;
                                throw Parser::ParseException(ss.str());
                            }
                            yylval.int32 = val;
                            return INT32;
                        }
0[xX][0-9A-Fa-f]+       {
                            unsigned long long int val = strtoull(yytext, NULL, 16);
                            if (val > 0xFFFFFFFFUL) {
                                std::stringstream ss;
                                ss << "Line " << line_num << ": Constant too large: " << yytext;
                                throw Parser::ParseException(ss.str());
                            }
                            yylval.int32 = val;
                            return INT32;
                        }
[0-9]*\.[0-9]+          { yylval.float32 = strtod(yytext, NULL); return FLOAT32; }
[_a-zA-Z][_a-zA-Z0-9]*  {
                            yylval.string = strdup(yytext);

                            if (typedefs.find(std::string(yytext)) != typedefs.end())
                                return IDTYPE;
                            else
                                return IDENT;
                        }
"?"                     { return QUESTION; }
":"                     { return COLON; }
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
"++"                    { return INCR; }
"--"                    { return DECR; }
"+="                    { return PLUSEQ; }
"-="                    { return MINUSEQ; }
"*="                    { return TIMESEQ; }
"/="                    { return DIVEQ; }
"%="                    { return MODEQ; }
"<<<"                   { return DIML; }
">>>"                   { return DIMR; }

[ \r\t\v\f]+            /* ignore whitespace */
"\n"                    { line_num++; }

.                       {
                            std::stringstream ss;
                            ss << "Line " << line_num << ": ";
                            ss << "Unexpected non-whitespace character '" << yytext << "'";
                            throw Parser::ParseException(ss.str());
                        }

%%
