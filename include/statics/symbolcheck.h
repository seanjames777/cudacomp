/**
 * @file symbolcheck.h
 *
 * @brief Symbol statics checker
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/ast.h>
#include <statics/symboltable.h>
#include <ast/astvisitor.h>
#include <statics/exceptions.h>

namespace Statics {

/**
 * @brief Ensures that symbols in function bodies are declared and defined before they
 * are used, and renames symbols to be unique function-wide.
 */
class SymbolCheck : public ASTVisitor {
private:

    SymbolSet decl;
    SymbolSet def;
    SymbolTable<Symbol> rename;
    int symbolCounter;

    Symbol makeUniqueSymbol(Symbol original, bool defined);

public:

    SymbolCheck();

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    virtual void visitIdentifierExp(std::shared_ptr<ASTIdentifierExp> id) override;

    virtual void visitCallExp(std::shared_ptr<ASTCallExp> call) override;

    virtual void visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) override;

    virtual void visitAssignStmt(std::shared_ptr<ASTAssignStmt> assign) override;

    virtual void visitReturnStmt(std::shared_ptr<ASTReturnStmt> ret) override;

    virtual void visitScopeStmt(std::shared_ptr<ASTScopeStmt> scope) override;

    virtual void visitIfStmt(std::shared_ptr<ASTIfStmt> ifStmt) override;

    virtual void visitWhileStmt(std::shared_ptr<ASTWhileStmt> whileStmt) override;

    virtual void visitForStmt(std::shared_ptr<ASTForStmt> forStmt) override;

    virtual void visitFunDecl(std::shared_ptr<ASTFunDecl> funDecl) override;

};

}
