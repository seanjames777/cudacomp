/**
 * @file parse.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <parser/parse.h>

extern int yyparse(std::shared_ptr<ASTTopSeqNode> *root);
extern FILE *yyin;

namespace Parser {

std::shared_ptr<ASTTopSeqNode> parse(const char *file) {
    FILE *fp = NULL;

    if (file) {
        fp = fopen(file, "r");

        if (!fp)
            return NULL;

        yyin = fp;
    }

    std::shared_ptr<ASTTopSeqNode> root = nullptr;

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
