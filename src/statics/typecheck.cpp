/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>
#include <ast/expr/astinteger.h>
#include <ast/expr/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/expr/astidentifier.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/type/astintegertype.h>
#include <ast/expr/astunop.h>
#include <ast/type/astbooleantype.h>
#include <ast/stmt/astscope.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astboolean.h>

namespace Statics {

UndefinedException::UndefinedException()
    : runtime_error("Undefined variable")
{
}

UndeclaredException::UndeclaredException()
    : runtime_error("Undeclared variable")
{
}

RedeclaredException::RedeclaredException()
    : runtime_error("Redeclared variable")
{
}

IllegalTypeException::IllegalTypeException()
    : runtime_error("Illegal type")
{
}

ASTType *typecheck_exp(TypeCtx *ctx, idset & decl, idset & def, ASTExpNode *node) {
    ASTType *type;

    // Integer constant
    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node))
        type = ASTIntegerType::get();
    else if (ASTBoolean *bool_exp = dynamic_cast<ASTBoolean *>(node))
        type = ASTBooleanType::get();
    // Variable reference
    else if (ASTIdentifier *id_exp = dynamic_cast<ASTIdentifier *>(node)) {
        // Must be declared
        if (decl.find(id_exp->getId()) == decl.end())
            throw new UndeclaredException();

        // Must be defined
        if (def.find(id_exp->getId()) == def.end())
            throw new UndefinedException();

        // Just look up type
        type = ctx->getSymbol(id_exp->getId());
    }
    // Unary operator
    else if (ASTUnop *unop_exp = dynamic_cast<ASTUnop *>(node)) {
        // Get operand types
        ASTType *t = typecheck_exp(ctx, decl, def, unop_exp->getExp());

        // Types must be appropriate for operation
        switch(unop_exp->getOp()) {
        case ASTUnop::NOT:
            if (!t->equal(ASTBooleanType::get()))
                throw new IllegalTypeException();
            type = t;
            break;
        case ASTUnop::BNOT:
        case ASTUnop::NEG:
            if (!t->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            type = t;
            break;
        }
    }
    // Binary operator
    else if (ASTBinop *binop_exp = dynamic_cast<ASTBinop *>(node)) {
        // Get operand types
        ASTType *t1 = typecheck_exp(ctx, decl, def, binop_exp->getE1());
        ASTType *t2 = typecheck_exp(ctx, decl, def, binop_exp->getE1());

        // Types must be appropriate for operation
        switch(binop_exp->getOp()) {
        case ASTBinop::ADD:
        case ASTBinop::SUB:
        case ASTBinop::MUL:
        case ASTBinop::DIV:
        case ASTBinop::MOD:
        case ASTBinop::SHL:
        case ASTBinop::SHR:
        case ASTBinop::BAND:
        case ASTBinop::BOR:
        case ASTBinop::BXOR:
            if (!t1->equal(ASTIntegerType::get()) || !t2->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            type = ASTIntegerType::get();
            break;
        case ASTBinop::OR:
        case ASTBinop::AND:
            if (!t1->equal(ASTBooleanType::get()) || !t2->equal(ASTBooleanType::get()))
                throw new IllegalTypeException();
            type = ASTBooleanType::get();
            break;
        case ASTBinop::LEQ:
        case ASTBinop::GEQ:
        case ASTBinop::LT:
        case ASTBinop::GT:
            if (!t1->equal(ASTIntegerType::get()) || !t2->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            type = ASTBooleanType::get();
            break;
        case ASTBinop::EQ:
        case ASTBinop::NEQ:
            if (!t1->equal(t2) || (!t1->equal(ASTIntegerType::get()) && !t1->equal(ASTBooleanType::get())))
                throw new IllegalTypeException();
            type = ASTBooleanType::get();
            break;
        }
    }
    else
        throw new ASTMalformedException();

    return type;
}

void typecheck_stmts(TypeCtx *ctx, idset & decl, idset & def, ASTStmtSeqNode *seq_node) {
    while (seq_node != NULL) {
        typecheck_stmt(ctx, decl, def, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_stmt(TypeCtx *ctx, idset & decl, idset & def, ASTStmtNode *head) {
    // If the first node is a variable declaration, we need to declare and
    // possibly define it in the rest of the code. Need to check the type
    // before marking as declared, in case the definition tries to be
    // recursive.
    if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(head)) {
        ASTType *decl_type = decl_stmt->getType();

        // Must not be declared yet
        if (decl.find(decl_stmt->getId()) != decl.end())
            throw new RedeclaredException();

        ASTExpNode *decl_exp = decl_stmt->getExp();

        // If there is a definition, check the type and mark as defined
        if (decl_exp) {
            ASTType *exp_type = typecheck_exp(ctx, decl, def, decl_exp);

            if (!exp_type->equal(decl_type))
                throw new IllegalTypeException();

            def.insert(decl_stmt->getId());
        }

        // Mark as declared, store the type
        decl.insert(decl_stmt->getId());
        ctx->setSymbol(decl_stmt->getId(), decl_type); // TODO declareSymbol
    }
    // Variable assignment. Mark as defined and check the rest of the code
    else if (ASTVarDefnStmt *defn_stmt = dynamic_cast<ASTVarDefnStmt *>(head)) {
        // Must be declared
        if (decl.find(defn_stmt->getId()) == decl.end())
            throw new UndeclaredException();

        ASTType *decl_type = ctx->getSymbol(defn_stmt->getId());
        ASTType *exp_type = typecheck_exp(ctx, decl, def, defn_stmt->getExp());

        if (!exp_type->equal(decl_type))
            throw new IllegalTypeException();

        if (def.find(defn_stmt->getId()) == def.end())
            def.insert(defn_stmt->getId());
    }
    // Return statement
    else if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head)) {
        ASTType *expected = ASTIntegerType::get();
        ASTType *exp_type = typecheck_exp(ctx, decl, def, ret_node->getExp());

        if (!exp_type->equal(expected))
            throw new IllegalTypeException();

        // Return statements need to define any variables that have been
        // declared. Every control flow path from a declaration of a
        // variable to its use must contain a definition, and there is no
        // control flow across a return statement.
        def = decl;
    }
    // Scope statement
    else if (ASTScope *scope_node = dynamic_cast<ASTScope *>(head)) {
        if (scope_node->getBody()) {
            // Scope inherits outside definitions/declarations
            idset scope_decl = decl;
            idset scope_def = def;

            typecheck_stmts(ctx, scope_decl, scope_def, scope_node->getBody());

            // Definitions of variables that were declared outside the scope
            // propogate out
            idset new_def;

            std::set_intersection(scope_def.begin(), scope_def.end(),
                decl.begin(), decl.end(),
                std::inserter(new_def, new_def.end()));

            def = new_def;
        }
    }
    else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
        // Condition must be a boolean
        ASTType *cond_type = typecheck_exp(ctx, decl, def, if_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw new IllegalTypeException();

        // Treat branches as scopes
        idset scope_decl_left = decl;
        idset scope_def_left = def;
        idset scope_decl_right = decl;
        idset scope_def_right = def;

        typecheck_stmts(ctx, scope_decl_left, scope_def_left, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            typecheck_stmts(ctx, scope_decl_right, scope_def_right, if_node->getFalseStmt());

        // Definitions of variables that were declared outside the if statement
        // and defined by BOTH branches propogate out
        idset both_def;
        idset new_def;

        std::set_intersection(
            scope_def_left.begin(), scope_def_left.end(),
            scope_def_right.begin(), scope_def_right.end(),
            std::inserter(both_def, both_def.end()));

        std::set_intersection(both_def.begin(), both_def.end(),
            decl.begin(), decl.end(),
            std::inserter(new_def, new_def.end()));

        def = new_def;
    }
    else
        throw new ASTMalformedException();
}

};
