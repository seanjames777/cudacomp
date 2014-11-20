/**
 * @file statics.h
 *
 * @brief Static semantics top-level. Runs a series of analyses such as type-checking,
 * return statement checks, etc. and builds up information about the program being
 * checked.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __STATICS_H
#define __STATICS_H

#include <statics/moduleinfo.h>
#include <statics/typecheck.h>
#include <statics/returncheck.h>

namespace Statics {

/**
 * @brief Runs all static checks against a program give the first node in its
 * list of top-level declarations/definitions. Throws one of a series of
 * exceptions if the code fails a static check. Outputs type information,
 * function signatures, etc.
 *
 * @param[in]  node   Top-level linked list output by parser
 * @param[out] module Output module information
 */
void run(ASTTopSeqNode *node, ModuleInfo *module);

}

#endif
