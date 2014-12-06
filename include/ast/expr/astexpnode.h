/**
 * @file astexpnode.h
 *
 * @brief Abstract syntax tree abstract expression node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTEXPNODE_H
#define __ASTEXPNODE_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>
#include <ast/type/asttypenode.h>

/**
 * @brief Base class for all expression AST nodes
 */
class ASTExpNode : public ASTNode {
private:

    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     */
    ASTExpNode();

    /**
     * @brief Destructor
     */
    virtual ~ASTExpNode() = 0;

    /**
     * @brief Get expression type. Only valid after typechecker assigns a type
     */
    std::shared_ptr<ASTTypeNode> getType();

    /**
     * @brief Set expression type
     */
    void setType(std::shared_ptr<ASTTypeNode> type);

};

/**
 * @brief Sequence of expression nodes
 */
typedef ASTSeqNode<ASTExpNode> ASTExpSeqNode;

#endif
