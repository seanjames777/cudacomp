/**
 * @file astallocexp.h
 *
 * @brief Pointer allocation expression
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTALLOCEXP_H
#define __ASTALLOCEXP_H

#include <ast/expr/astexpnode.h>
#include <ast/type/asttypenode.h>

/**
 * @brief Pointer allocation AST expression node
 */
class ASTAllocExp : public ASTExpNode {
private:

    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type Allocation element type
     */
    ASTAllocExp(std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Get allocation element type
     */
    std::shared_ptr<ASTTypeNode> getElemType();

    /**
     * @brief Set allocation element type
     */
    void setElemType(std::shared_ptr<ASTTypeNode> type);
    
    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
