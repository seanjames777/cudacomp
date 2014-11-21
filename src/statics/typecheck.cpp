/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astbinopexp.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/type/astintegertype.h>
#include <ast/expr/astunopexp.h>
#include <ast/type/astbooleantype.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/top/astfundefntop.h>
#include <ast/expr/astcallexp.h>
#include <ast/type/astvoidtype.h>
#include <ast/type/astptrtype.h>
#include <ast/stmt/astexprstmt.h>

namespace Statics {

UndefinedException::UndefinedException(std::string id) {
    std::stringstream ss;
    ss << "Undefined symbol '" << id << "'";
    msg = ss.str();
}

UndeclaredException::UndeclaredException(std::string id) {
    std::stringstream ss;
    ss << "Undeclared symbol '" << id << "'";
    msg = ss.str();
}

RedeclaredException::RedeclaredException(std::string id) {
    std::stringstream ss;
    ss << "Variable '" << id << "' already declared";
    msg = ss.str();
}

IllegalTypeException::IllegalTypeException() {
    msg = "Illegal type";
}

ASTTypeNode *typecheck_exp(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTExpNode *node) {
    // Integer constant
    if (ASTIntegerExp *int_exp = dynamic_cast<ASTIntegerExp *>(node))
        return ASTIntegerType::get();
    else if (ASTBooleanExp *bool_exp = dynamic_cast<ASTBooleanExp *>(node))
        return ASTBooleanType::get();
    // Variable reference
    else if (ASTIdentifierExp *id_exp = dynamic_cast<ASTIdentifierExp *>(node)) {
        // Must be declared
        if (decl.find(id_exp->getId()) == decl.end())
            throw new UndeclaredException(id_exp->getId());

        // Must be defined
        if (def.find(id_exp->getId()) == def.end())
            throw new UndefinedException(id_exp->getId());

        // Just look up type
        return func->getLocalType(id_exp->getId());
    }
    // Unary operator
    else if (ASTUnopExp *unop_exp = dynamic_cast<ASTUnopExp *>(node)) {
        // Get operand types
        ASTTypeNode *t = typecheck_exp(mod, func, decl, def, unop_exp->getExp());

        // Types must be appropriate for operation
        switch(unop_exp->getOp()) {
        case ASTUnopExp::NOT:
            if (!t->equal(ASTBooleanType::get()))
                throw new IllegalTypeException();
            return t;
            break;
        case ASTUnopExp::BNOT:
        case ASTUnopExp::NEG:
            if (!t->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            return t;
            break;
        }
    }
    // Binary operator
    else if (ASTBinopExp *binop_exp = dynamic_cast<ASTBinopExp *>(node)) {
        // Get operand types
        ASTTypeNode *t1 = typecheck_exp(mod, func, decl, def, binop_exp->getE1());
        ASTTypeNode *t2 = typecheck_exp(mod, func, decl, def, binop_exp->getE1());

        // Types must be appropriate for operation
        switch(binop_exp->getOp()) {
        case ASTBinopExp::ADD:
        case ASTBinopExp::SUB:
        case ASTBinopExp::MUL:
        case ASTBinopExp::DIV:
        case ASTBinopExp::MOD:
        case ASTBinopExp::SHL:
        case ASTBinopExp::SHR:
        case ASTBinopExp::BAND:
        case ASTBinopExp::BOR:
        case ASTBinopExp::BXOR:
            if (!t1->equal(ASTIntegerType::get()) || !t2->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            return ASTIntegerType::get();
        case ASTBinopExp::OR:
        case ASTBinopExp::AND:
            if (!t1->equal(ASTBooleanType::get()) || !t2->equal(ASTBooleanType::get()))
                throw new IllegalTypeException();
            return ASTBooleanType::get();
        case ASTBinopExp::LEQ:
        case ASTBinopExp::GEQ:
        case ASTBinopExp::LT:
        case ASTBinopExp::GT:
            if (!t1->equal(ASTIntegerType::get()) || !t2->equal(ASTIntegerType::get()))
                throw new IllegalTypeException();
            return ASTBooleanType::get();
        case ASTBinopExp::EQ:
        case ASTBinopExp::NEQ:
            if (!t1->equal(t2) || (!t1->equal(ASTIntegerType::get()) && !t1->equal(ASTBooleanType::get())))
                throw new IllegalTypeException();
            return ASTBooleanType::get();
        }
    }
    else if (ASTCallExp *call_exp = dynamic_cast<ASTCallExp *>(node)) {
        FunctionInfo *call_func = mod->getFunction(call_exp->getId());

        // Function must have been declared
        if (!call_func)
            throw new UndeclaredException(call_exp->getId());

        // Function must not have been shadowed by a variable. This leaves room for function
        // pointers.
        if (func->hasLocal(call_exp->getId()))
            throw new IllegalTypeException(); // TODO

        // TODO: make sure it is eventually defined
        // TODO: make sure any other declarations/definitions match

        ASTFunType *sig = call_func->getSignature();

        // Check the expressions against the signature
        ASTExpSeqNode *exprs = call_exp->getArgs();
        ASTArgSeqNode *args = sig->getArgs();

        while (true) {
            // Make sure the argument counts match
            if ((args == NULL || exprs == NULL) && (args != NULL || exprs != NULL))
                throw new IllegalTypeException(); // TODO maybe a different exception
            else if (args == NULL && exprs == NULL)
                break;

            ASTTypeNode *exp_type = typecheck_exp(mod, call_func, decl, def, exprs->getHead());
            ASTTypeNode *arg_type = args->getHead()->getType();

            // TODO: test for void argument

            if (!exp_type->equal(arg_type))
                throw new IllegalTypeException();

            args = args->getTail();
            exprs = exprs->getTail();
        }

        // TODO: test argument length mismatch, type mismatch, return type mismatch
        // TODO: actually check that stuff here

        return sig->getReturnType();
    }
    else
        throw new ASTMalformedException();
}

void typecheck_stmts(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtSeqNode *seq_node) {
    while (seq_node != NULL) {
        typecheck_stmt(mod, func, decl, def, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_stmt(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtNode *head) {
    // If the first node is a variable declaration, we need to declare and
    // possibly define it in the rest of the code. Need to check the type
    // before marking as declared, in case the definition tries to be
    // recursive.
    if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(head)) {
        ASTTypeNode *decl_type = decl_stmt->getType();

        // TODO: test for void declaration

        // Must not be declared yet
        if (decl.find(decl_stmt->getId()) != decl.end())
            throw new RedeclaredException(decl_stmt->getId());

        ASTExpNode *decl_exp = decl_stmt->getExp();

        // If there is a definition, check the type and mark as defined
        if (decl_exp) {
            ASTTypeNode *exp_type = typecheck_exp(mod, func, decl, def, decl_exp);

            if (!exp_type->equal(decl_type))
                throw new IllegalTypeException();

            def.insert(decl_stmt->getId());
        }

        // Mark as declared, store the type
        decl.insert(decl_stmt->getId());
        func->addLocal(decl_stmt->getId(), decl_type); // TODO declareSymbol
    }
    // Variable assignment. Mark as defined and check the rest of the code
    else if (ASTVarDefnStmt *defn_stmt = dynamic_cast<ASTVarDefnStmt *>(head)) {
        // Must be declared
        if (decl.find(defn_stmt->getId()) == decl.end())
            throw new UndeclaredException(defn_stmt->getId());

        ASTTypeNode *decl_type = func->getLocalType(defn_stmt->getId());
        ASTTypeNode *exp_type = typecheck_exp(mod, func, decl, def, defn_stmt->getExp());

        if (!exp_type->equal(decl_type))
            throw new IllegalTypeException();

        if (def.find(defn_stmt->getId()) == def.end())
            def.insert(defn_stmt->getId());
    }
    // Return statement
    else if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head)) {
        ASTTypeNode *expected = func->getSignature()->getReturnType();

        bool isVoid = expected->equal(ASTVoidType::get());

        // Should not return anything in a void function
        if (isVoid && ret_node->getExp())
            throw new IllegalTypeException(); // TODO better exception

        // Must return something in a non-void function
        if (!ret_node->getExp() && !isVoid)
            throw new IllegalTypeException(); // TODO better exception

        // Must return the correct type in a non-void function
        if (ret_node->getExp()) {
            ASTTypeNode *exp_type = typecheck_exp(mod, func, decl, def, ret_node->getExp());

            if (!exp_type->equal(expected))
                throw new IllegalTypeException();
        }

        // Return statements need to define any variables that have been
        // declared. Every control flow path from a declaration of a
        // variable to its use must contain a definition, and there is no
        // control flow across a return statement.
        def = decl;
    }
    // Scope statement
    else if (ASTScopeStmt *scope_node = dynamic_cast<ASTScopeStmt *>(head)) {
        if (scope_node->getBody()) {
            // Scope inherits outside definitions/declarations
            idset scope_decl = decl;
            idset scope_def = def;

            typecheck_stmts(mod, func, scope_decl, scope_def, scope_node->getBody());

            // Definitions of variables that were declared outside the scope
            // propogate out
            idset new_def;

            std::set_intersection(scope_def.begin(), scope_def.end(),
                decl.begin(), decl.end(),
                std::inserter(new_def, new_def.end()));

            def = new_def;
        }
    }
    // If statement
    else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
        // Condition must be a boolean
        ASTTypeNode *cond_type = typecheck_exp(mod, func, decl, def, if_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw new IllegalTypeException();

        // Treat branches as scopes
        idset scope_decl_left = decl;
        idset scope_def_left = def;
        idset scope_decl_right = decl;
        idset scope_def_right = def;

        typecheck_stmts(mod, func, scope_decl_left, scope_def_left, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            typecheck_stmts(mod, func, scope_decl_right, scope_def_right, if_node->getFalseStmt());

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
    // Expression statement
    else if (ASTExprStmt *exp_stmt = dynamic_cast<ASTExprStmt *>(head))
        typecheck_exp(mod, func, decl, def, exp_stmt->getExp());
    else
        throw new ASTMalformedException();
}

void typecheck_tops(ModuleInfo *mod, ASTTopSeqNode *seq_node) {
    while (seq_node != NULL) {
        typecheck_top(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_top(ModuleInfo *mod, ASTTopNode *node) {
    if (ASTFunDefnTop *funDefn = dynamic_cast<ASTFunDefnTop *>(node)) {
        // Allocate space for information about this function
        FunctionInfo *funInfo = new FunctionInfo(funDefn->getName(), funDefn->getSignature());
        mod->addFunction(funDefn->getName(), funInfo);

        idset decl, def;

        // Add all arguments to the local symbol table and mark them as declared
        // and defined.
        ASTArgSeqNode *args = funDefn->getSignature()->getArgs();

        while (args != NULL) {
            ASTArg *arg = args->getHead();
            funInfo->addLocal(arg->getName(), arg->getType());
            args = args->getTail();

            decl.insert(arg->getName());
            def.insert(arg->getName());
        }

        // Check the function body, building the local symbol table in the process
        typecheck_stmts(mod, funInfo, decl, def, funDefn->getBody());
    }
    else
        throw new ASTMalformedException();
}

};
