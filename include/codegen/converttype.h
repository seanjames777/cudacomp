/**
 * @file converttype.h
 *
 * @brief Converts between ASTTypeNode's and llvm::Type's
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CONVERTTYPE_H
#define __CONVERTTYPE_H

#include <ast/ast.h>

// TODO
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Scalar.h>

using namespace llvm;

namespace Codegen {

Type *convertType(std::shared_ptr<ASTTypeNode> type);

}

#endif
