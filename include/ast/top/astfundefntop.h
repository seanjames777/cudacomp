/**
 * @file astfundefntop.h
 *
 * @brief Function definition top-level abstract syntax tree node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFUNDEFNTOP_H
#define __ASTFUNDEFNTOP_H

#include <ast/top/asttopnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astfuntype.h>

/**
 * @brief Function definition top-level AST node
 */
class ASTFunDefnTop : public ASTTopNode {
private:

    std::string name;
    std::shared_ptr<ASTFunType> sig;
    std::shared_ptr<ASTStmtSeqNode> body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] name Function name
     * @param[in] sig  Function signature
     * @param[in] body Function body statement sequence
     */
    ASTFunDefnTop(std::string name, std::shared_ptr<ASTFunType> sig, std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @brief Destructor
     */
    ~ASTFunDefnTop();

    /**
     * @brief Get function name
     */
    std::string getName();

    /**
     * @brief Get function type/signature
     */
    std::shared_ptr<ASTFunType> getSignature();

    /**
     * @brief Get function body statement sequence
     */
    std::shared_ptr<ASTStmtSeqNode> getBody();

    /**
     * @brief Set the function body statement sequence
     */
    void setBody(std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
