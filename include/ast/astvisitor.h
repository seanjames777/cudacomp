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

// TODO: Might want to just make the AST immutable after all. Makes visiting hard.
// TODO: Might want a generic interface to AST node children. Then these could all share an
//       implementation.
// TODO: Might want to break out the casting logic from the visit logic.

#define DECLARE_CLASS(NAME) virtual void visit##NAME(std::shared_ptr<AST##NAME> node);
#define IMPL_CLASS(NAME)    void ASTVisitor::visit##NAME(std::shared_ptr<AST##NAME> node)
#define SUB_CLASS(SUB) \
    if (std::shared_ptr<AST##SUB> ptr = std::dynamic_pointer_cast<AST##SUB>(node)) \
        visit##SUB(ptr);
#define HAS_MEMBER(ARG) \
    if (node->ARG) \
        visitNode(node->ARG);
#define SEQ_CLASS(ELEM) \
    while (node != nullptr) {\
        visitNode(node->getHead());\
        node = node->getTail();\
    }

/**
 * @brief Base class for all classes that "visit" abstract syntax trees. This is a recursive
 * visitor pattern implementation, on which many analyses such as type checking and code
 * generation can be built.
 *
 * Analyses should override the visitXXX() method for each node type the analysis is interested
 * in. Overriden implementations are responsible for visiting their children, possibly by calling
 * the base class's implemetation. The visitXXX() function will be called for each superclass of
 * a node, from most to least general. Nodes may be visited more than once.
 *
 * The default implementation visits a node's children, unless the node has no children, in which
 * case it does nothing.
 *
 * For help editing this file and the implementation file, see instructions in astvisitor.cpp.
 */
class ASTVisitor {
public:

    DECLARE_CLASS(Node);

    DECLARE_CLASS(DeclNode);
    DECLARE_CLASS(ExpNode);
    DECLARE_CLASS(StmtNode);
    DECLARE_CLASS(ArgNode);
    DECLARE_CLASS(TypeNode);

    DECLARE_CLASS(DeclSeqNode);
    DECLARE_CLASS(ExpSeqNode);
    DECLARE_CLASS(StmtSeqNode);
    DECLARE_CLASS(ArgSeqNode);

    DECLARE_CLASS(FunDecl);
    DECLARE_CLASS(TypeDecl);
    DECLARE_CLASS(RecordDecl);

    DECLARE_CLASS(AllocArrayExp);
    DECLARE_CLASS(AllocExp);
    DECLARE_CLASS(BinopExp);
    DECLARE_CLASS(BooleanExp);
    DECLARE_CLASS(CallExp);
    DECLARE_CLASS(DerefExp);
    DECLARE_CLASS(IdentifierExp);
    DECLARE_CLASS(IndexExp);
    DECLARE_CLASS(IntegerExp);
    DECLARE_CLASS(RecordAccessExp);
    DECLARE_CLASS(TernopExp);
    DECLARE_CLASS(UnopExp);

    DECLARE_CLASS(AssertStmt);
    DECLARE_CLASS(AssignStmt);
    DECLARE_CLASS(ExprStmt);
    DECLARE_CLASS(ForStmt);
    DECLARE_CLASS(IfStmt);
    DECLARE_CLASS(ReturnStmt);
    DECLARE_CLASS(ScopeStmt);
    DECLARE_CLASS(VarDeclStmt);
    DECLARE_CLASS(WhileStmt);

    DECLARE_CLASS(ArrType);
    DECLARE_CLASS(BooleanType);
    DECLARE_CLASS(FunType);
    DECLARE_CLASS(IdType);
    DECLARE_CLASS(IntegerType);
    DECLARE_CLASS(RecordType);
    DECLARE_CLASS(PtrType);
    DECLARE_CLASS(VoidType);

};

#endif
