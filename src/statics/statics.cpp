/**
 * @file statics.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/statics.h>

namespace Statics {

void run(ASTTopSeqNode *node, ModuleInfo *module) {
    typecheck_tops(module, node);
    returncheck_tops(module, node);
}

}
