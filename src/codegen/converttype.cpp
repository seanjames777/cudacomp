/**
 * @file converttype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/converttype.h>

namespace Codegen {

Type *convertType(std::shared_ptr<ASTTypeNode> type) {
    LLVMContext & ctx = getGlobalContext();

    if (std::shared_ptr<ASTIntegerType> int_type = std::dynamic_pointer_cast<ASTIntegerType>(type))
        return Type::getInt32Ty(ctx);
    else if (std::shared_ptr<ASTBooleanType> bool_type = std::dynamic_pointer_cast<ASTBooleanType>(type))
        return Type::getInt1Ty(ctx);
    else if (std::shared_ptr<ASTVoidType> void_type = std::dynamic_pointer_cast<ASTVoidType>(type))
        return Type::getVoidTy(ctx);
    else if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type))
        return PointerType::getUnqual(convertType(ptr_type->getToType()));
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type))
        return PointerType::getUnqual(convertType(arr_type->getElemType()));
    else if (std::shared_ptr<ASTRecordType> rcd_type = std::dynamic_pointer_cast<ASTRecordType>(type)) {

        std::vector<Type *> elems;
        std::shared_ptr<ASTArgSeqNode> fields = rcd_type->getFields();
        while (fields != nullptr) {
            std::shared_ptr<ASTTypeNode> field_type = fields->getHead()->getType();
            elems.push_back(convertType(field_type));
            fields = fields->getTail();
        }

        return StructType::create(elems,rcd_type->getId());
    }
    else
        throw new ASTMalformedException();

    return nullptr;
}

}
