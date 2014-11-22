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

Type *convertType(std::shared_ptr<ASTTypeNode> type) {
    LLVMContext & ctx = getGlobalContext();

    if (std::shared_ptr<ASTIntegerType> int_type = std::dynamic_pointer_cast<ASTIntegerType>(type))
        return Type::getInt32Ty(ctx);
    else if (std::shared_ptr<ASTBooleanType> bool_type = std::dynamic_pointer_cast<ASTBooleanType>(type))
        return Type::getInt1Ty(ctx);
    else if (std::shared_ptr<ASTVoidType> void_type = std::dynamic_pointer_cast<ASTVoidType>(type))
        return Type::getVoidTy(ctx);
    else
        throw new ASTMalformedException();

    return NULL;
}

}
