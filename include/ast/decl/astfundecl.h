/**
 * @file astfundecl.h
 *
 * @brief Function declaration/definition top-level abstract syntax tree node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFUNDECL_H
#define __ASTFUNDECL_H

#include <ast/decl/astdeclnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astfuntype.h>

/**
 * @brief Function declaration/definition top-level AST node
 *
 * TODO: Make body optional to support forward declarations.
 */
class ASTFunDecl : public ASTDeclNode {
private:

    std::string name;
    std::shared_ptr<ASTFunType> sig;
    bool defn;
    enum Linkage linkage;
    std::shared_ptr<ASTStmtSeqNode> body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] name Function name
     * @param[in] sig  Function signature
     * @param[in] defn Whether this is a definition as well as a declaration
     * @param[in] body Function body statement sequence. Only valid if 'defn' is true.
     */
    ASTFunDecl(std::string name, std::shared_ptr<ASTFunType> sig, bool defn,
        enum ASTDeclNode::Linkage linkage, std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @brief Destructor
     */
    ~ASTFunDecl();

    /**
     * @brief Get function name
     */
    std::string getName();

    /**
     * @brief Get whether this is a definition
     */
    bool isDefn();

    /**
     * @brief Get function type/signature
     */
    std::shared_ptr<ASTFunType> getSignature();

    /**
     * @brief Get function body statement sequence
     */
    std::shared_ptr<ASTStmtSeqNode> getBody();

    /**
     * @brief Get the linkage
     */
    enum ASTDeclNode::Linkage getLinkage();

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
