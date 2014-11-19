/**
  * @file astnopstmt.h
  *
  * @brief Abstract syntax tree no-op node. Used to fill space in sequence linked
  * lists when there is no instruction, such as an empty 'else' branch.
  *
  * @author Sean James <seanjames777@gmail.com>
  */

#ifndef __ASTNOPSTMT_H
#define __ASTNOPSTMT_H

#include <ast/aststmtnode.h>

class ASTNopStmt : public ASTStmtNode {
};

#endif
