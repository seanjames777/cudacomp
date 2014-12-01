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

std::shared_ptr<ASTDeclSeqNode> parse(std::string file) {
    FILE *fp = nullptr;

    if (!file.empty()) {
        fp = fopen(file.c_str(), "r");

        if (!fp) {
            std::stringstream ss;
            ss << "Cannot open file '" << file << "'";
            throw ParseException(ss.str());
        }

        yyin = fp;
    }

    std::shared_ptr<ASTDeclSeqNode> root = nullptr;

    if (yyparse(&root)) {
        if (!file.empty())
            fclose(fp);

        return nullptr;
    }

    if (!file.empty())
        fclose(fp);

    return root;
}

};
