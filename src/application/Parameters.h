#pragma once

#include <utility>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>

enum TYPE_PARAMETER {
    LineEdit,
    CheckBox,
    SpinBox,
    DoubleSpinBox
};

class Parameter {
    TYPE_PARAMETER type_;
    QString name_;
    QString label_;
public:
    Parameter(TYPE_PARAMETER type, QString name, QString label) : type_(type), name_(std::move(name)), label_(std::move(label)) {}

    virtual ~Parameter() = default;

    virtual bool isChanged() = 0;

    TYPE_PARAMETER GetType() const {
        return type_;
    }

    QString GetName() const {
        return name_;
    }

    QString GetLabel() const {
        return label_;
    }
};

class LineEditParameter : public Parameter {
    QString original_text_;
    QString data_;

    QString modified_data_;

public:
    LineEditParameter(QString original_text, QString data, QString name, QString label) :
            Parameter(TYPE_PARAMETER::LineEdit, std::move(name), std::move(label)),
            original_text_(std::move(original_text)), data_(std::move(data)), modified_data_(data_) {}
    bool isChanged() override {
        return data_!= modified_data_;
    }

    QString GetOriginalText() const {
        return original_text_;
    }

    QString GetValue() const {
        return data_;
    }

    void SetValue(QString data) {
        data_ = std::move(data);
    }

    QString GetModifiedValue() const {
        return modified_data_;
    }

    void SetModifiedValue(QString data) {
        modified_data_ = std::move(data);
    }

};

class CheckBoxParameter : public Parameter {
    bool status_;
    bool modified_status_;
public:
    CheckBoxParameter(QString name, QString label, bool status = false) :
            Parameter(TYPE_PARAMETER::CheckBox, std::move(name),
                      std::move(label)), status_(status), modified_status_(status) {}

    bool isChanged() override {
        return status_!= modified_status_;
    }

    bool GetValue() const {
        return status_;
    }

    void SetValue(bool status) {
        status_ = status;
    }

    bool GetModifiedValue() const {
        return modified_status_;
    }

    void SetModifiedValue(bool status) {
        modified_status_ = status;
    }

};

template<typename T>
class SpinBoxParameter : public Parameter {
    T min_;
    T max_;
    T step_;
    T default_value_;
    T modified_value_;
public:
    SpinBoxParameter(TYPE_PARAMETER type, QString name, QString label, T min, T max, T start_value, T step) :
            Parameter(type, std::move(name), std::move(label)),
            min_(min), max_(max), step_(step), default_value_(start_value), modified_value_(start_value) {}

    bool isChanged() override {
        return default_value_ != modified_value_;
    }

    void SetValue(T default_value) {
        default_value_ = default_value;
    }

    void SetModifiedValue(T modified_value) {
        modified_value_ = modified_value;
    }

    T GetModifiedValue() const {
        return modified_value_;
    }

    T GetMin() const {
        return min_;
    }

    T GetMax() const {
        return max_;
    }

    T GetStep() const {
        return step_;
    }

    T GetValue() const {
        return default_value_;
    }
};