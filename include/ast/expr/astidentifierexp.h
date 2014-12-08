/**
 * @file astidentifierexp.h
 *
 * @brief Abstract syntax tree identifier node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIDENTIFIEREXP_H
#define __ASTIDENTIFIEREXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>
#include <string>

/**
 * @brief AST identifier expression nodes. Refers to a local variable in a
 * function body.
 */
class ASTIdentifierExp : public ASTExpNode {
private:

    std::string value;
    bool lvalue;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Local variable name
     */
    ASTIdentifierExp(std::string value);

    /**
     * @brief Destructor
     */
    ~ASTIdentifierExp();

    /**
     * @brief Get the referenced variable name
     */
    std::string getId();

    /**
     * @brief Set the references variable name
     */
    void setId(std::string id);

    /**
     * @brief Get whether this is an lvalue. Unless setIsLValue() is called, this
     * will always return false. SymbolCheck uses this method.
     */
    bool isLValue();

    /**
     * @brief Set whether this is an lvalue.
     */
    void setIsLValue(bool lvalue);

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
