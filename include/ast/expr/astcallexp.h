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
#include <string>

/**
 * @brief Function call expression AST node
 */
class ASTCallExp : public ASTExpNode {
private:

    std::string id;
    std::shared_ptr<ASTExpSeqNode> dim_args;
    std::shared_ptr<ASTExpSeqNode> args;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id       Name of function to call
     * @param[in] dim_args Sequence of dimensional argument expressions
     * @param[in] args     Sequence of argument expressions
     */
    ASTCallExp(
        std::string id,
        std::shared_ptr<ASTExpSeqNode> dim_args,
        std::shared_ptr<ASTExpSeqNode> args);

    /**
     * @brief Destructor
     */
    ~ASTCallExp();

    /**
     * @brief Get name of function to call
     */
    std::string getId();

    /**
     * @brief Set name of function to call
     */
    void setId(std::string id);

    /**
     * @brief Get argument sequence to function call
     */
    std::shared_ptr<ASTExpSeqNode> getDimArgs();

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
