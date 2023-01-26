#pragma once

#include <QString>
#include <QtWidgets/QMainWindow>
#include "ui_Widgets_Test.h"

// Blog: https://blog.csdn.net/fengbingchun/article/details/128727514

class Widgets_Test : public QMainWindow
{
    Q_OBJECT

public:
    Widgets_Test(QWidget *parent = nullptr);
    ~Widgets_Test();

signals:
    void get_address(const QString& str);

private slots:
    // 槽函数:on_<object name>_<signal name> : 自动关联,无需使用connect将信号与槽关联
    // 在Qt Creator中,打开ui,然后选中按钮,点击右键,然后选择"转到槽...",双击clicked(),会自动生成槽函数
    void on_csdn_address_clicked();
    void on_github_address_clicked();

    void set_address(const QString& str);

private:
    Ui::Widgets_TestClass ui;
};
