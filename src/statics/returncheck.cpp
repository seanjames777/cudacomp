/**
 * @file returncheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/returncheck.h>
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
#include <ast/astscope.h>
#include <ast/astifstmt.h>
#include <ast/astboolean.h>
#include <ast/astnopstmt.h>

namespace Statics {

bool returncheck_stmt(ASTStmtNode *node) {
    // Sequence node
    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        // We always have a head
        ASTStmtNode *head = seq_node->getHead();

        // Return statement
        if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head))
            return true;
        // Scope statement
        else if (ASTScope *scope_node = dynamic_cast<ASTScope *>(head)) {
            // May be in the body. Otherwise, keep going.
            if (scope_node->getBody() && returncheck_stmt(scope_node->getBody()))
                return true;
        }
        else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
            // If there is only a true statement, we need to also have a return
            // after the if statement. If both branches are present, and they
            // BOTH return, then the whole statement returns.
            if (if_node->getFalseStmt()) {
                if (returncheck_stmt(if_node->getTrueStmt()) &&
                    returncheck_stmt(if_node->getFalseStmt()))
                    return true;
            }
        }

        // If there are more statements, check them.
        if (seq_node->getTail() && returncheck_stmt(seq_node->getTail()))
            return true;
        // Otherwise, there is no return
        else
            return false;
    }
    // Illegal. The AST is always a linked list of statements.
    else
        throw new ASTMalformedException();

    return false;
}

};
