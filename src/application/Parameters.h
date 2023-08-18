#pragma once

#include <QString>

enum class TYPE_PARAMETER {
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
    Parameter(TYPE_PARAMETER type, QString name, QString label);

    virtual ~Parameter() = default;

    virtual bool isChanged() = 0;

    TYPE_PARAMETER GetType() const;

    const QString& GetName() const;

    const QString& GetLabel() const;
};

class LineEditParameter : public Parameter {
    QString original_text_;
    QString data_;
    QString modified_data_;

public:
    LineEditParameter(QString name, QString original_text, QString data, QString label);

    LineEditParameter(const LineEditParameter& other) : Parameter(TYPE_PARAMETER::LineEdit, other.GetName(), other.GetLabel()),
                                                        original_text_(other.GetOriginalText()), data_(other.GetValue()), modified_data_(data_) {}

    bool isChanged() override;

    const QString& GetOriginalText() const;

    const QString& GetValue() const;

    void SetValue(QString data);

    const QString& GetModifiedValue() const;

    void SetModifiedValue(QString data);
};

class CheckBoxParameter : public Parameter {
    bool status_;
    bool modified_status_;
public:
    CheckBoxParameter(QString name, QString label, bool status = false);

    CheckBoxParameter(const CheckBoxParameter& other);

    bool isChanged() override;

    bool GetValue() const;

    void SetValue(bool status);

    bool GetModifiedValue() const;

    void SetModifiedValue(bool status);

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

    SpinBoxParameter(const SpinBoxParameter& other) : Parameter(other.GetType(), other.GetName(), other.GetLabel()),
            min_(other.GetMin()), max_(other.GetMax()), step_(other.GetStep()),
            default_value_(other.GetValue()), modified_value_(other.GetModifiedValue()) {}

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
