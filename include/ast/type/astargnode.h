/**
 * @file astargnode.h
 *
 * @brief Abstract syntax function argument node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTARGNODE_H
#define __ASTARGNODE_H

#include <ast/type/asttypenode.h>
#include <ast/astseqnode.h>

/**
 * @brief Function argument signature AST Node. Combines a type and an argument
 * name. This is not strictly a type, and is also used in function declarations
 * and definitions, but it fits best here as it is especially not an expression
 * or statement.
 */
struct ASTArgNode : public ASTNode {
private:

    std::shared_ptr<ASTTypeNode> type;

    // Do not rely on the name, it is only used for parsing function definitions
    std::string name;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type Argument type
     * @param[in] name Argument name
     */
    ASTArgNode(std::shared_ptr<ASTTypeNode> type, std::string name);

    /**
     * @brief Get argument type
     */
    std::shared_ptr<ASTTypeNode> getType();

    /**
     * @brief Set argument type
     */
    void setType(std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Get argument name
     */
    std::string getName();

    /**
     * @brief Set argument name
     */
    void setName(std::string name);

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

/**
 * @brief Sequence of ASTArgNode nodes
 */
typedef ASTSeqNode<ASTArgNode> ASTArgSeqNode;

#endif
