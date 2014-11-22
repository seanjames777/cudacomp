/**
 * @file asttypedefntop.h
 *
 * @brief Type definition top-level abstract syntax tree node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTYPEDEFNTOP_H
#define __ASTTYPEDEFNTOP_H

#include <ast/top/asttopnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astfuntype.h>

/**
 * @brief Type definition top-level AST node
 */
class ASTTypeDefnTop : public ASTTopNode {
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
    ASTTypeDefnTop(std::string name, std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Destructor
     */
    ~ASTTypeDefnTop();

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
