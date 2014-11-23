/**
 * @file astarrtype.h
 *
 * @brief Abstract syntax tree array type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTARRTYPE_H
#define __ASTARRTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief AST type node for an array of values of another type
 */
class ASTArrType : public ASTTypeNode {
private:

    std::shared_ptr<ASTTypeNode> elemType;

public:

    /**
     * @brief Constructor
     *
     * @param[in] elemType Type pointers of this type point to
     */
    ASTArrType(std::shared_ptr<ASTTypeNode> elemType);

    /**
     * @brief Get type pointers of this type point to
     */
    std::shared_ptr<ASTTypeNode> getElemType();

    /**
     * @brief Set type pointers of this type point to
     */
    void setElemType(std::shared_ptr<ASTTypeNode> elemType);

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
