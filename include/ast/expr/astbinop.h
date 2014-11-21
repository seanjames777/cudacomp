/**
 * @file astbinop.h
 *
 * @brief Abstract syntax tree binary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBINOP_H
#define __ASTBINOP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Binary operator expression AST node
 */
class ASTBinop : public ASTExpNode {
public:

    /**
     * @brief Binary operations
     */
    enum op {
        ADD,  //!< Addition
        SUB,  //!< Subtraction
        MUL,  //!< Multiplication
        DIV,  //!< Division
        MOD,  //!< Modulo
        SHL,  //!< Left-shift
        SHR,  //!< Right-shift
        AND,  //!< Boolean AND
        OR,   //!< Boolean OR
        BAND, //!< Bitwise AND
        BOR,  //!< Bitwise OR
        BXOR, //!< Bitwise XOR
        LT,   //!< Less-than
        GT,   //!< Greater-than
        LEQ,  //!< Less-than or equal-to
        GEQ,  //!< Greater-than or equal-to
        EQ,   //!< Equal-to
        NEQ   //!< Not equal-to
    };

private:

    enum op op;
    ASTExpNode *e1;
    ASTExpNode *e2;

public:

    /**
     * @brief Constructor
     *
     * @param[in] op Binary operation
     * @param[in] e1 Left-hand expression
     * @param[in] e2 Right-hand expression
     */
    ASTBinop(enum op op, ASTExpNode *e1, ASTExpNode *e2);

    /**
     * @brief Destructor
     */
    ~ASTBinop();

    /**
     * @brief Get binary operation
     */
    enum op getOp();

    /**
     * @brief Get left-hand expression
     */
    ASTExpNode *getE1();

    /**
     * @brief Get right-hand expression
     */
    ASTExpNode *getE2();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
