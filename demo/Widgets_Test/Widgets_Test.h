#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "ui_Widgets_Test.h"
#include "logger.hpp"
#include "camera_dialog.h"

class Widgets_Test : public QMainWindow
{
    Q_OBJECT

public:
    Widgets_Test(QWidget *parent = nullptr);
    ~Widgets_Test();

private slots:
    void update_datetime();

private:
    void show_camera();

    Ui::Widgets_TestClass ui_;
    QLabel* label_datetime_{ nullptr };
    QTimer* timer_{ nullptr };
};

