/**
 * @file astcall.h
 *
 * @brief Abstract syntax tree binary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTCALL_H
#define __ASTCALL_H

#include <ast/expr/astexpnode.h>

class ASTCall : public ASTExpNode {
private:

    std::string id;
    ASTExpSeqNode *args;

public:

    ASTCall(std::string id, ASTExpSeqNode *args);

    ~ASTCall();

    std::string getId();

    ASTExpSeqNode *getArgs();

    void print(std::ostream & ss) override;

};

#endif


// TODO: need to create functions before generating their bodies... ugh
