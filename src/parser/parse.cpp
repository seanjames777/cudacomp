/**
 * @file parse.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <parser/parse.h>

extern int yyparse(Parser::ParserArgs *args);
extern FILE *yyin;

namespace Parser {

ParseException::ParseException(std::string msg)
    : msg(msg)
{
}

std::shared_ptr<ASTDeclSeqNode> parse(std::string file, bool header) {
    FILE *fp = nullptr;

    ParserArgs args;
    args.header = header;
    args.root = nullptr;

    if (!file.empty()) {
        fp = fopen(file.c_str(), "r");

        if (!fp) {
            std::stringstream ss;
            ss << "Cannot open file '" << file << "'";
            throw ParseException(ss.str());
        }

        yyin = fp;
    }

    if (yyparse(&args)) {
        if (!file.empty())
            fclose(fp);

        return nullptr;
    }

    if (!file.empty())
        fclose(fp);

    return args.root;
}

};
