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
    while (node != nullptr) {
        if (!VISIT_CALL(DeclNode, node->getHead()))
            return false;

        node = node->getTail();
    }

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
    else if (NODE_MATCH(ArrType, arrTypeNode, node)) {
        if (!VISIT_CALL(ArrType, arrTypeNode))
            return false;
    }
    else if (NODE_MATCH(FunType, funTypeNode, node)) {
        if (!VISIT_CALL(FunType, funTypeNode))
            return false;
    }

    return true;
}

VISIT_IMPL(ExpNode, node) {
    if (NODE_MATCH(AllocArrayExp, allocArrayExpNode, node)) {
        if (!VISIT_CALL(AllocArrayExp, allocArrayExpNode))
            return false;
    }
    else if (NODE_MATCH(BinopExp, binopExpNode, node)) {
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
    else if (NODE_MATCH(IndexExp, indexExpNode, node)) {
        if (!VISIT_CALL(IndexExp, indexExpNode))
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
    while (node != nullptr) {
        if (!VISIT_CALL(ExpNode, node->getHead()))
            return false;

        node = node->getTail();
    }

    return true;
}

VISIT_IMPL(BinopExp, node) {
    if (!VISIT_CALL(ExpNode, node->getE1()))
        return false;

    if (!VISIT_CALL(ExpNode, node->getE2()))
        return false;

    return true;
}

VISIT_IMPL(BooleanExp, node) {
    return true;
}

VISIT_IMPL(CallExp, node) {
    std::shared_ptr<ASTExpSeqNode> args = node->getArgs();

    while (args != nullptr) {
        if (!VISIT_CALL(ExpNode, args->getHead()))
            return false;

        args = args->getTail();
    }

    return true;
}

VISIT_IMPL(IndexExp, node) {
    if (!VISIT_CALL(ExpNode, node->getLValue()))
        return false;

    if (!VISIT_CALL(ExpNode, node->getSubscript()))
        return false;

    return true;
}

VISIT_IMPL(IdentifierExp, node) {
    return true;
}

VISIT_IMPL(UnopExp, node) {
    if (!VISIT_CALL(ExpNode, node->getExp()))
        return false;

    return true;
}

VISIT_IMPL(IntegerExp, node) {
    return true;
}

VISIT_IMPL(StmtNode, node) {
    if (NODE_MATCH(AssignStmt, assignStmtNode, node)) {
        if (!VISIT_CALL(AssignStmt, assignStmtNode))
            return false;
    }
    else if (NODE_MATCH(ExprStmt, exprStmtNode, node)) {
        if (!VISIT_CALL(ExprStmt, exprStmtNode))
            return false;
    }
    else if (NODE_MATCH(IfStmt, ifStmtNode, node)) {
        if (!VISIT_CALL(IfStmt, ifStmtNode))
            return false;
    }
    else if (NODE_MATCH(ReturnStmt, returnStmtNode, node)) {
        if (!VISIT_CALL(ReturnStmt, returnStmtNode))
            return false;
    }
    else if (NODE_MATCH(ScopeStmt, scopeStmtNode, node)) {
        if (!VISIT_CALL(ScopeStmt, scopeStmtNode))
            return false;
    }
    else if (NODE_MATCH(VarDeclStmt, varDeclStmt, node)) {
        if (!VISIT_CALL(VarDeclStmt, varDeclStmt))
            return false;
    }
    else if (NODE_MATCH(WhileStmt, whileStmtNode, node)) {
        if (!VISIT_CALL(WhileStmt, whileStmtNode))
            return false;
    }

    return true;
}

VISIT_IMPL(StmtSeqNode, node) {
    while (node != nullptr) {
        if (!VISIT_CALL(StmtNode, node->getHead()))
            return false;

        node = node->getTail();
    }

    return true;
}

VISIT_IMPL(ExprStmt, node) {
    if (!VISIT_CALL(ExpNode, node->getExp()))
        return false;

    return true;
}

VISIT_IMPL(IfStmt, node) {
    if (!VISIT_CALL(ExpNode, node->getCond()))
        return false;

    if (!VISIT_CALL(StmtSeqNode, node->getTrueStmt()))
        return false;

    if (node->getFalseStmt() && !VISIT_CALL(StmtSeqNode, node->getFalseStmt()))
        return false;

    return true;
}

VISIT_IMPL(WhileStmt, node) {
    if (!VISIT_CALL(ExpNode, node->getCond()))
        return false;

    if (!VISIT_CALL(StmtSeqNode, node->getBody()))
        return false;

    return true;
}

VISIT_IMPL(AllocArrayExp, node) {
    if (!VISIT_CALL(TypeNode, node->getElemType()))
        return false;

    if (!VISIT_CALL(ExpNode, node->getLength()))
        return false;

    return true;
}

VISIT_IMPL(ReturnStmt, node) {
    if (node->getExp() && !VISIT_CALL(ExpNode, node->getExp()))
        return false;

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

VISIT_IMPL(AssignStmt, node) {
    if (!VISIT_CALL(ExpNode, node->getExp()))
        return false;

    return true;
}

VISIT_IMPL(ArgNode, node) {
    if (!VISIT_CALL(TypeNode, node->getType()))
        return false;

    return true;
}

VISIT_IMPL(ArgSeqNode, node) {
    while (node != nullptr) {
        if (!VISIT_CALL(ArgNode, node->getHead()))
            return false;

        node = node->getTail();
    }

    return true;
}

VISIT_IMPL(TypeNode, node) {
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
    else if (NODE_MATCH(PtrType, ptrTypeNode, node)) {
        if (!VISIT_CALL(PtrType, ptrTypeNode))
            return false;
    }
    else if (NODE_MATCH(ArrType, arrTypeNode, node)) {
        if (!VISIT_CALL(ArrType, arrTypeNode))
            return false;
    }
    else if (NODE_MATCH(IdType, idTypeNode, node)) {
        if (!VISIT_CALL(IdType, idTypeNode))
            return false;
    }
    else if (NODE_MATCH(FunType, funTypeNode, node)) {
        if (!VISIT_CALL(FunType, funTypeNode))
            return false;
    }

    return true;
}

VISIT_IMPL(BooleanType, node) {
    return true;
}

VISIT_IMPL(FunType, node) {
    if (!VISIT_CALL(TypeNode, node->getReturnType()))
        return false;

    if (!VISIT_CALL(ArgSeqNode, node->getArgs()))
        return false;

    return true;
}

VISIT_IMPL(IdType, node) {
    return true;
}

VISIT_IMPL(IntegerType, node) {
    return true;
}

VISIT_IMPL(ArrType, node) {
    if (!VISIT_CALL(TypeNode, node->getElemType()))
        return false;

    return true;
}

VISIT_IMPL(PtrType, node) {
    if (!VISIT_CALL(TypeNode, node->getToType()))
        return false;

    return true;
}

VISIT_IMPL(VoidType, node) {
    return true;
}

