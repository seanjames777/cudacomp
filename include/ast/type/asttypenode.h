/**
 * @file asttypenode.h
 *
 * @brief Abstract syntax tree abstract type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTYPENODE_H
#define __ASTTYPENODE_H

#include <ast/astnode.h>

/**
 * @brief Abstract base class for all AST type classes
 */
class ASTTypeNode : public ASTNode {
public:

    /**
     * @brief Destructor
     */
    virtual ~ASTTypeNode() = 0;

    /**
     * @brief Check if this type is structurally equal to another type
     *
     * @param[in] other ASTTypeNode to compare to
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) = 0;

};

#endif
