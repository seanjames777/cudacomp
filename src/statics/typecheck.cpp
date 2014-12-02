/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>

namespace Statics {

std::shared_ptr<ASTTypeNode> typecheck_exp(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    std::shared_ptr<ASTExpNode> node)
{
    // Integer constant
    if (std::shared_ptr<ASTIntegerExp> int_exp = std::dynamic_pointer_cast<ASTIntegerExp>(node))
        return ASTIntegerType::get();
    // Boolean constant
    else if (std::shared_ptr<ASTBooleanExp> bool_exp = std::dynamic_pointer_cast<ASTBooleanExp>(node))
        return ASTBooleanType::get();
    else if (std::shared_ptr<ASTFloatExp> float_exp = std::dynamic_pointer_cast<ASTFloatExp>(node))
        return ASTFloatType::get();
    // Variable reference
    else if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(node)) {
        std::shared_ptr<ASTTypeNode> type = func->getLocalType(id_exp->getId());
        assert(type);

        return type;
    }
    // Array subscript
    else if (std::shared_ptr<ASTIndexExp> idx_exp = std::dynamic_pointer_cast<ASTIndexExp>(node)) {
        std::shared_ptr<ASTTypeNode> lhs_type = typecheck_exp(mod, func, idx_exp->getLValue());
        std::shared_ptr<ASTTypeNode> sub_type = typecheck_exp(mod, func, idx_exp->getSubscript());

        // TODO test the following cases:

        // Right hand side must be an integer
        if (!sub_type->equal(ASTIntegerType::get()))
            throw IllegalTypeException();

        // Left hand side must be an array
        if (std::shared_ptr<ASTArrType> lhs_arr = std::dynamic_pointer_cast<ASTArrType>(lhs_type))
            return lhs_arr->getElemType();
        else
            throw IllegalTypeException();
    }
    // Unary operator
    else if (std::shared_ptr<ASTUnopExp> unop_exp = std::dynamic_pointer_cast<ASTUnopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> t = typecheck_exp(mod, func, unop_exp->getExp());

        // Types must be appropriate for operation
        switch(unop_exp->getOp()) {
        case ASTUnopExp::NOT:
            if (!t->equal(ASTBooleanType::get()))
                throw IllegalTypeException();
            return t;
            break;
        case ASTUnopExp::BNOT:
        case ASTUnopExp::NEG:
            if (!t->equal(ASTIntegerType::get()))
                throw IllegalTypeException();
            return t;
            break;
        }
    }
    // Binary operator
    else if (std::shared_ptr<ASTBinopExp> binop_exp = std::dynamic_pointer_cast<ASTBinopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> t1 = typecheck_exp(mod, func, binop_exp->getE1());
        std::shared_ptr<ASTTypeNode> t2 = typecheck_exp(mod, func, binop_exp->getE2());

        // Types must be appropriate for operation
        switch(binop_exp->getOp()) {
        case ASTBinopExp::ADD:
        case ASTBinopExp::SUB:
        case ASTBinopExp::MUL:
        case ASTBinopExp::DIV:
        case ASTBinopExp::MOD:
            if (!t1->equal(t2))
                throw IllegalTypeException();

            if (t1->equal(ASTIntegerType::get())) {
                binop_exp->setType(ASTIntegerType::get());
                return ASTIntegerType::get();
            }

            if (t1->equal(ASTFloatType::get())) {
                binop_exp->setType(ASTFloatType::get());
                return ASTFloatType::get();
            }

            throw IllegalTypeException();
        case ASTBinopExp::SHL:
        case ASTBinopExp::SHR:
        case ASTBinopExp::BAND:
        case ASTBinopExp::BOR:
        case ASTBinopExp::BXOR:
            if (!t1->equal(t2))
                throw IllegalTypeException();

            if (t1->equal(ASTIntegerType::get())) {
                binop_exp->setType(ASTIntegerType::get());
                return ASTIntegerType::get();
            }

            throw IllegalTypeException();
        case ASTBinopExp::OR:
        case ASTBinopExp::AND:
            if (!t1->equal(ASTBooleanType::get()) || !t2->equal(ASTBooleanType::get()))
                throw IllegalTypeException();

            binop_exp->setType(ASTBooleanType::get());
            return ASTBooleanType::get();
        case ASTBinopExp::LEQ:
        case ASTBinopExp::GEQ:
        case ASTBinopExp::LT:
        case ASTBinopExp::GT:
            if (!t1->equal(t2))
                throw IllegalTypeException();

            if (t1->equal(ASTIntegerType::get())) {
                binop_exp->setType(ASTIntegerType::get());
                return ASTBooleanType::get();
            }

            if (t1->equal(ASTFloatType::get())) {
                binop_exp->setType(ASTFloatType::get());
                return ASTBooleanType::get();
            }

            throw IllegalTypeException();
        case ASTBinopExp::NONE: throw ASTMalformedException(); return nullptr;
        case ASTBinopExp::EQ:
        case ASTBinopExp::NEQ:
            // Must have the same type
            if (!t1->equal(t2))
                throw IllegalTypeException();

            // Must be an 'equality type'
            std::shared_ptr<ASTArrType> arrType = std::dynamic_pointer_cast<ASTArrType>(t1);

            if (t1->equal(ASTIntegerType::get()) ||
                t1->equal(ASTBooleanType::get()) ||
                arrType)
            {
                // TODO could split out boolean type, etc.
                binop_exp->setType(ASTIntegerType::get());
                return ASTBooleanType::get();
            }

            if (t1->equal(ASTFloatType::get())) {
                binop_exp->setType(ASTFloatType::get());
                return ASTBooleanType::get();
            }

            throw IllegalTypeException();
            return nullptr;
        }
    }
    // Ternary operator
    else if (std::shared_ptr<ASTTernopExp> tern_exp = std::dynamic_pointer_cast<ASTTernopExp>(node)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> condType = typecheck_exp(mod, func, tern_exp->getCond());

        if (!condType->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        // Each side must have the same type
        std::shared_ptr<ASTTypeNode> leftType = typecheck_exp(mod, func, tern_exp->getTrueExp());
        std::shared_ptr<ASTTypeNode> rightType = typecheck_exp(mod, func, tern_exp->getFalseExp());

        if (!leftType->equal(rightType))
            throw IllegalTypeException();

        tern_exp->setType(leftType);

        return leftType;
    }
    // Function call
    else if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        // The function checker guarantees that this exists
        std::shared_ptr<FunctionInfo> call_func = mod->getFunction(call_exp->getId());

        // Function must not have been shadowed by a variable. This leaves room for function
        // pointers.
        if (func->hasLocal(call_exp->getId()))
            throw IllegalTypeException();

        std::shared_ptr<ASTFunType> sig = call_func->getSignature();

        // Check the expressions against the signature
        std::shared_ptr<ASTExpSeqNode> exprs = call_exp->getArgs();
        std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();

        while (true) {
            // Make sure the argument counts match
            if ((args == nullptr || exprs == nullptr) && (args != nullptr || exprs != nullptr))
                throw IllegalTypeException(); // TODO maybe a different exception
            else if (args == nullptr && exprs == nullptr)
                break;

            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, exprs->getHead());
            std::shared_ptr<ASTTypeNode> arg_type = args->getHead()->getType();

            // TODO: test for void argument
            // TODO: test for void alloc_array

            if (!exp_type->equal(arg_type))
                throw IllegalTypeException();

            args = args->getTail();
            exprs = exprs->getTail();
        }

