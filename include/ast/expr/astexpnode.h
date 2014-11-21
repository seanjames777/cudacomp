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

/**
 * @brief Base class for all expression AST nodes
 */
class ASTExpNode : public ASTNode {
};

/**
 * @brief Sequence of expression nodes
 */
typedef ASTSeqNode<ASTExpNode> ASTExpSeqNode;

#endif
