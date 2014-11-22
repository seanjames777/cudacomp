/**
 * @file converttype.h
 *
 * @brief Converts between ASTTypeNode's and llvm::Type's
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CONVERTTYPE_H
#define __CONVERTTYPE_H

#include <ast/type/asttypenode.h>

namespace Codegen {

Type *convertType(std::shared_ptr<ASTTypeNode> type);

}

#endif
