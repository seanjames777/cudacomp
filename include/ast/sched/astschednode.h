/**
 * @file astschednode.h
 *
 * @brief Base class for all scheduling nodes
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSCHEDNODE_H
#define __ASTSCHEDNODE_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>

/**
 * @brief Base class for all scheduling nodes
 */
class ASTSchedNode : public ASTNode {
};

/**
 * @brief Sequence of ASTSchedNode's
 */
typedef ASTSeqNode<ASTSchedNode> ASTSchedSeqNode;

#endif
