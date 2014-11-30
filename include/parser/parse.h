/**
 * @file parse.h
 *
 * @brief Parser entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PARSE_H
#define __PARSE_H

#include <defs.h>
#include <ast/decl/astdeclnode.h>

namespace Parser {

class ParseException : public std::exception {
    private:
        std::string msg;
    public:
        ParseException(std::string msg);
        virtual const char *what() const noexcept override { return msg.c_str(); }
    };

std::shared_ptr<ASTDeclSeqNode> parse(const char *file);

};

#endif
