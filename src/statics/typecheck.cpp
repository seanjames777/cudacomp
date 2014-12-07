/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>

namespace Statics {

// May be better named as isNonLargeType; returns true for void types
bool isSmallType(std::shared_ptr<ASTTypeNode> node) {
    if (std::shared_ptr<ASTRecordType> type = std::dynamic_pointer_cast<ASTRecordType>(node))
        return false;
    return true;
}

bool isSmallNonvoidType(std::shared_ptr<ASTTypeNode> node) {
    if (std::shared_ptr<ASTVoidType> type = std::dynamic_pointer_cast<ASTVoidType>(node))
        return false;
    return isSmallType(node);
}

// Sets the type of an expression, propogating through sub-expressions such as
// the ternary operator. This is used to recursively set unknown types on
// expressions such as NULL, for which a type cannot be immediately synthesized.
static void prop_type(std::shared_ptr<ASTExpNode> exp, std::shared_ptr<ASTTypeNode> type) {
    if (std::shared_ptr<ASTTernopExp> tern_exp = std::dynamic_pointer_cast<ASTTernopExp>(exp)) {
        tern_exp->setType(type);
        prop_type(tern_exp->getTrueExp(), type);
        prop_type(tern_exp->getFalseExp(), type);
    }
    else
        exp->setType(type);
}

// Typecheck an expression against an expected type, where the expression may
// possible be a null pointer. If the expression is null, and the expected
// type is a pointer type, the expression's type is set and propogated to the
// expected type. Then returns true. Otherwise, returns false.
static bool check_null(
    std::shared_ptr<ASTExpNode> exp,
    std::shared_ptr<ASTTypeNode> actual_type,
    std::shared_ptr<ASTTypeNode> expected_type)
{
    // If the right handle side is NULL, then it's OK if we're assigning
    // to a pointer.
    if (actual_type->equal(ASTPtrType::getNullPtr())) {
        std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(expected_type);

        if (!ptr_type)
            throw IllegalTypeException();

        prop_type(exp, expected_type);

        return true;
    }

    return false;
}

std::shared_ptr<ASTTypeNode> typecheck_exp(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    bool lval,
    std::shared_ptr<ASTExpNode> node)
{
    // Variable reference
    if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(node))
        node->setType(func->getLocalType(id_exp->getId()));
    // Array subscript
    else if (std::shared_ptr<ASTIndexExp> idx_exp = std::dynamic_pointer_cast<ASTIndexExp>(node)) {
        std::shared_ptr<ASTTypeNode> lhs_type = typecheck_exp(mod, func, lval, idx_exp->getLValue());
        std::shared_ptr<ASTTypeNode> sub_type = typecheck_exp(mod, func, false, idx_exp->getSubscript());

        // Right hand side must be an integer
        if (!sub_type->equal(ASTIntegerType::get()))
            throw IllegalTypeException();

        // Left hand side must be an array
        if (std::shared_ptr<ASTArrType> lhs_arr = std::dynamic_pointer_cast<ASTArrType>(lhs_type))
            node->setType(lhs_arr->getElemType());
        else
            throw IllegalTypeException();
    }
    // Pointer dereference
    else if (std::shared_ptr<ASTDerefExp> ptr_exp = std::dynamic_pointer_cast<ASTDerefExp>(node)) {
        std::shared_ptr<ASTTypeNode> subexp_type = typecheck_exp(mod, func, lval, ptr_exp->getExp());

        // We can't assign a type to *null, *(true ? null : null), etc.
        if (subexp_type->equal(ASTPtrType::getNullPtr()))
            throw IllegalTypeException();
        // The sub-exp type must be a pointer
        else if (std::shared_ptr<ASTPtrType> sub_ptr = std::dynamic_pointer_cast<ASTPtrType>(subexp_type))
            node->setType(sub_ptr->getToType());
        else
            throw IllegalTypeException();
    }
    // Record access
    else if (std::shared_ptr<ASTRecordAccessExp> record_exp = std::dynamic_pointer_cast<ASTRecordAccessExp>(node)) {
        std::shared_ptr<ASTTypeNode> lvalue_type = typecheck_exp(mod, func, lval, record_exp->getLValue());

        std::string field_name = record_exp->getId();

        // This type needs to be a record
        if (std::shared_ptr<ASTRecordType> record_type = std::dynamic_pointer_cast<ASTRecordType>(lvalue_type)) {
            // The record type needs to be defined
            if (!mod->isRecordDefined(record_type->getId()))
                throw IllegalTypeException();
            node->setType(record_type->getField(field_name)->getType());
        }
        else
            throw IllegalTypeException();
    }
    // If we are expecting only lvalues, then any of the following are wrong
    else if (lval)
        throw IllegalLValueException();
    // Integer constant
    else if (std::shared_ptr<ASTIntegerExp> int_exp = std::dynamic_pointer_cast<ASTIntegerExp>(node))
        node->setType(ASTIntegerType::get());
    // Boolean constant
    else if (std::shared_ptr<ASTBooleanExp> bool_exp = std::dynamic_pointer_cast<ASTBooleanExp>(node))
        node->setType(ASTBooleanType::get());
    // Float constant
    else if (std::shared_ptr<ASTFloatExp> float_exp = std::dynamic_pointer_cast<ASTFloatExp>(node))
        node->setType(ASTFloatType::get());
    // Null constant
    else if (std::shared_ptr<ASTNullExp> null_exp = std::dynamic_pointer_cast<ASTNullExp>(node))
        node->setType(std::make_shared<ASTPtrType>(nullptr));
    // Unary operator
    else if (std::shared_ptr<ASTUnopExp> unop_exp = std::dynamic_pointer_cast<ASTUnopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, false, unop_exp->getExp());

