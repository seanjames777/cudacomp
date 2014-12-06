/**
 * @file astfloattype.h
 *
 * @brief Abstract syntax tree integer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFLOATTYPE_H
#define __ASTFLOATTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief Integer type AST type node. Currently, these represent signed 32-bit
 * integers only.
 */
class ASTFloatType : public ASTTypeNode {
private:

    static std::shared_ptr<ASTFloatType> instance;

public:

    /**
     * @brief Constructor. This is a private constructor: use the singleton
     * get() function.
     */
    ASTFloatType();

    /**
     * @brief Get the singleton instance of this class
     */
    static const std::shared_ptr<ASTFloatType> get();

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
