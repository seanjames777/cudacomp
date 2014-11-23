/**
 * @file astvisitor.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astvisitor.h>

VISIT_IMPL(DeclNode, node) {
    if (NODE_MATCH(FunDecl, funDeclNode, node)) {
        if (!VISIT_CALL(FunDecl, funDeclNode))
            return false;
    }
    else if (NODE_MATCH(TypeDecl, typeDeclNode, node)) {
        if (!VISIT_CALL(TypeDecl, typeDeclNode))
            return false;
    }

    return true;
}

VISIT_IMPL(DeclSeqNode, node) {
    if (!VISIT_CALL(DeclNode, node->getHead()))
        return false;

    if (node->getTail() && !VISIT_CALL(DeclSeqNode, node->getTail()))
        return false;

    return true;
}

VISIT_IMPL(FunDecl, node) {
    if (!VISIT_CALL(FunType, node->getSignature()))
        return false;

    if (!VISIT_CALL(StmtSeqNode, node->getBody()))
        return false;

    return true;
}

VISIT_IMPL(TypeDecl, node) {
    if (NODE_MATCH(BooleanType, booleanTypeNode, node)) {
        if (!VISIT_CALL(BooleanType, booleanTypeNode))
            return false;
    }
    else if (NODE_MATCH(IntegerType, integerTypeNode, node)) {
        if (!VISIT_CALL(IntegerType, integerTypeNode))
            return false;
    }
    else if (NODE_MATCH(VoidType, voidTypeNode, node)) {
        if (!VISIT_CALL(VoidType, voidTypeNode))
            return false;
    }
    else if (NODE_MATCH(IdType, idTypeNode, node)) {
        if (!VISIT_CALL(IdType, idTypeNode))
            return false;
    }
    else if (NODE_MATCH(PtrType, ptrTypeNode, node)) {
        if (!VISIT_CALL(PtrType, ptrTypeNode))
            return false;
    }
    else if (NODE_MATCH(FunType, funTypeNode, node)) {
        if (!VISIT_CALL(FunType, funTypeNode))
            return false;
    }

    return true;
}

VISIT_IMPL(ExpNode, node) {
    if (NODE_MATCH(BinopExp, binopExpNode, node)) {
        if (!VISIT_CALL(BinopExp, binopExpNode))
            return false;
    }
    else if (NODE_MATCH(BooleanExp, booleanExpNode, node)) {
        if (!VISIT_CALL(BooleanExp, booleanExpNode))
            return false;
    }
    else if (NODE_MATCH(CallExp, callExpNode, node)) {
        if (!VISIT_CALL(CallExp, callExpNode))
            return false;
    }
    else if (NODE_MATCH(IdentifierExp, identifierExpNode, node)) {
        if (!VISIT_CALL(IdentifierExp, identifierExpNode))
            return false;
    }
    else if (NODE_MATCH(IntegerExp, integerExpNode, node)) {
        if (!VISIT_CALL(IntegerExp, integerExpNode))
            return false;
    }
    else if (NODE_MATCH(UnopExp, unopExpNode, node)) {
        if (!VISIT_CALL(UnopExp, unopExpNode))
            return false;
    }

    return true;
}

VISIT_IMPL(ExpSeqNode, node) {
    if (!VISIT_CALL(ExpNode, node->getHead()))
        return false;

    if (node->getTail() && !VISIT_CALL(ExpSeqNode, node->getTail()))
        return false;

    return true;
}

VISIT_IMPL(BinopExp, node) {

    return true;
}

VISIT_IMPL(BooleanExp, node) {
    return true;
}

VISIT_IMPL(CallExp, node) {
    return true;
}

VISIT_IMPL(IdentifierExp, node) {
    return true;
}

VISIT_IMPL(UnopExp, node) {
    return true;
}

VISIT_IMPL(IntegerExp, node) {
    return true;
}

VISIT_IMPL(StmtNode, node) {
    return true;
}

VISIT_IMPL(StmtSeqNode, node) {
    return true;
}

VISIT_IMPL(ExprStmt, node) {
    return true;
}

VISIT_IMPL(IfStmt, node) {
    return true;
}

VISIT_IMPL(ReturnStmt, node) {
    return true;
}

VISIT_IMPL(ScopeStmt, node) {
    return true;
}

VISIT_IMPL(VarDeclStmt, node) {
    if (!VISIT_CALL(TypeNode, node->getType()))
        return false;

    if (node->getExp() && !VISIT_CALL(ExpNode, node->getExp()))
        return false;

    return true;
}

VISIT_IMPL(VarDefnStmt, node) {
    if (!VISIT_CALL(ExpNode, node->getExp()))
        return false;

    return true;
}

VISIT_IMPL(ArgNode, node) {
    return true;
}

VISIT_IMPL(ArgSeqNode, node) {
    return true;
}

VISIT_IMPL(TypeNode, node) {
    return true;
}

VISIT_IMPL(BooleanType, node) {
    return true;
}

VISIT_IMPL(FunType, node) {
    return true;
}

VISIT_IMPL(IdType, node) {
    return true;
}

VISIT_IMPL(IntegerType, node) {
    return true;
}

VISIT_IMPL(PtrType, node) {
    return true;
}

VISIT_IMPL(VoidType, node) {
    return true;
}

