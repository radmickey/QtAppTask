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
#include <vector>
#include <sstream>

class MainWindow {
private:
    QString config_file_;

    QVBoxLayout* main_layout_;

    QPushButton* save_button_;
    QWidget* main_window_;

    static const int kWeightMainWindow = 800;
    static const int kHeightMainWindow = 600;

    const QString kNameMainWindow = "Satellite App";

    std::vector<Parameter*> parameters;

public:
    explicit MainWindow(QWidget* parent = nullptr, QString config_file = "config.json");

    ~MainWindow();

    void setDefaultValues();

    void saveConfig(bool is_save_button = false);

    void show();

    void loadConfig();

    void createWidgets();
};