        // Types must be appropriate for operation
        switch(unop_exp->getOp()) {
        case ASTUnopExp::NOT:
            if (!exp_type->equal(ASTBooleanType::get()))
                throw IllegalTypeException();
            else
                node->setType(exp_type);
            break;
        case ASTUnopExp::BNOT:
        case ASTUnopExp::NEG:
            if (!exp_type->equal(ASTIntegerType::get()))
                throw IllegalTypeException();
            else
                node->setType(exp_type);
            break;
        }
    }
    // Binary operator
    else if (std::shared_ptr<ASTBinopExp> binop_exp = std::dynamic_pointer_cast<ASTBinopExp>(node)) {
        // Get operand types
        std::shared_ptr<ASTTypeNode> t1 = typecheck_exp(mod, func, false, binop_exp->getE1());
        std::shared_ptr<ASTTypeNode> t2 = typecheck_exp(mod, func, false, binop_exp->getE2());

        // Types must be appropriate for operation
        switch(binop_exp->getOp()) {
        case ASTBinopExp::ADD:
        case ASTBinopExp::SUB:
        case ASTBinopExp::MUL:
        case ASTBinopExp::DIV:
        case ASTBinopExp::MOD:
            if (!t1->equal(t2))
                throw IllegalTypeException();
            else if (t1->equal(ASTIntegerType::get()))
                node->setType(ASTIntegerType::get());
            else if (t1->equal(ASTFloatType::get()))
                node->setType(ASTFloatType::get());
            else
                throw IllegalTypeException();
            break;
        case ASTBinopExp::SHL:
        case ASTBinopExp::SHR:
        case ASTBinopExp::BAND:
        case ASTBinopExp::BOR:
        case ASTBinopExp::BXOR:
            if (!t1->equal(t2))
                throw IllegalTypeException();
            else if (t1->equal(ASTIntegerType::get()))
                node->setType(ASTIntegerType::get());
            else
                throw IllegalTypeException();
            break;
        case ASTBinopExp::OR:
        case ASTBinopExp::AND:
            if (!t1->equal(t2))
                throw IllegalTypeException();
            else if (t1->equal(ASTBooleanType::get()))
                node->setType(ASTBooleanType::get());
            else
                throw IllegalTypeException();
            break;
        case ASTBinopExp::LEQ:
        case ASTBinopExp::GEQ:
        case ASTBinopExp::LT:
        case ASTBinopExp::GT:
            if (!t1->equal(t2))
                throw IllegalTypeException();
            else if (t1->equal(ASTIntegerType::get()))
                node->setType(ASTBooleanType::get());
            else if (t1->equal(ASTFloatType::get()))
                node->setType(ASTBooleanType::get());
            else
                throw IllegalTypeException();
            break;
        case ASTBinopExp::NONE:
            throw ASTMalformedException();
            break;
        case ASTBinopExp::EQ:
        case ASTBinopExp::NEQ:
            if (std::shared_ptr<ASTPtrType> ptrType1 = std::dynamic_pointer_cast<ASTPtrType>(t1)) {
                std::shared_ptr<ASTPtrType> ptrType2 = std::dynamic_pointer_cast<ASTPtrType>(t2);

                // Both sides must be pointer types
                if (!ptrType2)
                    throw IllegalTypeException();

                // If both sides have definite 'to' types, OK. If both sides do
                // not, then we're comparing null to null: also OK.
                if (ptrType1->equal(ptrType2)) {
                    // If we compare null to null, we need to just make up a type
                    // for the pointers.
                    if (ptrType1->getToType() == nullptr) {
                        // Make sure both sides have indefinite types
                        ptrType1->setToType(ASTIntegerType::get());
                        ptrType2->setToType(ASTIntegerType::get());

                        // Propogate the types back down
                        prop_type(binop_exp->getE1(), ptrType1);
                        prop_type(binop_exp->getE2(), ptrType2);

                        // Set the binop's type
                        node->setType(ASTBooleanType::get());
                    }

                    node->setType(ASTBooleanType::get());
                }
                // If the types are not the same, and they are not both indefinite,
                // then there's a pointer type mismatch
                else if (ptrType1->getToType() != nullptr && ptrType2->getToType() != nullptr)
                    throw IllegalTypeException();
                // Otherwise, one pointer is indefinite, so we now know its type
                else {
                    // Find the definite type
                    std::shared_ptr<ASTTypeNode> definiteType =
                        ptrType1->getToType() != nullptr ? ptrType1->getToType()
                                                         : ptrType2->getToType();

                    // Make sure both sides have indefinite types
                    ptrType1->setToType(definiteType);
                    ptrType2->setToType(definiteType);

                    // Propogate the types back down
                    prop_type(binop_exp->getE1(), ptrType1);
                    prop_type(binop_exp->getE2(), ptrType2);

                    // Set the binop's type
                    node->setType(ASTBooleanType::get());
                }
            }
            else if (!t1->equal(t2))
                throw IllegalTypeException();
            else if (t1->equal(ASTIntegerType::get()))
                node->setType(ASTBooleanType::get());
            else if (t1->equal(ASTFloatType::get()))
                node->setType(ASTBooleanType::get());
            else if (t1->equal(ASTBooleanType::get()))
                node->setType(ASTBooleanType::get());
            else if (std::shared_ptr<ASTArrType> arrType = std::dynamic_pointer_cast<ASTArrType>(t1))
                node->setType(ASTBooleanType::get());
            else
                throw IllegalTypeException();
            break;
        }
    }
    // Ternary operator
    else if (std::shared_ptr<ASTTernopExp> tern_exp = std::dynamic_pointer_cast<ASTTernopExp>(node)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> condType = typecheck_exp(mod, func, false, tern_exp->getCond());

        if (!condType->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        // Each side must have the same type
        std::shared_ptr<ASTTypeNode> leftType = typecheck_exp(mod, func, false, tern_exp->getTrueExp());
        std::shared_ptr<ASTTypeNode> rightType = typecheck_exp(mod, func, false, tern_exp->getFalseExp());

        // Treat pointer types carefully
        if (std::shared_ptr<ASTPtrType> ptrType1 = std::dynamic_pointer_cast<ASTPtrType>(leftType)) {
            std::shared_ptr<ASTPtrType> ptrType2 = std::dynamic_pointer_cast<ASTPtrType>(rightType);

            // Both sides must be pointer types
            if (!ptrType2)
                throw IllegalTypeException();

            // If both sides have definite 'to' types, OK. If both sides do
            // not, then both are null: also OK.
            if (ptrType1->equal(ptrType2)) {
                // If both are null, the whole thing is null
                if (ptrType1->getToType() == nullptr)
                    node->setType(ASTPtrType::getNullPtr());
                // Otherwise, WLOG use the left type
                else
                    node->setType(ptrType1);
            }
            // If the types are not the same, and they are not both indefinite,
            // then there's a pointer type mismatch
            else if (ptrType1->getToType() != nullptr && ptrType2->getToType() != nullptr)
                throw IllegalTypeException();
            // Otherwise, one pointer is indefinite, so we now know its type
            else {
                // Find the definite type
                std::shared_ptr<ASTTypeNode> definiteType =
                    ptrType1->getToType() != nullptr ? ptrType1->getToType()
                                                     : ptrType2->getToType();

                // Make sure both sides have indefinite types
                ptrType1->setToType(definiteType);
                ptrType2->setToType(definiteType);

                // Propogate the types back down
                prop_type(tern_exp->getTrueExp(), ptrType1);
                prop_type(tern_exp->getFalseExp(), ptrType2);

                // WLOG use the left type
                node->setType(ptrType1);
            }
        }
        // Otherwise, the types must be the same
        else if (!leftType->equal(rightType))
            throw IllegalTypeException();
        // May not have void type
        else if (leftType->equal(ASTVoidType::get()))
            throw IllegalTypeException();
        else
            node->setType(leftType);
    }
    // Function call
    else if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        // The function checker guarantees that this exists
        std::shared_ptr<FunctionInfo> call_func = mod->getFunction(call_exp->getId());

        std::shared_ptr<ASTFunType> sig = call_func->getSignature();

        // Check the expressions against the signature
        std::shared_ptr<ASTExpSeqNode> exprs = call_exp->getArgs();
        std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();

        while (true) {
            // Make sure the argument counts match
            if ((args == nullptr || exprs == nullptr) && (args != nullptr || exprs != nullptr))
                throw IllegalTypeException(); // TODO maybe a different exception
            else if (args == nullptr && exprs == nullptr)
                break;

            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, false, exprs->getHead());
            std::shared_ptr<ASTTypeNode> arg_type = args->getHead()->getType();

            // The expression might be null, which is OK if the argument is a pointer type
            if (check_null(exprs->getHead(), exp_type, arg_type)) {
                // Nothing to do
            }
            else if (!exp_type->equal(arg_type))
                throw IllegalTypeException();

            args = args->getTail();
            exprs = exprs->getTail();
        }

        node->setType(sig->getReturnType());
    }
    // Array allocation
    else if (std::shared_ptr<ASTAllocArrayExp> alloc_exp = std::dynamic_pointer_cast<ASTAllocArrayExp>(node)) {
        std::shared_ptr<ASTTypeNode> elemType = alloc_exp->getElemType();

        if (elemType->equal(ASTVoidType::get()))
            throw IllegalTypeException();

        // Size must be an integer
        std::shared_ptr<ASTTypeNode> sizeType = typecheck_exp(mod, func, false, alloc_exp->getLength());

        if (!sizeType->equal(ASTIntegerType::get()))
            throw IllegalTypeException();

        // Returns an array of elemTypes
        node->setType(std::make_shared<ASTArrType>(elemType));
    }
    // Heap allocation
    else if (std::shared_ptr<ASTAllocExp> alloc_exp = std::dynamic_pointer_cast<ASTAllocExp>(node)) {
        std::shared_ptr<ASTTypeNode> elemType = alloc_exp->getElemType();

        // We cannot allocate certain types
        // TODO: better void check (e.g. structural search for void)
        if (elemType->equal(ASTVoidType::get()))
            throw IllegalTypeException();

        // Returns a pointer to an elemType
        node->setType(std::make_shared<ASTPtrType>(elemType));
    }

    assert(node->getType() && "Did not synthesize a type for an expression");
    return node->getType();
}

