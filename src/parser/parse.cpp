/**
 * @file parse.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <parser/parse.h>

extern int yyparse(ASTStmtNode **root);
extern FILE *yyin;

namespace Parser {

ASTStmtNode *parse(const char *file) {
    FILE *fp = NULL;

    if (file) {
        fp = fopen(file, "r");

        if (!fp)
            return NULL;

        yyin = fp;
    }

    ASTStmtNode *root = NULL;

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
