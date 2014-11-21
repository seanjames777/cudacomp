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
#include <ast/type/astarg.h>

/**
 * @brief Function type AST type node
 */
class ASTFunType : public ASTTypeNode {
private:

    ASTTypeNode *returnType;
    ASTArgSeqNode *args;

public:

    /**
     * @brief Constructor
     *
     * @param[in] returnType Return type
     * @param[in] args       Argument names and types sequence
     */
    ASTFunType(ASTTypeNode *returnType, ASTArgSeqNode *args);

    /**
     * @brief Get return type
     */
    ASTTypeNode *getReturnType();

    /**
     * @brief Get argument name and types sequence
     */
    ASTArgSeqNode *getArgs();

    /**
     * @brief Check if this type is structurally equal to another type. Names
     * of arguments are not considered: only argument types and return type.
     */
    virtual bool equal(ASTTypeNode *other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
