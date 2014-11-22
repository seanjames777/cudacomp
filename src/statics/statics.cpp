/**
 * @file statics.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/statics.h>

namespace Statics {

std::shared_ptr<ModuleInfo> run(std::shared_ptr<ASTTopSeqNode> node) {
    std::shared_ptr<ModuleInfo> module = std::make_shared<ModuleInfo>();

    typecheck_tops(module, node);
    returncheck_tops(module, node);

    return module;
}

}
