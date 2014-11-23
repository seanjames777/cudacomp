/**
 * @file asttypedefntop.h
 *
 * @brief Type definition top-level abstract syntax tree node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTYPEDECL_H
#define __ASTTYPEDECL_H

#include <ast/decl/astdeclnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astfuntype.h>

/**
 * @brief Type declaration top-level AST node
 */
class ASTTypeDecl : public ASTDeclNode {
private:

    std::string name;
    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] name Type name
     * @param[in] type Actual type
     */
    ASTTypeDecl(std::string name, std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Destructor
     */
    ~ASTTypeDecl();

    /**
     * @brief Get type name
     */
    std::string getName();

    /**
     * @brief Get actual type
     */
    std::shared_ptr<ASTTypeNode> getType();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
