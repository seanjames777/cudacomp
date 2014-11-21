/**
 * @file astfundefn.h
 *
 * @brief Top-level declaration or definition asbtract node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFUNDEFN_H
#define __ASTFUNDEFN_H

#include <ast/top/asttopnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astarg.h>
#include <ast/type/astfuntype.h>

class ASTFunDefn : public ASTTopNode {
private:

    std::string name;
    ASTFunType *sig;
    ASTStmtSeqNode *body;

public:

    ASTFunDefn(std::string name, ASTFunType *sig, ASTStmtSeqNode *body);

    ~ASTFunDefn();

    std::string getName();

    ASTFunType *getSignature();

    ASTStmtSeqNode *getBody();

    void setBody(ASTStmtSeqNode *body);

    virtual void print(std::ostream & ss) override;

};

#endif
