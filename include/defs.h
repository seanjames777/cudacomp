/**
 * @file defs.h
 *
 * @brief Global definitions and includes
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __DEFS_H
#define __DEFS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <memory>

// TODO move me
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

#endif
