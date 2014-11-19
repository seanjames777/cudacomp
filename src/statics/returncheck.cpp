/**
 * @file returncheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/returncheck.h>
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

bool returncheck_stmts(ASTStmtSeqNode *seq_node) {
    while (seq_node != NULL) {
        if (returncheck_stmt(seq_node->getHead()))
            return true;
        seq_node = seq_node->getTail();
    }

    return false;
}

bool returncheck_stmt(ASTStmtNode *head) {
    // Return statement
    if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head))
        return true;
    // Scope statement
    else if (ASTScope *scope_node = dynamic_cast<ASTScope *>(head)) {
        // May be in the body. Otherwise, keep going.
        if (scope_node->getBody() && returncheck_stmts(scope_node->getBody()))
            return true;
    }
    else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
        // If there is only a true statement, we need to also have a return
        // after the if statement. If both branches are present, and they
        // BOTH return, then the whole statement returns.
        if (if_node->getFalseStmt()) {
            if (returncheck_stmts(if_node->getTrueStmt()) &&
                returncheck_stmts(if_node->getFalseStmt()))
                return true;
        }
    }

    return false;
}

};
