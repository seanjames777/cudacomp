/**
 * @file returncheck.h
 *
 * @brief Checks that every non-void function has a return statement on every
 * control flow path.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __RETURNCHECK_H
#define __RETURNCHECK_H

#include <ast/stmt/aststmtnode.h>

namespace Statics {

    bool returncheck_stmts(ASTStmtSeqNode *nodes);

    bool returncheck_stmt(ASTStmtNode *node);

};

#endif
