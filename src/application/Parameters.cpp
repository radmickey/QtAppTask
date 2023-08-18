#include "Parameters.h"

Parameter::Parameter(TYPE_PARAMETER type, QString name, QString label) : type_(type), name_(std::move(name)), label_(std::move(label)) {}

TYPE_PARAMETER Parameter::GetType() const {
    return type_;
}

const QString& Parameter::GetName() const {
    return name_;
}

const QString& Parameter::GetLabel() const {
    return label_;
}

LineEditParameter::LineEditParameter(QString name, QString original_text, QString data, QString label) :
        Parameter(TYPE_PARAMETER::LineEdit, std::move(name), std::move(label)),
        original_text_(std::move(original_text)), data_(std::move(data)), modified_data_(data_) {}

bool LineEditParameter::isChanged() {
    return data_ != modified_data_;
}

const QString& LineEditParameter::GetOriginalText() const {
    return original_text_;
}

const QString& LineEditParameter::GetValue() const {
    return data_;
}

void LineEditParameter::SetValue(QString data) {
    data_ = std::move(data);
}

const QString& LineEditParameter::GetModifiedValue() const {
    return modified_data_;
}

void LineEditParameter::SetModifiedValue(QString data) {
    modified_data_ = std::move(data);
}

CheckBoxParameter::CheckBoxParameter(QString name, QString label, bool status) :
        Parameter(TYPE_PARAMETER::CheckBox, std::move(name),std::move(label)),
                status_(status), modified_status_(status) {}

CheckBoxParameter::CheckBoxParameter(const CheckBoxParameter& other) :
        Parameter(TYPE_PARAMETER::CheckBox, other.GetName(), other.GetLabel()),
                status_(other.GetValue()), modified_status_(status_) {}

bool CheckBoxParameter::isChanged() {
    return status_ != modified_status_;
}

bool CheckBoxParameter::GetValue() const {
    return status_;
}

void CheckBoxParameter::SetValue(bool status) {
    status_ = status;
}

bool CheckBoxParameter::GetModifiedValue() const {
    return modified_status_;
}

void CheckBoxParameter::SetModifiedValue(bool status) {
    modified_status_ = status;
}
