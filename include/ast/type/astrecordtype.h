/**
 * @file aststructtype.h
 *
 * @brief Abstract syntax tree function type node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTRECORDTYPE_H
#define __ASTRECORDTYPE_H

#include <ast/type/asttypenode.h>
#include <ast/type/astargnode.h>

/**
 * @brief Record type AST type node
 */
class ASTRecordType : public ASTTypeNode {
private:

    std::string id;
    std::shared_ptr<ASTArgSeqNode> fields;
    //TODO: add map of offsets (string to int)

public:

    /**
     * @brief Constructor
     *
     * @param[in] id         The type name
     * @param[in] fields     Field names and types sequence
     */
    ASTRecordType(std::string id, std::shared_ptr<ASTArgSeqNode> fields);

    /**
     * @brief Get the name of the stuct
     */
    std::string getId();

    /**
     * @brief Get field name and types sequence
     */
    std::shared_ptr<ASTArgSeqNode> getFields();

    /**
     * @brief Sets the field name and type sequences
     */
    void setFields(std::shared_ptr<ASTArgSeqNode> fields);

    /**
     * @brief Check if this type is structurally equal to another type. Names
     * of arguments are not considered: only argument types and return type.
     */
    virtual bool equal(std::shared_ptr<ASTTypeNode> other) override;

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

    /**
     * @copydoc ASTTypeNode::getSize()
     */
    virtual int getSize() override;

};

#endif
