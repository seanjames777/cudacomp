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
    std::shared_ptr<ASTStmtSeqNode> body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] name Function name
     * @param[in] sig  Function signature
     * @param[in] body Function body statement sequence
     */
    ASTRecordDecl(std::string name, std::shared_ptr<ASTRecordType> sig, std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @brief Destructor
     */
    ~ASTRecordDecl();

    /**
     * @brief Get record name
     */
    std::string getName();

    /**
     * @brief Get record type/signature
     */
    std::shared_ptr<ASTRecordType> getSignature();

    /**
     * @brief Get record body statement sequence
     */
    std::shared_ptr<ASTStmtSeqNode> getBody();

    /**
     * @brief Set the record body statement sequence
     */
    void setBody(std::shared_ptr<ASTStmtSeqNode> body);

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
