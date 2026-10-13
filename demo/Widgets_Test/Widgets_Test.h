#pragma once

#include <QMainWindow>
#include "ui_Widgets_Test.h"
#include "logger.hpp"
#include "camera_dialog.h"

/* Blog:
* https://blog.csdn.net/fengbingchun/article/details/128727514
* https://blog.csdn.net/fengbingchun/article/details/146457607
*/

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

