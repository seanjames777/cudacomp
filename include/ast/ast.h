/**
 * @file ast.h
 *
 * @brief Abstract Syntax Tree master header, which includes all of the separate AST headers. This
 * is convenient, but requires the compiler to pull in a lot of headers.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __AST_H
#define __AST_H

#include <ast/astnode.h>
#include <ast/astseqnode.h>
#include <ast/decl/astdeclnode.h>
#include <ast/decl/astfundecl.h>
#include <ast/decl/asttypedecl.h>
#include <ast/expr/astbinopexp.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/expr/astcallexp.h>
#include <ast/expr/astexpnode.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/expr/astindexexp.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astunopexp.h>
#include <ast/stmt/astexprstmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astassignstmt.h>
#include <ast/stmt/astwhilestmt.h>
#include <ast/type/astargnode.h>
#include <ast/type/astbooleantype.h>
#include <ast/type/astarrtype.h>
#include <ast/type/astfuntype.h>
#include <ast/type/astidtype.h>
#include <ast/type/astintegertype.h>
#include <ast/type/astptrtype.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astvoidtype.h>

#endif
