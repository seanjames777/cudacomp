/**
 * @file symbolcheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/symbolcheck.h>

namespace Statics {

SymbolCheck::SymbolCheck()
    : symbolCounter(0)
{
}

void SymbolCheck::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitNode(ast);
}

void SymbolCheck::visitIdentifierExp(std::shared_ptr<ASTIdentifierExp> id) {
    // Must be declared
    if (decl.find(id->getId()) == decl.end())
        throw UndeclaredIdentifierException(id->getId());

    if (!id->isLValue() && def.find(id->getId()) == def.end())
        throw UndefinedIdentifierException(id->getId());
    else {
        // Mark as defined
        if (def.find(id->getId()) == def.end())
            def.insert(id->getId());
    }

    // Rename to a unique symbol
    id->setId(rename.get(id->getId()));

    ASTVisitor::visitIdentifierExp(id);
}

void SymbolCheck::visitCallExp(std::shared_ptr<ASTCallExp> call) {
    // Function must not have been shadowed by a variable. This leaves room for function
    // pointers.
    if (decl.find(call->getId()) != decl.end())
        throw IllegalTypeException(); // TODO better error

    ASTVisitor::visitCallExp(call);
}

Symbol SymbolCheck::makeUniqueSymbol(Symbol original, bool defined) {
    // The original symbol must not be declared yet
    if (decl.find(original) != decl.end())
        throw RedeclaredIdentifierException(original);

    // Mark as declared
    decl.insert(original);

    // Mark the symbol as defined if necessary
    if (defined)
        def.insert(original);

    // Create a unique name and add it to the 'rename' map
    std::stringstream ss;
    ss << "sym" << symbolCounter++ << "_" << original;
    std::string unique = ss.str();

    rename.set(original, unique);

    return unique;
}

void SymbolCheck::visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) {
    ASTVisitor::visitVarDeclStmt(varDecl);

    // Check the rules and generate a unique name
    std::shared_ptr<ASTExpNode> decl_exp = varDecl->getExp();
    std::string newName = makeUniqueSymbol(varDecl->getId(), decl_exp != nullptr);
    varDecl->setId(newName);
}

void SymbolCheck::visitAssignStmt(std::shared_ptr<ASTAssignStmt> assign) {
    // If we assign to a variable, then we can mark it as an lvalue. Then the recursive
    // visit to the identifier will handle everything for us. Specifically, it won't
    // require that the identifier be defined yet, because we are defining it here.
    if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(assign->getLValue())) {
        // The 'isLValue' value means that the identifier doesn't have to be
        // defined to be assigned to. It must be defined if this is a compound
        // assignment.
        id_exp->setIsLValue(assign->getOp() == ASTBinopExp::NONE);

        // Make sure the right hand side is valid before assigning
        visitNode(assign->getExp());
        visitNode(assign->getLValue());
    }
    // Otherwise proceed normally
    else
        ASTVisitor::visitAssignStmt(assign);
}

void SymbolCheck::visitReturnStmt(std::shared_ptr<ASTReturnStmt> ret) {
    ASTVisitor::visitReturnStmt(ret);

    // Return statements need to define any variables that have been
    // declared. Every control flow path from a declaration of a
    // variable to its use must contain a definition, and there is no
    // control flow across a return statement.
    def = decl;
}

void SymbolCheck::visitScopeStmt(std::shared_ptr<ASTScopeStmt> scope) {
    if (scope->getBody()) {
        // Save the declarations and definitions outside of the scope
        SymbolSet old_decl = decl;
        SymbolSet old_def = def;

        ASTVisitor::visitScopeStmt(scope);

        // Definitions of variables that were declared outside the scope
        // propogate out
        SymbolSet new_def;

        // Definitions propogate out of the scope if they were declared before
        // the scope.
        std::set_intersection(def.begin(), def.end(),
            old_decl.begin(), old_decl.end(),
            std::inserter(new_def, new_def.end()));

        decl = old_decl;
        def = new_def;
    }
    else
        ASTVisitor::visitScopeStmt(scope);
}

void SymbolCheck::visitIfStmt(std::shared_ptr<ASTIfStmt> ifStmt) {
    visitExpNode(ifStmt->getCond());

    // Treat branches as scopes
    SymbolSet old_decl = decl;
    SymbolSet old_def = def;

    visitStmtSeqNode(ifStmt->getTrueStmt());

    SymbolSet def_left = def;
    decl = old_decl;
    def = old_def;

    if (ifStmt->getFalseStmt())
        visitStmtSeqNode(ifStmt->getFalseStmt());

    SymbolSet def_right = def;

    // Definitions of variables that were declared outside the if statement
    // and defined by BOTH branches propogate out
    SymbolSet both_def;
    SymbolSet new_def;

    std::set_intersection(
        def_left.begin(), def_left.end(),
        def_right.begin(), def_right.end(),
        std::inserter(both_def, both_def.end()));

    std::set_intersection(both_def.begin(), both_def.end(),
        old_decl.begin(), old_decl.end(),
        std::inserter(new_def, new_def.end()));

    decl = old_decl;
    def = new_def;
}

void SymbolCheck::visitWhileStmt(std::shared_ptr<ASTWhileStmt> whileStmt) {
    // Treat body as scope
    SymbolSet old_decl = decl;
    SymbolSet old_def = def;

    ASTVisitor::visitWhileStmt(whileStmt);

    // Definitions and declarations inside the body of the loop do NOT propagate out
    decl = old_decl;
    def = old_def;
}

void SymbolCheck::visitForStmt(std::shared_ptr<ASTForStmt> forStmt) {
    // Treat body as scope
    SymbolSet old_decl = decl;
    SymbolSet old_def = def;

    ASTVisitor::visitForStmt(forStmt);
    
    // Definitions and declarations inside the body of the loop do NOT propagate out
    decl = old_decl;
    def = old_def;

}

void SymbolCheck::visitRangeForStmt(std::shared_ptr<ASTRangeForStmt> rangeStmt) {
    // Check the condtion
    visitNode(rangeStmt->getRange());

    // Treat body as scope
    SymbolSet old_decl = decl;
    SymbolSet old_def = def;

    std::string newName = makeUniqueSymbol(rangeStmt->getIteratorId(), true);
    rangeStmt->setIteratorId(newName);

    visitStmtSeqNode(rangeStmt->getBody());

    // Definitions and declarations inside the body of the loop do NOT propagate out
    decl = old_decl;
    def = old_def;
}

void SymbolCheck::visitFunDecl(std::shared_ptr<ASTFunDecl> funDecl) {
    // Note: We process forward declarations exactly the same way as actual
    // definitions. This ensures that the argument symbols get renamed in whichever
    // declaration was used for the FunctionInfo the same way as they will be renamed
    // now.

    decl.clear();
    def.clear();
    rename.clear();
    symbolCounter = 0;

    // Mark all arguments as declared and defined.
    std::shared_ptr<ASTArgSeqNode> args = funDecl->getSignature()->getArgs();

    while (args != nullptr) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();

        Symbol uniqueName = makeUniqueSymbol(arg->getName(), true);
        arg->setName(uniqueName);

        args = args->getTail();
    }

    ASTVisitor::visitFunDecl(funDecl);
}

}
