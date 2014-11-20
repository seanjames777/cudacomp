/**
 * @file asttopnode.h
 *
 * @brief Top-level declaration or definition asbtract node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTOPNODE_H
#define __ASTTOPNODE_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>

class ASTTopNode : public ASTNode {
};

typedef ASTSeqNode<ASTTopNode> ASTTopSeqNode;

#endif
