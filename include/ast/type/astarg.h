/**
 * @file astarg.h
 *
 * @brief Abstract syntax function argument node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTARG_H
#define __ASTARG_H

#include <ast/type/asttypenode.h>
#include <ast/astseqnode.h>

/**
 * @brief Function argument signature AST Node. Combines a type and an argument
 * name. This is not strictly a type, and is also used in function declarations
 * and definitions, but it fits best here as it is especially not an expression
 * or statement.
 */
struct ASTArg : public ASTNode {
private:

    ASTTypeNode *type;

    // Do not rely on the name, it is only used for parsing function definitions
    std::string name;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type Argument type
     * @param[in] name Argument name
     */
    ASTArg(ASTTypeNode *type, std::string name);

    /**
     * @brief Get argument type
     */
    ASTTypeNode *getType();

    /**
     * @brief Get argument name
     */
    std::string getName();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

/**
 * @brief Sequence of ASTArg nodes
 */
typedef ASTSeqNode<ASTArg> ASTArgSeqNode;

#endif
