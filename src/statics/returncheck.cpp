/**
 * @file returncheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/returncheck.h>
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
#include <ast/type/astvoidtype.h>

namespace Statics {

NoReturnException::NoReturnException()
    : runtime_error("Missing return statement")
{
}

IllegalReturnTypeException::IllegalReturnTypeException()
    : runtime_error("Illegal return type")
{
}

bool returncheck_stmts(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtSeqNode> seq_node) {
    while (seq_node != nullptr) {
        if (returncheck_stmt(func, seq_node->getHead()))
            return true;
        seq_node = seq_node->getTail();
    }

    return false;
}

bool returncheck_stmt(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtNode> head) {
    // Return statement
    if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head))
        return true;
    // Scope statement
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        // May be in the body. Otherwise, keep going.
        if (scope_node->getBody() && returncheck_stmts(func, scope_node->getBody()))
            return true;
    }
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        // If there is only a true statement, we need to also have a return
        // after the if statement. If both branches are present, and they
        // BOTH return, then the whole statement returns.
        if (if_node->getFalseStmt()) {
            if (returncheck_stmts(func, if_node->getTrueStmt()) &&
                returncheck_stmts(func, if_node->getFalseStmt()))
                return true;
        }
    }

    return false;
}

void returncheck_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopSeqNode> nodes) {
    std::shared_ptr<ASTTopSeqNode> seq_node = nodes;

    while (seq_node != nullptr) {
        returncheck_top(module, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void returncheck_top(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopNode> node) {
    if (std::shared_ptr<ASTFunDefnTop> funDefn = std::dynamic_pointer_cast<ASTFunDefnTop>(node)) {
        bool isVoid = funDefn->getSignature()->getReturnType()->equal(ASTVoidType::get());

        std::shared_ptr<FunctionInfo> func = module->getFunction(funDefn->getName());

        // Check if all control flow paths return a value
        if (!returncheck_stmts(func, funDefn->getBody())) {

            // Void functions do not need to return on every control flow path, but the
            // code generator requires that there be a return statement, so insert one.
            if (isVoid) {
                std::shared_ptr<ASTStmtSeqNode> stmts = funDefn->getBody();

                // Body is empty
                if (stmts == nullptr)
                    funDefn->setBody(std::make_shared<ASTStmtSeqNode>(std::make_shared<ASTReturnStmt>(nullptr), nullptr));
                // Seek to the end of the body and insert a return
                else {
                    while (stmts->getTail() != nullptr)
                        stmts = stmts->getTail();

                    stmts->setTail(std::make_shared<ASTStmtSeqNode>(std::make_shared<ASTReturnStmt>(nullptr), nullptr));
                }
            }
            // The program is legitimately wrong
            else
                throw new NoReturnException();
        }
    }
    else
        throw new ASTMalformedException();
}

};
