/**
 * @file converttype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/converttype.h>
#include <ast/type/astintegertype.h>
#include <ast/type/astbooleantype.h>
#include <ast/type/astvoidtype.h>

namespace Codegen {

Type *convertType(ASTType *type) {
    LLVMContext & ctx = getGlobalContext();

    if (ASTIntegerType *int_type = dynamic_cast<ASTIntegerType *>(type))
        return Type::getInt32Ty(ctx);
    else if (ASTBooleanType *bool_type = dynamic_cast<ASTBooleanType *>(type))
        return Type::getInt1Ty(ctx);
    else if (ASTVoidType *void_type = dynamic_cast<ASTVoidType *>(type))
        return Type::getVoidTy(ctx);
    else
        throw new ASTMalformedException();

    return NULL;
}

}
