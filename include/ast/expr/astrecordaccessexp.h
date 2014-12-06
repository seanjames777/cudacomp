/**
 * @file astrecordaccessexp.h
 *
 * @brief Record field accessing expression or lvalue AST node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTRECORDACCESSEXP_H
#define __ASTRECORDACCESSEXP_H

#include <ast/expr/astexpnode.h>
#include <ast/type/astrecordtype.h>

/**
 * @brief Record field accessing expression/lvalue AST node
 */
class ASTRecordAccessExp : public ASTExpNode {
private:

    std::shared_ptr<ASTExpNode> lvalue;
    std::shared_ptr<ASTRecordType> type;
    std::string id;

public:

    /**
     * @brief Constructor
     *
     * @param[in] lvalue    LValue being accessed
     * @param[in] id        Name of the field to access
     */
    ASTRecordAccessExp(std::shared_ptr<ASTExpNode> lvalue, std::string id);

    /**
     * @brief Get the lvalue being accessed
     */
    std::shared_ptr<ASTExpNode> getLValue();

    /**
     * @brief Get the name of the field
     */
    std::string getId();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
