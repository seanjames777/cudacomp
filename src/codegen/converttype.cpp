/**
 * @file converttype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/converttype.h>
#include <codegen/codegenctx.h>

namespace Codegen {

Type *convertType(std::shared_ptr<ASTTypeNode> type, CodegenCtx *ctx) {
    LLVMContext & llvmCtx = getGlobalContext();

    if (std::shared_ptr<ASTIntegerType> int_type = std::dynamic_pointer_cast<ASTIntegerType>(type))
        return Type::getInt32Ty(llvmCtx);
    else if (std::shared_ptr<ASTBooleanType> bool_type = std::dynamic_pointer_cast<ASTBooleanType>(type))
        return Type::getInt1Ty(llvmCtx);
    else if (std::shared_ptr<ASTFloatType> bool_type = std::dynamic_pointer_cast<ASTFloatType>(type))
        return Type::getFloatTy(llvmCtx);
    else if (std::shared_ptr<ASTVoidType> void_type = std::dynamic_pointer_cast<ASTVoidType>(type))
        return Type::getVoidTy(llvmCtx);
    else if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type))
        return PointerType::getUnqual(convertType(ptr_type->getToType(), ctx));
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type))
        return PointerType::getUnqual(convertType(arr_type->getElemType(), ctx));
    else if (std::shared_ptr<ASTRecordType> rcd_type = std::dynamic_pointer_cast<ASTRecordType>(type)) {
        return ctx->getRecordType(rcd_type->getId());
    }
    else
        throw ASTMalformedException();

    return nullptr;
}

}
