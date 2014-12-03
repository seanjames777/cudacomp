/**
 * @file astfuntype.h
 *
 * @brief Abstract syntax tree function type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFUNTYPE_H
#define __ASTFUNTYPE_H

#include <ast/type/asttypenode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astvoidtype.h>

/**
 * @brief Function type AST type node
 */
class ASTFunType : public ASTTypeNode {
private:

    std::shared_ptr<ASTTypeNode> returnType;
    std::shared_ptr<ASTArgSeqNode> dim_args;
    std::shared_ptr<ASTArgSeqNode> args;

public:

    /**
     * @brief Constructor
     *
     * @param[in] returnType Return type
     * @param[in] dim_args   Dimensional argument names and type sequence
     * @param[in] args       Argument names and types sequence
     */
    ASTFunType(
        std::shared_ptr<ASTTypeNode> returnType,
        std::shared_ptr<ASTArgSeqNode> dim_args,
        std::shared_ptr<ASTArgSeqNode> args);

    /**
     * @brief Get return type
     */
    std::shared_ptr<ASTTypeNode> getReturnType();

    /**
     * @brief Set return type
     */
    void setReturnType(std::shared_ptr<ASTTypeNode> returnType);

    /**
     * @brief Get dimensional argument name and types sequence
     */
    std::shared_ptr<ASTArgSeqNode> getDimArgs();

    /**
     * @brief Get argument name and types sequence
     */
    std::shared_ptr<ASTArgSeqNode> getArgs();

    /**
     * @brief Get number of arguments
     */
    int getNumArgs();

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