        // TODO: test argument length mismatch, type mismatch, return type mismatch
        // TODO: actually check that stuff here

        return sig->getReturnType();
    }
    // Array allocation
    else if (std::shared_ptr<ASTAllocArrayExp> alloc_exp = std::dynamic_pointer_cast<ASTAllocArrayExp>(node)) {
        std::shared_ptr<ASTTypeNode> elemType = alloc_exp->getElemType();

        if (elemType->equal(ASTVoidType::get()))
            throw IllegalTypeException();

        // Size must be an integer
        std::shared_ptr<ASTTypeNode> sizeType = typecheck_exp(mod, func, alloc_exp->getLength());

        if (!sizeType->equal(ASTIntegerType::get()))
            throw IllegalTypeException();

        // Returns an array of elemTypes
        return std::make_shared<ASTArrType>(elemType);
    }
    else
        throw ASTMalformedException();
}

void typecheck_stmts(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    std::shared_ptr<ASTStmtSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_stmt(mod, func, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_stmt(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    std::shared_ptr<ASTStmtNode> head)
{
    // If the first node is a variable declaration, we need to declare and
    // possibly define it in the rest of the code. Need to check the type
    // before marking as declared, in case the definition tries to be
    // recursive.
    if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head)) {
        std::shared_ptr<ASTTypeNode> decl_type = decl_stmt->getType();

        if (decl_type->equal(ASTVoidType::get()))
            throw IllegalTypeException();

        std::shared_ptr<ASTExpNode> decl_exp = decl_stmt->getExp();

        // If there is a definition, check the type and mark as defined
        if (decl_exp) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, decl_exp);

            if (!exp_type->equal(decl_type))
                throw IllegalTypeException();
        }

        // Store the type
        func->addLocal(decl_stmt->getId(), decl_type); // TODO declareSymbol
    }
    // Assignment. Mark as defined and check the rest of the code
    else if (std::shared_ptr<ASTAssignStmt> defn_stmt = std::dynamic_pointer_cast<ASTAssignStmt>(head)) {
        // Simple variable
        if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(defn_stmt->getLValue())) {
            std::shared_ptr<ASTTypeNode> decl_type = func->getLocalType(id_exp->getId());
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, defn_stmt->getExp());

            // Must assign the same type
            if (!exp_type->equal(decl_type))
                throw IllegalTypeException();

            defn_stmt->setType(exp_type);
        }
        // Array subscript
        else if (std::shared_ptr<ASTIndexExp> idx_exp = std::dynamic_pointer_cast<ASTIndexExp>(defn_stmt->getLValue())) {
            std::shared_ptr<ASTTypeNode> lhs_type = typecheck_exp(mod, func, idx_exp);
            std::shared_ptr<ASTTypeNode> rhs_type = typecheck_exp(mod, func, defn_stmt->getExp());

            // Must assign the same type
            if (!lhs_type->equal(rhs_type))
                throw IllegalTypeException();

            defn_stmt->setType(rhs_type);
        }
        else throw IllegalLValueException();
    }
    // Return statement
    else if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        std::shared_ptr<ASTTypeNode> expected = func->getSignature()->getReturnType();

        bool isVoid = expected->equal(ASTVoidType::get());

        // Should not return anything in a void function
        if (isVoid && ret_node->getExp())
            throw IllegalTypeException(); // TODO better exception

        // Must return something in a non-void function
        if (!ret_node->getExp() && !isVoid)
            throw IllegalTypeException(); // TODO better exception

        // Must return the correct type in a non-void function
        if (ret_node->getExp()) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, ret_node->getExp());

            if (!exp_type->equal(expected))
                throw IllegalTypeException();
        }
    }
    // Scope statement
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        if (scope_node->getBody())
            typecheck_stmts(mod, func, scope_node->getBody());
    }
    // If statement
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, if_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        typecheck_stmts(mod, func, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            typecheck_stmts(mod, func, if_node->getFalseStmt());
    }
    // While statement
    else if (std::shared_ptr<ASTWhileStmt> while_node = std::dynamic_pointer_cast<ASTWhileStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, while_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        typecheck_stmts(mod, func, while_node->getBody());
    }
    // Expression statement
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head))
        typecheck_exp(mod, func, exp_stmt->getExp());
    else
        throw ASTMalformedException();
}

void typecheck_tops(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_top(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_top(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclNode> node)
{
    if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(node)) {
        // Skip empty declarations
        if (!funDefn->isDefn())
            return;

        // The function checker has already allocated FunctionInfo's for us
        std::shared_ptr<FunctionInfo> funInfo = mod->getFunction(funDefn->getName());

        // TODO: void args

        funInfo->copyArgumentsToLocals();

        // Check the function body, building the local symbol table in the process
        typecheck_stmts(mod, funInfo, funDefn->getBody());
    }
    // Skip
    else if (std::shared_ptr<ASTTypeDecl> typeDefn = std::dynamic_pointer_cast<ASTTypeDecl>(node))
        return;
    else
        throw ASTMalformedException();
}

};
