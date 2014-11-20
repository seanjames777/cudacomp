/**
 * @file converttype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CONVERTTYPE_H
#define __CONVERTTYPE_H

// TODO: probably should be called ASTTypeNode

#include <ast/type/asttype.h>

namespace Codegen {

Type *convertType(ASTType *type);

}

#endif
