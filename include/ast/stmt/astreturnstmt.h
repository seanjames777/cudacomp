/**
 * @file astreturnstmt.h
 *
 * @brief Abstract syntax tree return statement node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTRETURNSTMT_H
#define __ASTRETURNSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Return statement AST node. Returns to the calling function, optionally
 * passing a return value.
 */
class ASTReturnStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] exp Value expression to return, or null
     */
    ASTReturnStmt(std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Destructor
     */
    ~ASTReturnStmt();

    /**
     * @brief Get value to return. May be null.
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
