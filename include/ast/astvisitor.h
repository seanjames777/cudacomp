/**
 * @file astvisitor.h
 *
 * @brief Abstract syntax tree visitor
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVISITOR_H
#define __ASTVISITOR_H

#include <ast/ast.h>

#define VISIT_HDR(NAME, ARG) virtual bool visit##NAME(std::shared_ptr<AST##NAME> ARG);
#define VISIT_IMPL(NAME, ARG) \
    template<typename T> \
    bool ASTVisitor<T>::visit##NAME(std::shared_ptr<AST##NAME> ARG)
#define VISIT_CALL(NAME, ARG) visit##NAME(ARG)
#define NODE_MATCH(NAME, ARG1, ARG2) std::shared_ptr<AST##NAME> ARG1 = std::dynamic_pointer_cast<AST##NAME>(ARG2)

// TODO: Might want to just make the AST immutable after all. Makes visiting hard.
// TODO: Might want a generic interface to AST node children. Then these could all share an
//       implementation.
// TODO: Might want to break out the casting logic from the visit logic.

/**
 * @brief Base class for all classes that "visit" abstract syntax trees. This is a recursive
 * visitor pattern implementation, on which many analyses such as type checking and code
 * generation can be built.
 *
 * Analyses should override the visitXXX() method for each node type the analysis is interested
 * in. These functions return a boolean indicating whether the analyses should visit the node's
 * remaining siblings. Overriden implementations are responsible for visiting their children, as
 * well as respecting the return value as described previously.
 *
 * The default implementation visits a node's children until one of them returns false, then
 * returns false unless all of the children returned true. The actual visit logic does nothing.
 *
 * Note that it is possible to avoid entirely terminating an analysis by "catching" a false return
 * value higher up the tree.
 */
template<typename T>
class ASTVisitor {
private:

    std::vector<T> stack;

protected:

    void push(T elem);

    T pop();

public:

    VISIT_HDR(DeclNode, node)
    VISIT_HDR(DeclSeqNode, node)
    VISIT_HDR(FunDecl, node)
    VISIT_HDR(TypeDecl, node)
    VISIT_HDR(ExpNode, node)
    VISIT_HDR(ExpSeqNode, node)
    VISIT_HDR(AllocArrayExp, node)
    VISIT_HDR(BinopExp, node)
    VISIT_HDR(BooleanExp, node)
    VISIT_HDR(CallExp, node)
    VISIT_HDR(IdentifierExp, node)
    VISIT_HDR(IndexExp, node)
    VISIT_HDR(IntegerExp, node)
    VISIT_HDR(UnopExp, node)
    VISIT_HDR(StmtNode, node)
    VISIT_HDR(StmtSeqNode, node)
    VISIT_HDR(AssignStmt, node)
    VISIT_HDR(ExprStmt, node)
    VISIT_HDR(IfStmt, node)
    VISIT_HDR(ReturnStmt, node)
    VISIT_HDR(ScopeStmt, node)
    VISIT_HDR(VarDeclStmt, node)
    VISIT_HDR(WhileStmt, node)
    VISIT_HDR(ArgNode, node)
    VISIT_HDR(ArgSeqNode, node)
    VISIT_HDR(TypeNode, node)
    VISIT_HDR(ArrType, node)
    VISIT_HDR(BooleanType, node)
    VISIT_HDR(FunType, node)
    VISIT_HDR(IdType, node)
    VISIT_HDR(IntegerType, node)
    VISIT_HDR(PtrType, node)
    VISIT_HDR(VoidType, node)

};

#endif
