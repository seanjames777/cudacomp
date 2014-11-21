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

/**
 * @brief Function call expression AST node
 */
class ASTCall : public ASTExpNode {
private:

    std::string id;
    ASTExpSeqNode *args;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id   Name of function to call
     * @param[in] args Sequence of argument expressions
     */
    ASTCall(std::string id, ASTExpSeqNode *args);

    /**
     * @brief Destructor
     */
    ~ASTCall();

    /**
     * @brief Get name of function to call
     */
    std::string getId();

    /**
     * @brief Get argument sequence to function call
     */
    ASTExpSeqNode *getArgs();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
