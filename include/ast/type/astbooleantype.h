/**
 * @file astbooleantype.h
 *
 * @brief Abstract syntax tree integer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBOOLEANTYPE_H
#define __ASTBOOLEANTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief Boolean type AST type node
 */
class ASTBooleanType : public ASTTypeNode {
private:

    static std::shared_ptr<ASTBooleanType> instance;

public:

    /**
     * @brief Constructor. This is a private constructor: use the singleton
     * get() function.
     */
    ASTBooleanType();

    /**
     * @brief Get the singleton instance of this class
     */
    static std::shared_ptr<ASTBooleanType> get();

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
