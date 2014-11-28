/**
 * @file astrangeforstmt.h
 *
 * @brief Abstract syntax tree 'for' loop over a range
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTRANGEFORSTMT_H
#define __ASTRANGEFORSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/type/asttypenode.h>
#include <ast/expr/astexpnode.h>

class ASTRangeForStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTTypeNode> type;
    std::string id;
    std::shared_ptr<ASTExpNode> range;
    std::shared_ptr<ASTStmtSeqNode> body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] type  Iterator variable type
     * @param[in] id    Iterator variable ID
     * @param[in] range Range to iterate over
     * @param[in] body  Statement to evaluate for each iteration
     */
    ASTRangeForStmt(
        std::shared_ptr<ASTTypeNode> type,
        std::string id,
        std::shared_ptr<ASTExpNode> range,
        std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @brief Destructor
     */
    ~ASTRangeForStmt();

    /**
     * @brief Get iterator variable type
     */
    std::shared_ptr<ASTTypeNode> getIteratorType();

    /**
     * @brief Set iterator variable type
     */
    void setIteratorType(std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Get iterator variable name
     */
    std::string getIteratorId();

    /**
     * @brief Set iterator variable name
     */
    void setIteratorId(std::string id);

    /**
     * @brief Get range expression
     */
    std::shared_ptr<ASTExpNode> getRange();

    /**
     * @brief Get statement sequence to evaluate in a loop while the condition is true
     */
    std::shared_ptr<ASTStmtSeqNode> getBody();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
