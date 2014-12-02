/**
 * @file cudapartition.h
 *
 * @brief Partitions functions into host, device, and global functions
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CUDAPARTITION_H
#define __CUDAPARTITION_H

#include <ast/astvisitor.h>
#include <statics/exceptions.h>
#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <options.h>

/**
 * @brief Partitions functions into host, device, and global functions
 */
class CudaPartition : public ASTVisitor {
private:

    std::shared_ptr<ModuleInfo> module;
    bool emitDevice;
    bool inDeviceMode;

public:

    CudaPartition(std::shared_ptr<ModuleInfo> module);

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    virtual void visitCallExp(std::shared_ptr<ASTCallExp> call) override;

};

#endif
