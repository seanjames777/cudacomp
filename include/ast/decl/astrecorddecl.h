/**
 * @file astrecorddecl.h
 *
 * @brief Record declaration/definition top-level abstract syntax tree node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTRECORDDECL_H
#define __ASTRECORDDECL_H

#include <ast/decl/astdeclnode.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/type/astargnode.h>
#include <ast/type/astrecordtype.h>

/**
 * @brief Record declaration/definition top-level AST node
 *
 */
class ASTRecordDecl : public ASTDeclNode {
private:

    std::string name;
    std::shared_ptr<ASTRecordType> sig;

public:

    /**
     * @brief Constructor
     *
     * @param[in] name Record type name
     * @param[in] sig  Record signature
     * @param[in] defn Is this record was defined or just declared
     */
    ASTRecordDecl(std::string name, std::shared_ptr<ASTRecordType> sig, bool defn);

    /**
     * @brief Destructor
     */
    ~ASTRecordDecl();

    /**
     * @brief Get record name
     */
    std::string getName();
    
    /**
     * @brief Get if this record is defined
     */
    bool isDefn();

    /**
     * @brief Get record type/signature
     */
    std::shared_ptr<ASTRecordType> getSignature();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
