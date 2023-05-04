#pragma once

#include "Parameters.h"

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
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>

class MainWindow {
private:
    QString config_file_;

    QVBoxLayout* main_layout_;

    QPushButton* save_button_;
    QWidget* main_window_;

    std::unordered_map<QWidget*, Parameter*> edit_widgets_map_;

    static const int kWeightMainWindow = 800;
    static const int kHeightMainWindow = 600;

    const QString kNameMainWindow = "Satellite App";

    std::vector<Parameter*> parameters;

public:
    explicit MainWindow(QWidget* parent = nullptr, QString config_file = "config.json") : config_file_(std::move(config_file)) {
        main_window_ = new QWidget(parent);
        main_layout_ = new QVBoxLayout();
        save_button_ = new QPushButton("Save");

        main_window_->resize(kWeightMainWindow, kHeightMainWindow);
        main_window_->setWindowTitle(kNameMainWindow);

        setDefaultValues();

        loadConfig();

        createWidgets();

    }

    ~MainWindow() {
        saveConfig();

        for (auto& parameter : parameters) {
            delete parameter;
        }

        delete save_button_;
        delete main_window_;
    }

    void setDefaultValues() {
        const int kNoradIdMin = 10000;
        const int kNoradIdMax = 99999;
        const int kNoradIdStep = 1;
        const int kNoradIdDefault = 10000;

        const double kAltitudeMin = 160.0;
        const double kAltitudeMax = std::numeric_limits<double>::max();
        const double kAltitudeStep = 0.01;
        const double kAltitudeDefault = 160.0;

        const double kInclinationMin = 0.0;
        const double kInclinationMax = 360.0;
        const double kInclinationStep = 0.01;
        const double kInclinationDefault = 0.0;

        const double kApogeeMin = 1000.0;
        const double kApogeeMax = std::numeric_limits<double>::max();
        const double kApogeeStep = 0.01;
        const double kApogeeDefault = 1000.0;

        const double kPerigeeMin = 600.0;
        const double kPerigeeMax = std::numeric_limits<double>::max();
        const double kPerigeeStep = 0.01;
        const double kPerigeeDefault = 600.0;

        const double kEccentricityMin = 0.0;
        const double kEccentricityMax = std::numeric_limits<double>::max();
        const double kEccentricityStep = 0.01;
        const double kEccentricityDefault = 0.0;

        const double kArgPerigeeMin = 0.0;
        const double kArgPerigeeMax = 360.0;
        const double kArgPerigeeStep = 0.01;
        const double kArgPerigeeDefault = 0.0;

        const double kMeanAnomalyMin = 0.0;
        const double kMeanAnomalyMax = 360.0;
        const double kMeanAnomalyStep = 0.01;
        const double kMeanAnomalyDefault = 0.0;


        std::vector<std::vector<QString>> lineEditParams = {
                {"Write a satellite name...", "Sat1", "satelliteName", "Satellite Name:"},
                {"Write a satellite model...", "Model A", "satelliteModel", "Model:"},
                {"Write a country code...", "RU", "countryCode", "Country Code:"},
                {"Write a satellite description", "Satellite for Earth observation", "description", "Description:"},
        };

        std::vector<std::vector<QString>> IntSpinBoxParams = {
                {"noradId", "NORAD ID:"}
        };

        std::vector<std::vector<QString>> DoubleSpinBoxParams = {
                { "altitude", "Altitude (km):"},
                { "inclination", "Inclination (deg):"},
                { "apogee", "Apogee (km):"},
                { "perigee", "Perigee (km):"},
                { "eccentricity", "Eccentricity:"},
                { "argPerigee", "Argument of Perigee (deg):"},
                { "meanAnomaly", "Mean Anomaly (deg):"}
        };

        std::vector<std::vector<QString>> CheckBoxParams = {
                {"status", "Status (on/off)"}
        };


        for (const auto& param : lineEditParams) {
            parameters.push_back(new LineEditParameter(param[0], param[1], param[2], param[3]));
        }

        for (const auto& param : IntSpinBoxParams) {
            if (param[0] == "noradId") {
                parameters.push_back(new SpinBoxParameter<int>(TYPE_PARAMETER::SpinBox, param[0],
                                                               param[1], kNoradIdMin, kNoradIdMax,
                                                               kNoradIdDefault, kNoradIdStep));
            }
        }

        for (const auto& param : DoubleSpinBoxParams) {
            if (param[0] == "altitude") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kAltitudeMin, kAltitudeMax,
                                                                  kAltitudeDefault, kAltitudeStep));
            } else if (param[0] == "inclination") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kInclinationMin, kInclinationMax,
                                                                  kInclinationDefault, kInclinationStep));
            } else if (param[0] == "apogee") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kApogeeMin, kApogeeMax,
                                                                  kApogeeDefault, kApogeeStep));
            } else if (param[0] == "perigee") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kPerigeeMin, kPerigeeMax,
                                                                  kPerigeeDefault, kPerigeeStep));
            } else if (param[0] == "eccentricity") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kEccentricityMin, kEccentricityMax,
                                                                  kEccentricityDefault, kEccentricityStep));
            } else if (param[0] == "argPerigee") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kArgPerigeeMin, kArgPerigeeMax,
                                                                  kArgPerigeeDefault, kArgPerigeeStep));
            } else if (param[0] == "meanAnomaly") {
                parameters.push_back(new SpinBoxParameter<double>(TYPE_PARAMETER::DoubleSpinBox, param[0],
                                                                  param[1], kMeanAnomalyMin, kMeanAnomalyMax,
                                                                  kMeanAnomalyDefault, kMeanAnomalyStep));
            }
        }

        for (const auto& param : CheckBoxParams) {
            parameters.push_back(new CheckBoxParameter(param[0], param[1]));
        }
    }

    void saveConfig(bool is_save_button = false) {
        std::string text_to_save;

        QJsonObject jsonObj;

        for (auto & parameter : parameters) {
            QString name = parameter->GetName();
            switch (parameter->GetType()) {
                case LineEdit: {
                    QString value = dynamic_cast<LineEditParameter*>(parameter)->GetModifiedValue();
                    QString default_value = dynamic_cast<LineEditParameter*>(parameter)->GetValue();
                    jsonObj[name] = value;
                    if (parameter->isChanged()) {
                        dynamic_cast<LineEditParameter*>(parameter)->SetValue(value);
                        text_to_save += name.toStdString() + " has been changed from " + default_value.toStdString() +
                                        " to " + value.toStdString() + "\n";
                    }
                    break;
                }
                case CheckBox: {
                    jsonObj[name] = dynamic_cast<CheckBoxParameter*>(parameter)->GetModifiedValue();
                    bool status = dynamic_cast<CheckBoxParameter*>(parameter)->GetModifiedValue();
                    bool default_status = dynamic_cast<CheckBoxParameter*>(parameter)->GetValue();
                    if (parameter->isChanged()) {
                        dynamic_cast<CheckBoxParameter*>(parameter)->SetValue(status);
                        text_to_save += name.toStdString() + " has been changed from " +
                                        (default_status ? "true" : "false") +
                                        " to " +
                                        (status ? "true" : "false")
                                        + "\n";
                    }
                    break;
                }
                case SpinBox: {
                    int value = dynamic_cast<SpinBoxParameter<int>*>(parameter)->GetModifiedValue();
                    int default_value = dynamic_cast<SpinBoxParameter<int>*>(parameter)->GetValue();

                    jsonObj[name] = value;
                    if (parameter->isChanged()) {
                        dynamic_cast<SpinBoxParameter<int>*>(parameter)->SetValue(value);
                        text_to_save += name.toStdString() + " has been changed from " +
                                        std::to_string(default_value) + " to " +
                                        std::to_string(value) + "\n";
                    }
                    break;
                }
                case DoubleSpinBox: {
                    double value = dynamic_cast<SpinBoxParameter<double>*>(parameter)->GetModifiedValue();
                    double default_value = dynamic_cast<SpinBoxParameter<double>*>(parameter)->GetValue();

                    jsonObj[name] = value;

                    if (parameter->isChanged()) {
                        dynamic_cast<SpinBoxParameter<double>*>(parameter)->SetValue(value);
                        text_to_save += name.toStdString() + " has been changed from " +
                                        std::to_string(default_value) + " to " +
                                        std::to_string(value) + "\n";
                    }
                    break;
                }
            }
        }

        if (text_to_save.empty()) {
            text_to_save = "No parameters have been changed.\n";
        } else {

            QJsonDocument jsonDoc(jsonObj);

            QFile file(config_file_);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qWarning("Could not open file for writing.");
                return;
            }

            file.write(jsonDoc.toJson());
            file.close();
        }

        if (is_save_button) {
            QMessageBox::information(nullptr, "Saving...", QString::fromStdString(text_to_save));
        }

    }

    void show()  {
        main_window_->show();
    }

    void loadConfig() {
        QFile file(config_file_);

        if (!file.open(QIODevice::ReadOnly)) {
            // Файл не найден, используем значения по умолчанию
            return;
        }

        // Читаем содержимое файла и сохраняем его в QByteArray
        QByteArray data = file.readAll();
        file.close();

        // Распарсиваем JSON-строку в объект QVariant
        QJsonParseError error{};
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (doc.isNull()) {
            // Ошибка при парсинге JSON-строки, используем значения по умолчанию
            return;
        }

        QJsonObject root = doc.object();
        for (auto& parameter : parameters) {
            switch (parameter->GetType()) {
                case LineEdit: {
                    if (root.contains(parameter->GetName())) {
                        QVariant value = root.value(parameter->GetName());
                        dynamic_cast<LineEditParameter *>(parameter)->SetValue(value.toString());
                        dynamic_cast<LineEditParameter *>(parameter)->SetModifiedValue(value.toString());
                    }
                    break;
                }
                case CheckBox: {
                    if (root.contains(parameter->GetName())) {
                        QVariant value = root.value(parameter->GetName());
                        dynamic_cast<CheckBoxParameter*>(parameter)->SetValue(value.toBool());
                        dynamic_cast<CheckBoxParameter*>(parameter)->SetModifiedValue(value.toBool());
                    }
                    break;
                }
                case SpinBox: {
                    if (root.contains(parameter->GetName())) {
                        QVariant value = root.value(parameter->GetName());
                        dynamic_cast<SpinBoxParameter<int>*>(parameter)->SetValue(value.toInt());
                        dynamic_cast<SpinBoxParameter<int>*>(parameter)->SetModifiedValue(value.toInt());
                    }
                    break;
                }
                case DoubleSpinBox: {
                    if (root.contains(parameter->GetName())) {
                        QVariant value = root.value(parameter->GetName());
                        dynamic_cast<SpinBoxParameter<double>*>(parameter)->SetValue(value.toDouble());
                        dynamic_cast<SpinBoxParameter<double>*>(parameter)->SetModifiedValue(value.toDouble());
                    }
                    break;
                }
            }
        }
    }

    void createWidgets() {
        const int kWeightParameter = 700;
        const int kHeightParameter = 25;

        // Create and add parameters to m_params
        for (auto& parameter : parameters) {
            auto* label = new QLabel(parameter->GetLabel());
            QWidget* widget;
            switch (parameter->GetType()) {
                case LineEdit: {
                    widget = new QLineEdit(dynamic_cast<LineEditParameter *>(parameter)->GetValue());
                    label->setObjectName(parameter->GetName());
                    dynamic_cast<QLineEdit*>(widget)->setPlaceholderText(dynamic_cast<LineEditParameter*>(parameter)->GetOriginalText());

                    QObject::connect(dynamic_cast<QLineEdit*>(widget), &QLineEdit::editingFinished, [=](){
                        auto* lineEdit = dynamic_cast<QLineEdit*>(widget);
                        Parameter* parameter_edit = edit_widgets_map_[widget];
                        QString text = lineEdit->text();
                        dynamic_cast<LineEditParameter *>(parameter_edit)->SetModifiedValue(text);
                    });
                    break;
                }
                case CheckBox: {
                    widget = new QCheckBox();
                    dynamic_cast<QCheckBox*>(widget)->setChecked(
                            dynamic_cast<CheckBoxParameter *>(parameter)->GetValue());
                    QObject::connect(dynamic_cast<QCheckBox*>(widget), &QCheckBox::stateChanged, [=](){
                        auto* checkBox = dynamic_cast<QCheckBox*>(widget);
                        Parameter* parameter_edit = edit_widgets_map_[widget];
                        bool status = checkBox->checkState();
                        dynamic_cast<CheckBoxParameter *>(parameter_edit)->SetModifiedValue(status);
                    });

                    break;
                }
                case SpinBox: {
                    widget = new QSpinBox();
                    dynamic_cast<QSpinBox*>(widget)->setMinimum(dynamic_cast<SpinBoxParameter<int>*>(parameter)->GetMin());
                    dynamic_cast<QSpinBox*>(widget)->setMaximum(dynamic_cast<SpinBoxParameter<int>*>(parameter)->GetMax());
                    dynamic_cast<QSpinBox*>(widget)->setValue(
                            dynamic_cast<SpinBoxParameter<int> *>(parameter)->GetValue());
                    dynamic_cast<QSpinBox*>(widget)->setSingleStep(dynamic_cast<SpinBoxParameter<int>*>(parameter)->GetStep());

                    QObject::connect(dynamic_cast<QSpinBox*>(widget), QOverload<>::of(&QSpinBox::editingFinished), [=]() {
                        auto* spinBox = dynamic_cast<QSpinBox*>(widget);
                        Parameter* parameter_edit = edit_widgets_map_[widget];
                        dynamic_cast<SpinBoxParameter<int>*>(parameter_edit)->SetModifiedValue(spinBox->value());
                    });

                    break;
                }
                case DoubleSpinBox: {
                    widget = new QDoubleSpinBox();
                    dynamic_cast<QDoubleSpinBox*>(widget)->setMinimum(dynamic_cast<SpinBoxParameter<double>*>(parameter)->GetMin());
                    dynamic_cast<QDoubleSpinBox*>(widget)->setMaximum(dynamic_cast<SpinBoxParameter<double>*>(parameter)->GetMax());
                    dynamic_cast<QDoubleSpinBox*>(widget)->setValue(
                            dynamic_cast<SpinBoxParameter<double> *>(parameter)->GetValue());
                    dynamic_cast<QDoubleSpinBox*>(widget)->setSingleStep(dynamic_cast<SpinBoxParameter<double>*>(parameter)->GetStep());

                    QObject::connect(dynamic_cast<QDoubleSpinBox*>(widget), QOverload<>::of(&QDoubleSpinBox::editingFinished), [=]() {
                        auto* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(widget);
                        Parameter* parameter_edit = edit_widgets_map_[widget];
                        dynamic_cast<SpinBoxParameter<double>*>(parameter_edit)->SetModifiedValue(doubleSpinBox->value());
                    });

                    break;
                }
            }

            edit_widgets_map_[widget] = parameter;
            widget->setFixedSize(kWeightParameter, kHeightParameter);

            auto* inputLayout = new QHBoxLayout();
            inputLayout->addWidget(label);
            inputLayout->addWidget(widget);
            main_layout_->addLayout(inputLayout);
        }

        main_layout_->addWidget(save_button_, 0, Qt::AlignTop | Qt::AlignRight);
        main_window_->setLayout(main_layout_);

        QObject::connect(save_button_, &QPushButton::clicked, [&]() {
            saveConfig(true);
        });
    }
};
