/**
 * @file astnode.h
 *
 * @brief Abstract syntax tree abstract node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTNODE_H
#define __ASTNODE_H

#include <defs.h>

class ASTMalformedException : public std::runtime_error {
public:

    ASTMalformedException();

};

class ASTNode {
public:

    virtual ~ASTNode();

};

#endif
