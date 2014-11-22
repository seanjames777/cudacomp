/**
 * @file astcallexp.h
 *
 * @brief Abstract syntax tree binary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTCALLEXP_H
#define __ASTCALLEXP_H

#include <ast/expr/astexpnode.h>

/**
 * @brief Function call expression AST node
 */
class ASTCallExp : public ASTExpNode {
private:

    std::string id;
    std::shared_ptr<ASTExpSeqNode> args;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id   Name of function to call
     * @param[in] args Sequence of argument expressions
     */
    ASTCallExp(std::string id, std::shared_ptr<ASTExpSeqNode> args);

    /**
     * @brief Destructor
     */
    ~ASTCallExp();

    /**
     * @brief Get name of function to call
     */
    std::string getId();

    /**
     * @brief Get argument sequence to function call
     */
    std::shared_ptr<ASTExpSeqNode> getArgs();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
