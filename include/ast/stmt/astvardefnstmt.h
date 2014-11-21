/**
 * @file astvardefnstmt.h
 *
 * @brief Abstract syntax tree variable definition statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVARDEFNSTMT_H
#define __ASTVARDEFNSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Variable definition statement AST node. Assigns a new value to an
 * existing variable.
 */
class ASTVarDefnStmt : public ASTStmtNode {
private:

    std::string id;
    ASTExpNode *exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id  Variable name
     * @param[in] exp New value expression
     */
    ASTVarDefnStmt(std::string id, ASTExpNode *exp);

    /**
     * @brief Destructor
     */
    ~ASTVarDefnStmt();

    /**
     * @brief Get variable name to assign to
     */
    std::string getId();

    /**
     * @brief Get expression to assign value from
     */
    ASTExpNode *getExp();

    /**
     * @coypdoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
