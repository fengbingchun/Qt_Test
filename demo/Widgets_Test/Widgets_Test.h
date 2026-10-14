#pragma once

#include <QMainWindow>
#include "ui_Widgets_Test.h"
#include "logger.hpp"
#include "camera_dialog.h"

class Widgets_Test : public QMainWindow
{
    Q_OBJECT

public:
    Widgets_Test(QWidget *parent = nullptr);
    ~Widgets_Test();

private:
    void show_camera();

    Ui::Widgets_TestClass ui_;
};

