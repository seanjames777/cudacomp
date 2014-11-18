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

Type *typecheck_exp(TypeCtx *ctx, idset & decl, idset & def, ASTExpNode *node) {
    Type *type = NULL;

    Type *int32 = Type::getInt32Ty(ctx->getContext());

    // Integer constant
    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node)) {
        type = int32;
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
        type = ctx->getSymbol(id_exp->getId());
    }
    // Binary operator
    else if (ASTBinop *binop_exp = dynamic_cast<ASTBinop *>(node)) {
        // Get operand types
        Type *t1 = typecheck_exp(ctx, decl, def, binop_exp->getE1());
        Type *t2 = typecheck_exp(ctx, decl, def, binop_exp->getE1());

        // Types must be appropriate for operation
        switch(binop_exp->getOp()) {
        case ASTBinop::ADD:
        case ASTBinop::SUB:
        case ASTBinop::MUL:
        case ASTBinop::DIV:
            if (t1 != int32 || t2 != int32)
                throw new IllegalTypeException();
            type = int32;
        }
    }

    // Store the type for code generation
    ctx->setType(node, type);

    return type;
}

Type *convert_type(TypeCtx *ctx, ASTType *type) {
    if (ASTIntegerType *int_type = dynamic_cast<ASTIntegerType *>(type))
        return Type::getInt32Ty(ctx->getContext());

    throw new ASTMalformedException();
}

void typecheck_stmt(TypeCtx *ctx, idset & decl, idset & def, ASTStmtNode *node) {
    // Sequence node
    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        ASTStmtNode *head = seq_node->getHead();

        // If the first node is a variable declaration, we need to declare and
        // possibly define it in the rest of the code. Need to check the type
        // before marking as declared, in case the definition tries to be
        // recursive.
        if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(head)) {
            Type *decl_type = convert_type(ctx, decl_stmt->getType());

            // Must not be declared yet
            if (decl.find(decl_stmt->getId()) != decl.end())
                throw new RedeclaredException();

            ASTExpNode *decl_exp = decl_stmt->getExp();

            idset extdef = def;

            // If there is a definition, check the type and mark as defined
            if (decl_exp) {
                Type *exp_type = typecheck_exp(ctx, decl, def, decl_exp);

                if (exp_type != decl_type)
                    throw new IllegalTypeException();

                extdef.insert(decl_stmt->getId());
            }

            // Mark as declared, store the type
            idset extdecl = decl;
            extdecl.insert(decl_stmt->getId());
            ctx->setSymbol(decl_stmt->getId(), decl_type); // TODO createSymbol

            // Now check the rest of the function
            if (seq_node->getTail())
                typecheck_stmt(ctx, extdecl, extdef, seq_node->getTail());
        }
        // Variable assignment. Mark as defined and check the rest of the code
        else if (ASTVarDefnStmt *defn_stmt = dynamic_cast<ASTVarDefnStmt *>(head)) {
            // Must be declared
            if (decl.find(defn_stmt->getId()) == decl.end())
                throw new UndeclaredException();

            Type *decl_type = ctx->getSymbol(defn_stmt->getId());
            Type *exp_type = typecheck_exp(ctx, decl, def, defn_stmt->getExp());

            if (exp_type != decl_type)
                throw new IllegalTypeException();

            idset extdef = def;
            extdef.insert(defn_stmt->getId());

            // Now check the rest of the function
            if (seq_node->getTail())
                typecheck_stmt(ctx, decl, extdef, seq_node->getTail());
        }
        // Otherwise just typecheck each in turn
        else {
            typecheck_stmt(ctx, decl, def, seq_node->getHead());

            if (seq_node->getTail())
                typecheck_stmt(ctx, decl, def, seq_node->getTail());
        }
    }
    // Return statement
    else if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(node)) {
        Type *expected = Type::getInt32Ty(ctx->getContext());
        Type *exp_type = typecheck_exp(ctx, decl, def, ret_node->getExp());

        if (exp_type != expected)
            throw new IllegalTypeException();
    }
    // Illegal
    else {
        throw new ASTMalformedException();
    }
}

};
