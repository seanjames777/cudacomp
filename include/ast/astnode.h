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

/**
 * @brief Exception thrown to indicate that the AST is malformed in various
 * parts of the compiler.
 */
class ASTMalformedException : public std::runtime_error {
public:

    /**
     * @brief Constructor
     */
    ASTMalformedException();

};

/**
 * @brief Base class of all AST nodes
 */
class ASTNode {
public:

    /**
     * @brief Destructor
     */
    virtual ~ASTNode();

    /**
     * @brief Print a formatted description of the AST node to an output
     * stream.
     *
     * @param[in] ss Output stream
     */
    virtual void print(std::ostream & ss) = 0;

};

#endif
