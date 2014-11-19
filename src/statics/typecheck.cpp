/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>
#include <ast/astinteger.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>
#include <ast/astidentifier.h>
#include <ast/astvardeclstmt.h>
#include <ast/astvardefnstmt.h>
#include <ast/astintegertype.h>
#include <ast/astunop.h>
#include <ast/astbooleantype.h>

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
    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node)) {
        type = ASTIntegerType::get();
    }
    // Variable reference
    else if (ASTIdentifier *id_exp = dynamic_cast<ASTIdentifier *>(node)) {
        // Must be declared
        if (decl.find(id_exp->getId()) == decl.end())
            throw new UndeclaredException();

        // Must be defined
        if (def.find(id_exp->getId()) == def.end())
            throw new UndefinedException();

        // Just look up type
        type = ctx->getSymbolAST(id_exp->getId());
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
            type = t1;
            break;
        case ASTBinop::OR:
        case ASTBinop::AND:
            if (!t1->equal(ASTBooleanType::get()) || !t2->equal(ASTBooleanType::get()))
                throw new IllegalTypeException();
            type = t1;
            break;
        }
    }

    // Store the type for code generation
    ctx->setType(node, ctx->convert_type(type));

    return type;
}

void typecheck_stmt(TypeCtx *ctx, idset & decl, idset & def, ASTStmtNode *node) {
    // Sequence node
    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        // We always have a head
        ASTStmtNode *head = seq_node->getHead();

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

            ASTType *decl_type = ctx->getSymbolAST(defn_stmt->getId());
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

        // Typecheck the rest of the list
        if (seq_node->getTail())
            typecheck_stmt(ctx, decl, def, seq_node->getTail());
    }
    // Illegal. The AST is always a linked list of statements.
    else {
        throw new ASTMalformedException();
    }
}

};