/**
 * @file cudapartition.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/cudapartition.h>

CudaPartition::CudaPartition(std::shared_ptr<ModuleInfo> module)
    : module(module)
{
    struct CCArgs *args = getOptions();
    emitDevice = args->emit_device;

    inDeviceMode = emitDevice;
}

void CudaPartition::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitNode(ast);

    std::shared_ptr<ASTDeclSeqNode> decls = ast;

    // Determine which functions are global functions
    while (decls != nullptr) {
        if (std::shared_ptr<ASTFunDecl> funDecl = std::dynamic_pointer_cast<ASTFunDecl>(decls->getHead())) {
            if (funDecl->isDefn()) {
                std::shared_ptr<FunctionInfo> info = module->getFunction(funDecl->getName());
                std::shared_ptr<ASTFunType> sig = info->getSignature();

                // Functions with dimension arguments are always global
                if (sig->getDimArgs() != nullptr)
                    info->setUsage(FunctionInfo::Global);
                // The _cc_main function is always global in device-only mode
                else if (info->getName() == "_cc_main") {
                    if (emitDevice)
                        info->setUsage(FunctionInfo::Global);
                    else
                        info->setUsage(FunctionInfo::Host);
                }

                // TODO: make sure global functions are never called directly
            }
        }

        decls = decls->getTail();
    }
}

void CudaPartition::visitCallExp(std::shared_ptr<ASTCallExp> call) {
    std::shared_ptr<FunctionInfo> info = module->getFunction(call->getId());

    // Skip global functions
    if (info->getSignature()->getDimArgs() != nullptr) {
        inDeviceMode = true;
        ASTVisitor::visitCallExp(call);
        inDeviceMode = emitDevice;
    }
    else {
        enum FunctionInfo::CudaUsage usage = info->getUsage();

        // If we make a call from host or device mode, add to the corresponding set
        if (inDeviceMode)
            usage = (enum FunctionInfo::CudaUsage)(usage | FunctionInfo::Device);
        else
            usage = (enum FunctionInfo::CudaUsage)(usage | FunctionInfo::Host);

        info->setUsage(usage);

        ASTVisitor::visitCallExp(call);
    }
}
