/**
 * @file converttype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/converttype.h>
#include <codegen/codegenctx.h>

namespace Codegen {

Type *convertType(std::shared_ptr<ASTTypeNode> type, CodegenCtx *c) {
    LLVMContext & ctx = getGlobalContext();

    if (std::shared_ptr<ASTIntegerType> int_type = std::dynamic_pointer_cast<ASTIntegerType>(type))
        return Type::getInt32Ty(ctx);
    else if (std::shared_ptr<ASTBooleanType> bool_type = std::dynamic_pointer_cast<ASTBooleanType>(type))
        return Type::getInt1Ty(ctx);
    else if (std::shared_ptr<ASTFloatType> bool_type = std::dynamic_pointer_cast<ASTFloatType>(type))
        return Type::getFloatTy(ctx);
    else if (std::shared_ptr<ASTVoidType> void_type = std::dynamic_pointer_cast<ASTVoidType>(type))
        return Type::getVoidTy(ctx);
    else if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type))
        return PointerType::getUnqual(convertType(ptr_type->getToType(), c));
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type))
        return PointerType::getUnqual(convertType(arr_type->getElemType(), c));
    else if (std::shared_ptr<ASTRecordType> rcd_type = std::dynamic_pointer_cast<ASTRecordType>(type)) {
        if (c)
            return c->getRecordType(rcd_type->getId());
        else
            throw ASTMalformedException(); 
    }
    else
        throw ASTMalformedException();

    return nullptr;
}

}
