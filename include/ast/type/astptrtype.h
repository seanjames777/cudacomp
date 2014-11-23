/**
 * @file astptrtype.h
 *
 * @brief Abstract syntax tree pointer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTPTRTYPE_H
#define __ASTPTRTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief AST type node for a type of a pointer to a value of another type
 */
class ASTPtrType : public ASTTypeNode {
private:

    std::shared_ptr<ASTTypeNode> toType;

public:

    /**
     * @brief Constructor
     *
     * @param[in] toType Type pointers of this type point to
     */
    ASTPtrType(std::shared_ptr<ASTTypeNode> toType);

    /**
     * @brief Get type pointers of this type point to
     */
    std::shared_ptr<ASTTypeNode> getToType();

    /**
     * @brief Set type pointers of this type point to
     */
    void setToType(std::shared_ptr<ASTTypeNode> toType);

    /**
     * @copydoc ASTTypeNodeNode::equal()
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
