/**
 * @file aststmtnode.h
 *
 * @brief Abstract syntax tree abstract statement node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSTMTNODE_H
#define __ASTSTMTNODE_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>

/**
 * @brief Base class for all statement AST nodes
 */
class ASTStmtNode : public ASTNode {
};

/**
 * @brief Sequence of statement nodes
 */
typedef ASTSeqNode<ASTStmtNode> ASTStmtSeqNode;

#endif
