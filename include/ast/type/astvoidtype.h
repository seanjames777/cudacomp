/**
 * @file astvoidtype.h
 *
 * @brief Abstract syntax tree void type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVOIDTYPE_H
#define __ASTVOIDTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief Void type AST type node.
 */
class ASTVoidType : public ASTTypeNode {
private:

    static std::shared_ptr<ASTVoidType> instance;

public:

    /**
     * @brief Constructor. This is a private constructor: use the singleton
     * get() function.
     */
    ASTVoidType();

    /**
     * @brief Get the singleton instance of this class
     */
    static std::shared_ptr<ASTVoidType> get();

    /**
     * @copydoc ASTTypeNode::equal()
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;
};

#endif
