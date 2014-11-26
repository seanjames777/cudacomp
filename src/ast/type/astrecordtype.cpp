/**
 * @file aststructtype.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/type/astrecordtype.h>


ASTRecordType::ASTRecordType(std::string id, std::shared_ptr<ASTArgSeqNode> fields)
    : id(id),
      fields(fields)
{
}

std::string ASTRecordType::getId() {
    return id;
}

std::shared_ptr<ASTArgSeqNode> ASTRecordType::getFields() {
    return fields;
}

void ASTRecordType::setFields(std::shared_ptr<ASTArgSeqNode> f) {
    fields = f;
}

bool ASTRecordType::equal(std::shared_ptr<ASTTypeNode> other_type) {
    if (other_type == nullptr)
        return false;

    std::shared_ptr<ASTRecordType> other = std::dynamic_pointer_cast<ASTRecordType>(other_type);

    if (other == nullptr)
        return false;

    std::string other_id = other->id;
    std::string my_id = id;

    if ((my_id.compare(other_id)) != 0)
        return false;

    std::shared_ptr<ASTArgSeqNode> my_fields = fields;
    std::shared_ptr<ASTArgSeqNode> other_fields = other->fields;

    while (true) {
        if (my_fields == nullptr && other_fields == nullptr)
            return true;
        else if ((my_fields == nullptr && other_fields != nullptr) || (my_fields != nullptr && other_fields == nullptr))
            return false;
        else if (!my_fields->getHead()->getType()->equal(other_fields->getHead()->getType()))
            return false;

        my_fields = my_fields->getTail();
        other_fields = other_fields->getTail();
    }
}

void ASTRecordType::print(std::ostream & ss) {
    ss << "struct ";

    ss << id;

    ss << "(";

    std::shared_ptr<ASTArgSeqNode> field = fields;

    while (field != nullptr) {
        field->print(ss);

        field = field->getTail();

        if (field != nullptr)
            ss << ", ";
    }

    ss << ")";
}

// TODO: size after alignment ? alignment TODO
int ASTRecordType::getSize() {
    std::shared_ptr<ASTArgSeqNode> field = fields;
    int sum = 0;
    while (field) {
        sum += field->getHead()->getType()->getSize();
        field = field->getTail();
    }
    return sum;
}
