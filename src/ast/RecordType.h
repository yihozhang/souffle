/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2014, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file RecordType.h
 *
 * Defines the record type class
 *
 ***********************************************************************/

#pragma once

#include "ast/Attribute.h"
#include "ast/Node.h"
#include "ast/QualifiedName.h"
#include "ast/Type.h"
#include "parser/SrcLocation.h"
#include "souffle/utility/ContainerUtil.h"
#include "souffle/utility/StreamUtil.h"
#include "souffle/utility/tinyformat.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace souffle::ast {

/**
 * @class RecordType
 * @brief Record type class for record type declarations
 *
 * A record type aggregates a list of fields (i.e. name & type) into a new type.
 * Each record type has a name making the record type unique.
 */
class RecordType : public Type {
public:
    RecordType(QualifiedName name, VecOwn<Attribute> fields, SrcLocation loc = {})
            : Type(std::move(name), std::move(loc)), fields(std::move(fields)) {}

    /** Add field to record type */
    void add(std::string name, QualifiedName type) {
        fields.push_back(mk<Attribute>(std::move(name), std::move(type)));
    }

    /** Get fields of record */
    std::vector<Attribute*> getFields() const {
        return toPtrVector(fields);
    }

    /** Set field type */
    void setFieldType(size_t idx, QualifiedName type) {
        fields.at(idx)->setTypeName(std::move(type));
    }

    RecordType* clone() const override {
        return new RecordType(getQualifiedName(), souffle::clone(fields), getSrcLoc());
    }

protected:
    void print(std::ostream& os) const override {
        os << tfm::format(".type %s = [%s]", getQualifiedName(), join(fields, ", "));
    }

    bool equal(const Node& node) const override {
        const auto& other = dynamic_cast<const RecordType&>(node);
        return getQualifiedName() == other.getQualifiedName() && equal_targets(fields, other.fields);
    }

private:
    /** record fields */
    VecOwn<Attribute> fields;
};

}  // namespace souffle::ast