void typecheck_stmts(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    std::shared_ptr<ASTStmtSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_stmt(mod, func, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_stmt(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<FunctionInfo> func,
    std::shared_ptr<ASTStmtNode> head)
{
    // If the first node is a variable declaration, we need to declare and
    // possibly define it in the rest of the code. Need to check the type
    // before marking as declared, in case the definition tries to be
    // recursive.
    if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head)) {
        std::shared_ptr<ASTTypeNode> decl_type = decl_stmt->getType();

        if (!isSmallNonvoidType(decl_type))
            throw IllegalTypeException();

        std::shared_ptr<ASTExpNode> decl_exp = decl_stmt->getExp();

        // If there is a definition, check the type and mark as defined
        if (decl_exp) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, false, decl_exp);

            // If the right handle side is NULL, then it's OK if we're assigning
            // to a pointer.
            if (check_null(decl_exp, exp_type, decl_type)) {
                // Nothing to do
            }
            // Otherwise, the types must match
            else if (!exp_type->equal(decl_type))
                throw IllegalTypeException();
        }

        // Store the type
        func->addLocal(decl_stmt->getId(), decl_type); // TODO declareSymbol
    }
    // Assignment. Mark as defined and check the rest of the code
    else if (std::shared_ptr<ASTAssignStmt> defn_stmt = std::dynamic_pointer_cast<ASTAssignStmt>(head)) {
        std::shared_ptr<ASTTypeNode> lhs_type = typecheck_exp(mod, func, true, defn_stmt->getLValue());
        std::shared_ptr<ASTTypeNode> rhs_type = typecheck_exp(mod, func, false, defn_stmt->getExp());

        if (!isSmallNonvoidType(lhs_type))
            throw IllegalTypeException();

        // If the right handle side is NULL, then it's OK if we're assigning
        // to a pointer.
        if (check_null(defn_stmt->getExp(), rhs_type, lhs_type)) {
            // Nothing to do
        }
        // Must assign the same type
        else if (!rhs_type->equal(lhs_type))
            throw IllegalTypeException();
    }
    // Return statement
    else if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        std::shared_ptr<ASTTypeNode> expected = func->getSignature()->getReturnType();

        bool isVoid = expected->equal(ASTVoidType::get());

        // Should not return anything in a void function
        if (isVoid && ret_node->getExp())
            throw IllegalTypeException(); // TODO better exception

        // Must return something in a non-void function
        if (!ret_node->getExp() && !isVoid)
            throw IllegalTypeException(); // TODO better exception

        // Must return the correct type in a non-void function
        if (ret_node->getExp()) {
            std::shared_ptr<ASTTypeNode> exp_type = typecheck_exp(mod, func, false, ret_node->getExp());

            // If the right handle side is NULL, then it's OK if we're returning
            // a pointer
            if (check_null(ret_node->getExp(), exp_type, expected)) {
                // Nothing to do
            }
            // Must return the correct type
            else if (!exp_type->equal(expected))
                throw IllegalTypeException();
        }
    }
    // Scope statement
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        if (scope_node->getBody())
            typecheck_stmts(mod, func, scope_node->getBody());
    }
    // If statement
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, false, if_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        typecheck_stmts(mod, func, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            typecheck_stmts(mod, func, if_node->getFalseStmt());
    }
    // Assert statement
    else if (std::shared_ptr<ASTAssertStmt> assert_node = std::dynamic_pointer_cast<ASTAssertStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, false, assert_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();
    }
    // While statement
    else if (std::shared_ptr<ASTWhileStmt> while_node = std::dynamic_pointer_cast<ASTWhileStmt>(head)) {
        // Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, false, while_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        typecheck_stmts(mod, func, while_node->getBody());
    }
    // For loop
    else if (std::shared_ptr<ASTForStmt> for_node = std::dynamic_pointer_cast<ASTForStmt>(head)) {
        // Evaluate the initialization statement
        if (for_node->getInit())
            typecheck_stmt(mod, func, for_node->getInit());

        //Condition must be a boolean
        std::shared_ptr<ASTTypeNode> cond_type = typecheck_exp(mod, func, false, for_node->getCond());

        if (!cond_type->equal(ASTBooleanType::get()))
            throw IllegalTypeException();

        // TODO: additional checks on the iter statement
        // Evaluate the iter statement
        if (for_node->getIter()) {
            if (std::shared_ptr<ASTVarDeclStmt> decl = std::dynamic_pointer_cast<ASTVarDeclStmt>(for_node->getIter()))
                throw IllegalTypeException(); // TODO: Better exception

            typecheck_stmt(mod, func, for_node->getIter());
        }

        // Evaluate the body of the loop
        typecheck_stmts(mod, func, for_node->getBody());
    }
    // Expression statement
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head)) {
        std::shared_ptr<ASTTypeNode> type = typecheck_exp(mod, func, false, exp_stmt->getExp());

        // If the expression has an indefinite pointer type, assign an arbitrary
        // type
        if (auto ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type))
            if (!ptr_type->getToType()) {
                ptr_type->setToType(ASTIntegerType::get());
                prop_type(exp_stmt->getExp(), ptr_type);
            }

        // The expression cannot be a large type
        if (!isSmallType(type))
            throw IllegalTypeException();
    }
    else
        throw ASTMalformedException();
}

