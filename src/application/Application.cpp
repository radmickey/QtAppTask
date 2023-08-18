#include "Application.h"

std::string doubleToString(double value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

MainWindow::MainWindow(QWidget* parent, QString config_file) : config_file_(std::move(
    config_file)) {
    main_window_ = new QWidget(parent);
    main_layout_ = new QVBoxLayout();
    save_button_ = new QPushButton("Save");

    main_window_->resize(kWeightMainWindow, kHeightMainWindow);
    main_window_->setWindowTitle(kNameMainWindow);

    setDefaultValues();

    loadConfig();

    createWidgets();

}

MainWindow::~MainWindow() {
    saveConfig();

    for (auto& parameter : parameters) {
        delete parameter;
    }

    delete save_button_;
    delete main_window_;
}

void MainWindow::setDefaultValues() {
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

    std::vector<LineEditParameter> lineEditParameters = {
        {"satelliteName", "Write a satellite name...", "Sat1", "Satellite Name:"},
        {"satelliteModel", "Write a satellite model...", "Model A", "Model:"},
        {"countryCode", "Write a country code...", "RU", "Country Code:"},
        {"description", "Write a satellite description...", "Satellite for Earth observation", "Description:"},
    };

    std::vector<SpinBoxParameter<int>> IntSpinBoxParameters = {
        {TYPE_PARAMETER::SpinBox, "noradId", "NORAD ID:", kNoradIdMin, kNoradIdMax, kNoradIdDefault, kNoradIdStep}
    };

    std::vector<SpinBoxParameter<double>> DoubleSpinBoxParameters = {
        {TYPE_PARAMETER::DoubleSpinBox, "altitude", "Altitude (km):",
            kAltitudeMin, kAltitudeMax, kAltitudeDefault, kAltitudeStep},
        {TYPE_PARAMETER::DoubleSpinBox, "inclination", "Inclination (deg):",
            kInclinationMin, kInclinationMax, kInclinationDefault, kInclinationStep},
        {TYPE_PARAMETER::DoubleSpinBox, "apogee", "Apogee (km):",
            kApogeeMin, kApogeeMax, kApogeeDefault, kApogeeStep},
        {TYPE_PARAMETER::DoubleSpinBox, "perigee", "Perigee (km):",
            kPerigeeMin, kPerigeeMax, kPerigeeDefault, kPerigeeStep},
        {TYPE_PARAMETER::DoubleSpinBox, "eccentricity", "Eccentricity:",
            kEccentricityMin, kEccentricityMax, kEccentricityDefault, kEccentricityStep},
        {TYPE_PARAMETER::DoubleSpinBox, "argPerigee", "Argument of Perigee (deg):",
            kArgPerigeeMin, kArgPerigeeMax, kArgPerigeeDefault, kArgPerigeeStep},
        {TYPE_PARAMETER::DoubleSpinBox, "meanAnomaly", "Mean Anomaly (deg):",
            kMeanAnomalyMin, kMeanAnomalyMax, kMeanAnomalyDefault, kMeanAnomalyStep}
    };

    std::vector<std::vector<QString>> CheckBoxParams = {
        {"status", "Status (on/off)"}
    };

    std::vector<CheckBoxParameter> CheckBoxParameters = {
        {"status", "Status (on/off)", false}
    };

    for (const auto& parameter : lineEditParameters) {
        parameters.push_back(new LineEditParameter(parameter));
    }

    for (const auto& parameter : IntSpinBoxParameters) {
        parameters.push_back(new SpinBoxParameter<int>(parameter));
    }

    for (const auto& parameter : DoubleSpinBoxParameters) {
        parameters.push_back(new SpinBoxParameter<double>(parameter));
    }

    for (const auto& parameter : CheckBoxParameters) {
        parameters.push_back(new CheckBoxParameter(parameter));
    }
}

void MainWindow::saveConfig(bool is_save_button) {
    std::string text_to_save;

    QJsonObject jsonObj;

    for (auto& parameter : parameters) {
        QString name = parameter->GetName();
        switch (parameter->GetType()) {
            case TYPE_PARAMETER::LineEdit: {
                auto* line_edit_parameter = dynamic_cast<LineEditParameter*>(parameter);

                QString value = line_edit_parameter->GetModifiedValue();
                QString default_value = line_edit_parameter->GetValue();

                jsonObj[name] = value;
                if (line_edit_parameter->isChanged()) {
                    line_edit_parameter->SetValue(value);
                    text_to_save += name.toStdString() + " has been changed from " + default_value.toStdString() +
                                    " to " + value.toStdString() + "\n";
                }
                break;
            }
            case TYPE_PARAMETER::CheckBox: {
                auto* checkbox_parameter = dynamic_cast<CheckBoxParameter*>(parameter);
                bool status = checkbox_parameter->GetModifiedValue();
                bool default_status = checkbox_parameter->GetValue();

                jsonObj[name] = status;

                if (checkbox_parameter->isChanged()) {
                    checkbox_parameter->SetValue(status);
                    text_to_save += name.toStdString() + " has been changed from " +
                                    (default_status ? "true" : "false") +
                                    " to " +
                                    (status ? "true" : "false")
                                    + "\n";
                }
                break;
            }
            case TYPE_PARAMETER::SpinBox: {
                auto* int_spinbox_parameter = dynamic_cast<SpinBoxParameter<int>*>(parameter);
                int value = int_spinbox_parameter->GetModifiedValue();
                int default_value = int_spinbox_parameter->GetValue();

                jsonObj[name] = value;

                if (int_spinbox_parameter->isChanged()) {
                    int_spinbox_parameter->SetValue(value);
                    text_to_save += name.toStdString() + " has been changed from " +
                                    std::to_string(default_value) + " to " +
                                    std::to_string(value) + "\n";
                }
                break;
            }
            case TYPE_PARAMETER::DoubleSpinBox: {
                auto* double_spinbox_parameter = dynamic_cast<SpinBoxParameter<double>*>(parameter);
                double value = double_spinbox_parameter->GetModifiedValue();
                double default_value = double_spinbox_parameter->GetValue();

                jsonObj[name] = value;

                if (double_spinbox_parameter->isChanged()) {
                    double_spinbox_parameter->SetValue(value);
                    text_to_save += name.toStdString() + " has been changed from " +
                                doubleToString(default_value) + " to " +
                                doubleToString(value) + "\n";
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

void MainWindow::show() {
    main_window_->show();
}

void MainWindow::loadConfig() {
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
            case TYPE_PARAMETER::LineEdit: {
                if (root.contains(parameter->GetName())) {
                    auto* line_edit_parameter = dynamic_cast<LineEditParameter*>(parameter);
                    QVariant value = root.value(parameter->GetName());

                    line_edit_parameter->SetValue(value.toString());
                    line_edit_parameter->SetModifiedValue(value.toString());
                }
                break;
            }
            case TYPE_PARAMETER::CheckBox: {
                if (root.contains(parameter->GetName())) {
                    auto* checkbox_parameter = dynamic_cast<CheckBoxParameter*>(parameter);
                    QVariant value = root.value(parameter->GetName());

                    checkbox_parameter->SetValue(value.toBool());
                    checkbox_parameter->SetModifiedValue(value.toBool());
                }
                break;
            }
            case TYPE_PARAMETER::SpinBox: {
                if (root.contains(parameter->GetName())) {
                    auto* spin_box_parameter = dynamic_cast<SpinBoxParameter<int>*>(parameter);
                    QVariant value = root.value(parameter->GetName());

                    spin_box_parameter->SetValue(value.toInt());
                    spin_box_parameter->SetModifiedValue(value.toInt());
                }
                break;
            }
            case TYPE_PARAMETER::DoubleSpinBox: {
                if (root.contains(parameter->GetName())) {
                    auto* spin_box_parameter = dynamic_cast<SpinBoxParameter<double>*>(parameter);
                    QVariant value = root.value(parameter->GetName());

                    spin_box_parameter->SetValue(value.toDouble());
                    spin_box_parameter->SetModifiedValue(value.toDouble());
                }
                break;
            }
        }
    }
}

void MainWindow::createWidgets() {
    const int kWidthParameter = 700;
    const int kHeightParameter = 25;

    for (auto& parameter : parameters) {
        auto* label = new QLabel(parameter->GetLabel());
        QWidget* widget;
        TYPE_PARAMETER type = parameter->GetType();
        switch (type) {
            case TYPE_PARAMETER::LineEdit: {
                auto* line_edit_parameter = dynamic_cast<LineEditParameter*>(parameter);
                auto* line_edit_widget = new QLineEdit(line_edit_parameter->GetValue());
                widget = line_edit_widget;

                label->setObjectName(parameter->GetName());
                line_edit_widget->setPlaceholderText(line_edit_parameter->GetOriginalText());

                QObject::connect(line_edit_widget, &QLineEdit::editingFinished, [=]() {
                    QString text = line_edit_widget->text();

                    if (!text.isEmpty()) {
                        line_edit_parameter->SetModifiedValue(text);
                    } else {
                        line_edit_widget->setText(line_edit_parameter->GetValue());
                    }
                });
                break;
            }
            case TYPE_PARAMETER::CheckBox: {
                auto* checkbox_widget = new QCheckBox();
                auto* checkbox_parameter = dynamic_cast<CheckBoxParameter*>(parameter);
                widget = checkbox_widget;

                checkbox_widget->setChecked(checkbox_parameter->GetValue());

                QObject::connect(checkbox_widget, &QCheckBox::stateChanged, [=]() {
                    bool status = checkbox_widget->checkState();
                    checkbox_parameter->SetModifiedValue(status);
                });
                break;
            }
            case TYPE_PARAMETER::SpinBox: {
                auto* spin_box_widget = new QSpinBox();
                auto* int_spin_box_parameter = dynamic_cast<SpinBoxParameter<int>*>(parameter);
                widget = spin_box_widget;

                spin_box_widget->setMinimum(int_spin_box_parameter->GetMin());
                spin_box_widget->setMaximum(int_spin_box_parameter->GetMax());
                spin_box_widget->setValue(int_spin_box_parameter->GetValue());
                spin_box_widget->setSingleStep(int_spin_box_parameter->GetStep());

                QObject::connect(spin_box_widget, QOverload<>::of(&QSpinBox::editingFinished), [=]() {
                    int_spin_box_parameter->SetModifiedValue(spin_box_widget->value());
                });
                break;
            }
            case TYPE_PARAMETER::DoubleSpinBox: {
                auto* double_spin_box_widget = new QDoubleSpinBox();
                auto* double_spin_box_parameter = dynamic_cast<SpinBoxParameter<double>*>(parameter);
                widget = double_spin_box_widget;

                double_spin_box_widget->setMinimum(double_spin_box_parameter->GetMin());
                double_spin_box_widget->setMaximum(double_spin_box_parameter->GetMax());
                double_spin_box_widget->setValue(double_spin_box_parameter->GetValue());
                double_spin_box_widget->setSingleStep(double_spin_box_parameter->GetStep());

                QObject::connect(double_spin_box_widget, QOverload<>::of(&QDoubleSpinBox::editingFinished), [=]() {
                    double_spin_box_parameter->SetModifiedValue(double_spin_box_widget->value());
                });
                break;
            }
        }

        widget->setFixedSize(kWidthParameter, kHeightParameter);

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
