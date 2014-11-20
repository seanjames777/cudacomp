/**
 * @file typectx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typectx.h>

TypeCtx::TypeCtx() {
}

void TypeCtx::setSymbol(std::string id, ASTType *type) {
    symbols[id] = type;
}

ASTType *TypeCtx::getSymbol(std::string id) {
    return symbols[id];
}