void typecheck_tops(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typecheck_top(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typecheck_top(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclNode> node)
{
    if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(node)) {
        std::shared_ptr<ASTFunType> sig = funDefn->getSignature();

        // Function cannot have a large type return value
        if (!isSmallType(sig->getReturnType()))
                throw IllegalTypeException();

        // Make sure:
        //   - Arguments do not have the same name
        //   - Arguments do not have void type
        //   - Arguments do not have large type
        SymbolSet argNames;

        std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();

        while (args != nullptr) {
            std::shared_ptr<ASTArgNode> arg = args->getHead();

            if (argNames.find(arg->getName()) != argNames.end())
                throw IllegalTypeException(); // TODO better exception
            else
                argNames.insert(arg->getName());

            if (!isSmallNonvoidType(arg->getType()))
                throw IllegalTypeException();

            args = args->getTail();
        }

        // Skip empty declarations
        if (!funDefn->isDefn())
            return;

        // The function checker has already allocated FunctionInfo's for us
        std::shared_ptr<FunctionInfo> funInfo = mod->getFunction(funDefn->getName());

        funInfo->copyArgumentsToLocals();

        // Check the function body, building the local symbol table in the process
        typecheck_stmts(mod, funInfo, funDefn->getBody());
    }
    // Skip
    else if (std::shared_ptr<ASTTypeDecl> typeDefn = std::dynamic_pointer_cast<ASTTypeDecl>(node)) {
        // Cannot typedef void
        if (typeDefn->getType()->equal(ASTVoidType::get()))
            throw IllegalTypeException();
    }
    else if (std::shared_ptr<ASTRecordDecl> recordDecl = std::dynamic_pointer_cast<ASTRecordDecl>(node)) {
        if (recordDecl->isDefn())
            mod->defineRecord(recordDecl->getName());
    }
}

};
