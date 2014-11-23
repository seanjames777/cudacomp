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
#include <ast/type/asttypenode.h>

/**
 * @brief Variable declaration statement AST node. Declares and optionally
 * defines a new local variable in a function body. Note: not a top-level
 * declaration, although it would fit the AST's model.
 */
class ASTVarDeclStmt : public ASTStmtNode {
private:

    std::string id;
    std::shared_ptr<ASTExpNode> exp;
    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type Variable type
     * @param[in] id   Variable name
     * @param[in] exp  Initial definition, or null
     */
    ASTVarDeclStmt(std::shared_ptr<ASTTypeNode> type, std::string id, std::shared_ptr<ASTExpNode> exp);

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
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @brief Get variable type
     */
    std::shared_ptr<ASTTypeNode> getType();

    /**
     * @brief Set variable type
     */
    void setType(std::shared_ptr<ASTTypeNode> type);

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
