/**
 * @file parse.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PARSE_H
#define __PARSE_H

#include <defs.h>
#include <ast/astnode.h>
#include <parser/parse.h>

extern int yyparse(ASTNode **root);
extern FILE *yyin;

namespace Parser {

ASTNode *parse(const char *file) {
    FILE *fp = NULL;

    if (file) {
        fp = fopen(file, "r");

        if (!fp)
            return NULL;

        yyin = fp;
    }

    ASTNode *root = NULL;

    if (yyparse(&root)) {
        if (file)
            fclose(fp);

        return NULL;
    }

    if (file)
        fclose(fp);

    return root;
}

};

#endif
