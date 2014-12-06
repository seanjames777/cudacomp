/**
 * @file astbinopexp.h
 *
 * @brief Abstract syntax tree binary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBINOPEXP_H
#define __ASTBINOPEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>
#include <ast/type/asttypenode.h>

/**
 * @brief Binary operator expression AST node
 */
class ASTBinopExp : public ASTExpNode {
public:

    /**
     * @brief Binary operations
     */
    enum op {
        NONE, //!< No operation
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
    std::shared_ptr<ASTExpNode> e1;
    std::shared_ptr<ASTExpNode> e2;
    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] op Binary operation
     * @param[in] e1 Left-hand expression
     * @param[in] e2 Right-hand expression
     */
    ASTBinopExp(enum op op, std::shared_ptr<ASTExpNode> e1, std::shared_ptr<ASTExpNode> e2);

    /**
     * @brief Destructor
     */
    ~ASTBinopExp();

    /**
     * @brief Get binary operation
     */
    enum op getOp();

    /**
     * @brief Get left-hand expression
     */
    std::shared_ptr<ASTExpNode> getE1();

    /**
     * @brief Get right-hand expression
     */
    std::shared_ptr<ASTExpNode> getE2();

    /**
     * @brief Utility for printing operators to a stream
     *
     * @param[in] op Operator to print
     * @param[in] ss Stream to print to
     */
    static void printBinop(enum ASTBinopExp::op op, std::ostream & ss);

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
