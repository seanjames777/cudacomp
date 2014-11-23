/**
 * @file astidtype.h
 *
 * @brief Abstract syntax named type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIDTYPE_H
#define __ASTIDTYPE_H

#include <ast/type/asttypenode.h>

/**
 * @brief Function type AST type node
 */
class ASTIdType : public ASTTypeNode {
private:

    std::string id;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id Type name
     */
    ASTIdType(std::string id);

    /**
     * @brief Get the type name
     */
    std::string getId();

    /**
     * @brief Check if this type is structurally equal to another type. Names
     * of arguments are not considered: only argument types and return type.
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
