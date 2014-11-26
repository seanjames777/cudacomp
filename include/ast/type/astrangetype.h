/**
 * @file astrangetype.h
 *
 * @brief Abstract syntax tree range type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTRANGETYPE_H
#define __ASTRANGETYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief AST type node for a range of integers
 */
class ASTRangeType : public ASTTypeNode {
private:

    static std::shared_ptr<ASTRangeType> instance;

public:

    /**
     * @brief Constructor
     */
    ASTRangeType();

    /**
     * @brief Get the singleton instance of this class
     */
    static std::shared_ptr<ASTRangeType> get();

    /**
     * @copydoc ASTTypeNodeNode::equal()
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

    /**
     * @copydoc ASTTypeNode::getSize()
     */
    virtual int getSize() override;

};

#endif
