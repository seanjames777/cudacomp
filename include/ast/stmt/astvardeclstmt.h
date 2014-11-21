/**
 * @file astvardeclstmt.h
 *
 * @brief Abstract syntax tree variable declaration statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVARDECLSTMT_H
#define __ASTVARDECLSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>
#include <ast/type/asttype.h>

/**
 * @brief Variable declaration statement AST node. Declares and optionally
 * defines a new local variable in a function body.
 */
class ASTVarDeclStmt : public ASTStmtNode {
private:

    std::string id;
    ASTExpNode *exp;
    ASTType *type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type Variable type
     * @param[in] id   Variable name
     * @param[in] exp  Initial definition, or null
     */
    ASTVarDeclStmt(ASTType *type, std::string id, ASTExpNode *exp);

    /**
     * @brief Destructor
     */
    ~ASTVarDeclStmt();

    /**
     * @brief Get variable name
     */
    std::string getId();

    /**
     * @brief Get initial variable definition. May be null.
     */
    ASTExpNode *getExp();

    /**
     * @brief Get variable type
     */
    ASTType *getType();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
