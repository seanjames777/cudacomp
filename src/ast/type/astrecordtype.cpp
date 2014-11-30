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
    return "_struct_" + id;
}

std::shared_ptr<ASTArgSeqNode> ASTRecordType::getFields() {
    return fields;
}

std::shared_ptr<ASTArgNode> ASTRecordType::getField(std::string id) {
    std::shared_ptr<ASTArgSeqNode> my_fields = fields;
    while (my_fields) {

        std::shared_ptr<ASTArgNode> field = my_fields->getHead();
        if (id.compare(field->getName()) == 0)
            return field;

        my_fields = my_fields->getTail();
    }
    return nullptr;
}

void ASTRecordType::setFields(std::shared_ptr<ASTArgSeqNode> f) {
    fields = f;
}

int ASTRecordType::getFieldIndex(std::string field_id) {
    int idx = 0;
    std::shared_ptr<ASTArgSeqNode> f = fields;
    while (f) {

        std::shared_ptr<ASTArgNode> field = f->getHead();

        if ((field_id.compare(field->getName())) == 0)
            return idx;

        f = f->getTail();
        idx++;
    }
    
    // TODO: raise an exception
    return -1;
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
    else
        return true;
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
    int numfields = 0;
    int sum = 0;
    while (field) {
        sum += field->getHead()->getType()->getSize();
        numfields++;
        field = field->getTail();
    }
    // Very generous size estimate. TODO: fix sizes by using datalayout.
    // ALSO this is not necessarily aligned so shit.
    return sum + 4 * numfields;
}
