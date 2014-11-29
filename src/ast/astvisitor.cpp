/**
 * @file astvisitor.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astvisitor.h>

// HELP: Because of the lack of reflection/RTTI in C++, this file generates a scary macro
// implementing the AST visitor interface. I suggest looking at the existing code for examples.
// Nonetheless, how to edit this file:
//
//   - Add an IMPL_CLASS for new AST nodes
//   - Add a DECLARE_CLASS in the header
//   - Add a SUB_CLASS entry in the IMPL_CLASS for every superclass of every AST type
//   - Add a SEQ_CLASS in an additional IMPL_CLASS if a class has a corresponding sequence type
//   - Add a HAS_MEMBER() for each AST node that should be traversed recursively

IMPL_CLASS(Node) {
    SUB_CLASS(DeclNode);
    SUB_CLASS(ExpNode);
    SUB_CLASS(StmtNode);
    SUB_CLASS(ArgNode);
    SUB_CLASS(TypeNode);

    SUB_CLASS(DeclSeqNode);
    SUB_CLASS(ExpSeqNode);
    SUB_CLASS(StmtSeqNode);
    SUB_CLASS(ArgSeqNode);
}

IMPL_CLASS(DeclNode) {
    SUB_CLASS(FunDecl);
    SUB_CLASS(RecordDecl);
    SUB_CLASS(TypeDecl);
}

IMPL_CLASS(ExpNode) {
    SUB_CLASS(AllocArrayExp);
    SUB_CLASS(AllocExp);
    SUB_CLASS(BinopExp);
    SUB_CLASS(BooleanExp);
    SUB_CLASS(CallExp);
    SUB_CLASS(DerefExp);
    SUB_CLASS(IdentifierExp);
    SUB_CLASS(IndexExp);
    SUB_CLASS(IntegerExp);
    SUB_CLASS(RangeExp);
    SUB_CLASS(RecordAccessExp);
    SUB_CLASS(UnopExp);
}

IMPL_CLASS(StmtNode) {
    SUB_CLASS(AssignStmt);
    SUB_CLASS(ExprStmt);
    SUB_CLASS(IfStmt);
    SUB_CLASS(RangeForStmt);
    SUB_CLASS(ReturnStmt);
    SUB_CLASS(ScopeStmt);
    SUB_CLASS(VarDeclStmt);
    SUB_CLASS(WhileStmt);
}

IMPL_CLASS(ArgNode) {
    HAS_MEMBER(getType());
}

IMPL_CLASS(TypeNode) {
    SUB_CLASS(ArrType);
    SUB_CLASS(BooleanType);
    SUB_CLASS(FunType);
    SUB_CLASS(IdType);
    SUB_CLASS(IntegerType);
    SUB_CLASS(PtrType);
    SUB_CLASS(RangeType);
    SUB_CLASS(RecordType);
    SUB_CLASS(VoidType);
}

IMPL_CLASS(DeclSeqNode) {
    SEQ_CLASS(DeclNode);
}

IMPL_CLASS(ExpSeqNode) {
    SEQ_CLASS(ExpNode);
}

IMPL_CLASS(StmtSeqNode) {
    SEQ_CLASS(StmtNode);
}

IMPL_CLASS(ArgSeqNode) {
    SEQ_CLASS(ArgNode);
}

IMPL_CLASS(FunDecl) {
    HAS_MEMBER(getSignature())
    HAS_MEMBER(getBody())
}

IMPL_CLASS(TypeDecl) {
    HAS_MEMBER(getType());
}

IMPL_CLASS(RecordDecl) {
    HAS_MEMBER(getSignature());
}

IMPL_CLASS(AllocArrayExp) {
    HAS_MEMBER(getElemType());
    HAS_MEMBER(getLength());
}

IMPL_CLASS(AllocExp) {
    HAS_MEMBER(getElemType());
}

IMPL_CLASS(BinopExp) {
    HAS_MEMBER(getE1());
    HAS_MEMBER(getE2());
}

IMPL_CLASS(BooleanExp) {
}

IMPL_CLASS(CallExp) {
    HAS_MEMBER(getArgs());
}

IMPL_CLASS(DerefExp) {
    HAS_MEMBER(getExp());
}

IMPL_CLASS(IdentifierExp) {
}

IMPL_CLASS(IndexExp) {
    HAS_MEMBER(getLValue());
    HAS_MEMBER(getSubscript());
}

IMPL_CLASS(IntegerExp) {
}

IMPL_CLASS(RangeExp) {
    HAS_MEMBER(getMin());
    HAS_MEMBER(getMax());
}

IMPL_CLASS(RecordAccessExp) {
    HAS_MEMBER(getLValue());
}

IMPL_CLASS(UnopExp) {
    HAS_MEMBER(getExp());
}

IMPL_CLASS(ExprStmt) {
    HAS_MEMBER(getExp());
}

IMPL_CLASS(IfStmt) {
    HAS_MEMBER(getCond());
    HAS_MEMBER(getTrueStmt());
    HAS_MEMBER(getFalseStmt());
}

IMPL_CLASS(WhileStmt) {
    HAS_MEMBER(getCond());
    HAS_MEMBER(getBody());
}

IMPL_CLASS(RangeForStmt) {
    HAS_MEMBER(getIteratorType());
    HAS_MEMBER(getRange());
    HAS_MEMBER(getBody());
}

IMPL_CLASS(ReturnStmt) {
    HAS_MEMBER(getExp());
}

IMPL_CLASS(ScopeStmt) {
    HAS_MEMBER(getBody());
}

IMPL_CLASS(VarDeclStmt) {
    HAS_MEMBER(getType());
    HAS_MEMBER(getExp());
}

IMPL_CLASS(AssignStmt) {
    HAS_MEMBER(getLValue());
    HAS_MEMBER(getExp());
}

IMPL_CLASS(BooleanType) {
}

IMPL_CLASS(FunType) {
    HAS_MEMBER(getReturnType());
    HAS_MEMBER(getArgs());
}

IMPL_CLASS(IdType) {
}

IMPL_CLASS(IntegerType) {
}

IMPL_CLASS(ArrType) {
    HAS_MEMBER(getElemType());
}

IMPL_CLASS(RangeType) {
}

IMPL_CLASS(RecordType) {
    HAS_MEMBER(getFields());
}

IMPL_CLASS(PtrType) {
    HAS_MEMBER(getToType());
}

IMPL_CLASS(VoidType) {
}

