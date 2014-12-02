/**
 * @file statics.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/statics.h>

namespace Statics {

std::shared_ptr<ModuleInfo> run(std::shared_ptr<ASTDeclSeqNode> node) {
    struct CCArgs *args = getOptions();

    std::shared_ptr<ModuleInfo> module = std::make_shared<ModuleInfo>();

    if (args->verbose)
        std::cout << "Resolving types..." << std::endl;

    TypeResolve typeResolver(module);
    typeResolver.run(node);

    if (args->verbose)
        std::cout << "Checking functions..." << std::endl;

    FunCheck funChecker(module);
    funChecker.run(node);

    SymbolMangle mangle(module);
    mangle.run(node);

    if (args->verbose)
        std::cout << "Checking symbols..." << std::endl;

    SymbolCheck symbolChecker;
    symbolChecker.run(node);

    if (args->verbose)
        std::cout << "Checking types..." << std::endl;

    typecheck_tops(module, node);
    returncheck_tops(module, node);

    if (args->verbose)
        std::cout << "Partitioning CUDA functions..." << std::endl;

    CudaPartition part(module);
    part.run(node);

    return module;
}

}
