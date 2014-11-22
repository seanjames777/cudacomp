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
#include <ast/top/asttopnode.h>

namespace Parser {

std::shared_ptr<ASTTopSeqNode> parse(const char *file);

};

#endif
