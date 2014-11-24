/**
 * @file asttopnode.h
 *
 * @brief Top-level declaration or definition asbtract node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTDECLNODE_H
#define __ASTDECLNODE_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>

/**
 * @brief Base class for all top-level declarations and definitions in a module,
 * such as functions, global variables, types, structs, etc.
 */
class ASTDeclNode : public ASTNode {
public:

    /**
     * @brief Function linkage options
     *
     * Currently this is only used for function declarations, but it could be
     * extended to structs, global variables, etc.
     */
    enum Linkage {
        Internal,  //!< Defined in the current module
        External   //!< Defined outside the current module
    };

    /**
     * @brief Constructor
     */
    ASTDeclNode();

};

/**
 * @brief Sequence of top-level nodes
 */
typedef ASTSeqNode<ASTDeclNode> ASTDeclSeqNode;

#endif
