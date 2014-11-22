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

/**
 * @brief Base class for all top-level declarations and definitions in a module,
 * such as functions, global variables, types, structs, etc.
 */
class ASTTopNode : public ASTNode {
public:

    /**
     * @brief Constructor
     */
    ASTTopNode();

};

/**
 * @brief Sequence of top-level nodes
 */
typedef ASTSeqNode<ASTTopNode> ASTTopSeqNode;

#endif
