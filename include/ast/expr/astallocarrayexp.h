/**
 * @file astallocarrayexp.h
 *
 * @brief Array allocation expression
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTARRAYALLOCEXP_H
#define __ASTARRAYALLOCEXP_H

#include <ast/expr/astexpnode.h>
#include <ast/type/asttypenode.h>

/**
 * @brief Array allocation AST expression node
 */
class ASTAllocArrayExp : public ASTExpNode {
private:

    std::shared_ptr<ASTTypeNode> type;
    std::shared_ptr<ASTExpNode> length;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type   Array element type
     * @param[in] length Array length
     */
    ASTAllocArrayExp(std::shared_ptr<ASTTypeNode> type, std::shared_ptr<ASTExpNode> length);

    /**
     * @brief Get array element type
     */
    std::shared_ptr<ASTTypeNode> getElemType();

    /**
     * @brief Get array length
     */
    std::shared_ptr<ASTExpNode> getLength();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
