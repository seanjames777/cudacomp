/**
 * @file parse.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <parser/parse.h>

extern int yyparse(std::shared_ptr<ASTDeclSeqNode> *root);
extern FILE *yyin;

namespace Parser {

ParseException::ParseException(std::string msg)
    : msg(msg)
{
}

std::shared_ptr<ASTDeclSeqNode> parse(const char *file) {
    FILE *fp = nullptr;

    if (file) {
        fp = fopen(file, "r");

        if (!fp)
            return nullptr;

        yyin = fp;
    }

    std::shared_ptr<ASTDeclSeqNode> root = nullptr;

    if (yyparse(&root)) {
        if (file)
            fclose(fp);

        return nullptr;
    }

    if (file)
        fclose(fp);

    return root;
}

};
