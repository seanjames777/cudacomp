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
#include <ast/top/asttypedefntop.h>

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

std::shared_ptr<ASTTypeNode> typecheck_exp(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    idset & decl,
    idset & def,
    std::shared_ptr<ASTExpNode> node)
{
    // Integer constant
    if (std::shared_ptr<ASTIntegerExp> int_exp = std::dynamic_pointer_cast<ASTIntegerExp>(node))
        return ASTIntegerType::get();
    else if (std::shared_ptr<ASTBooleanExp> bool_exp = std::dynamic_pointer_cast<ASTBooleanExp>(node))
        return ASTBooleanType::get();
    // Variable reference
    else if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(node)) {
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
    else if (std::shared_ptr<ASTUnopExp> unop_exp = std::dynamic_pointer_cast<ASTUnopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> t = typecheck_exp(mod, func, decl, def, unop_exp->getExp());

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
    else if (std::shared_ptr<ASTBinopExp> binop_exp = std::dynamic_pointer_cast<ASTBinopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> t1 = typecheck_exp(mod, func, decl, def, binop_exp->getE1());
        std::shared_ptr<ASTTypeNode> t2 = typecheck_exp(mod, func, decl, def, binop_exp->getE1());

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
    else if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        std::shared_ptr<FunctionInfo> call_func = mod->getFunction(call_exp->getId());

        // Function must have been declared
        if (!call_func)
            throw new UndeclaredException(call_exp->getId());

        // Function must not have been shadowed by a variable. This leaves room for function
        // pointers.
        if (func->hasLocal(call_exp->getId()))
            throw new IllegalTypeException(); // TODO

        // TODO: make sure it is eventually defined
        // TODO: make sure any other declarations/definitions match

        std::shared_ptr<ASTFunType> sig = call_func->getSignature();

        // Check the expressions against the signature
        std::shared_ptr<ASTExpSeqNode> exprs = call_exp->getArgs();
        std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();

        while (true) {
            // Make sure the argument counts match
            if ((args == nullptr || exprs == nullptr) && (args != nullptr || exprs != nullptr))
                throw new IllegalTypeException(); // TODO maybe a different exception
            else if (args == nullptr && exprs == nullptr)
                break;

            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, call_func, decl, def, exprs->getHead());
            std::shared_ptr<ASTTypeNode> arg_type = args->getHead()->getType();

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

void typecheck_stmts(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    idset & decl,
    idset & def,
    std::shared_ptr<ASTStmtSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_stmt(mod, func, decl, def, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_stmt(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    idset & decl,
    idset & def,
    std::shared_ptr<ASTStmtNode> head)
{
    // If the first node is a variable declaration, we need to declare and
    // possibly define it in the rest of the code. Need to check the type
    // before marking as declared, in case the definition tries to be
    // recursive.
    if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head)) {
        std::shared_ptr<ASTTypeNode> decl_type = decl_stmt->getType();

        // TODO: test for void declaration

        // Must not be declared yet
        if (decl.find(decl_stmt->getId()) != decl.end())
            throw new RedeclaredException(decl_stmt->getId());

        std::shared_ptr<ASTExpNode> decl_exp = decl_stmt->getExp();

        // If there is a definition, check the type and mark as defined
        if (decl_exp) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, decl, def, decl_exp);

            if (!exp_type->equal(decl_type))
                throw new IllegalTypeException();

            def.insert(decl_stmt->getId());
        }

        // Mark as declared, store the type
        decl.insert(decl_stmt->getId());
        func->addLocal(decl_stmt->getId(), decl_type); // TODO declareSymbol
    }
    // Variable assignment. Mark as defined and check the rest of the code
    else if (std::shared_ptr<ASTVarDefnStmt> defn_stmt = std::dynamic_pointer_cast<ASTVarDefnStmt>(head)) {
        // Must be declared
        if (decl.find(defn_stmt->getId()) == decl.end())
            throw new UndeclaredException(defn_stmt->getId());

        std::shared_ptr<ASTTypeNode> decl_type = func->getLocalType(defn_stmt->getId());
        std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, decl, def, defn_stmt->getExp());

        if (!exp_type->equal(decl_type))
            throw new IllegalTypeException();

        if (def.find(defn_stmt->getId()) == def.end())
            def.insert(defn_stmt->getId());
    }
    // Return statement
    else if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        std::shared_ptr<ASTTypeNode> expected = func->getSignature()->getReturnType();

        bool isVoid = expected->equal(ASTVoidType::get());

        // Should not return anything in a void function
        if (isVoid && ret_node->getExp())
            throw new IllegalTypeException(); // TODO better exception

        // Must return something in a non-void function
        if (!ret_node->getExp() && !isVoid)
            throw new IllegalTypeException(); // TODO better exception

        // Must return the correct type in a non-void function
        if (ret_node->getExp()) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, decl, def, ret_node->getExp());

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
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
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
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, decl, def, if_node->getCond());

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
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head))
        typecheck_exp(mod, func, decl, def, exp_stmt->getExp());
    else
        throw new ASTMalformedException();
}

void typecheck_tops(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTTopSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_top(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_top(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTTopNode> node)
{
    if (std::shared_ptr<ASTFunDefnTop> funDefn = std::dynamic_pointer_cast<ASTFunDefnTop>(node)) {
        // Allocate space for information about this function
        std::shared_ptr<FunctionInfo> funInfo = std::make_shared<FunctionInfo>(funDefn->getName(), funDefn->getSignature());
        mod->addFunction(funInfo);

        idset decl, def;

        // Add all arguments to the local symbol table and mark them as declared
        // and defined.
        std::shared_ptr<ASTArgSeqNode> args = funDefn->getSignature()->getArgs();

        while (args != nullptr) {
            std::shared_ptr<ASTArgNode> arg = args->getHead();

            decl.insert(arg->getName());
            def.insert(arg->getName());

            args = args->getTail();
        }

        // Check the function body, building the local symbol table in the process
        typecheck_stmts(mod, funInfo, decl, def, funDefn->getBody());
    }
    else if (std::shared_ptr<ASTTypeDefnTop> typeDefn = std::dynamic_pointer_cast<ASTTypeDefnTop>(node)) {
        // Skip it
    }
    else
        throw new ASTMalformedException();
}

};
