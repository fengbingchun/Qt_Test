#pragma once

#include <QString>
#include <QtWidgets/QMainWindow>
#include <QImage>
#include <QLabel>
#include <thread>
#include <atomic>
#include "ui_Widgets_Test.h"
#include "logger.hpp"

/* Blog:
* https://blog.csdn.net/fengbingchun/article/details/128727514
* https://blog.csdn.net/fengbingchun/article/details/146457607
*/

class DisplayLabel : public QLabel { // 解决QLabel控件在运行时不能缩小的情况
    Q_OBJECT

public:
    explicit DisplayLabel(QWidget* parent = nullptr) {}
    QSize minimumSizeHint() const override {
        return QSize(1, 1);
    }
};

class Widgets_Test : public QMainWindow
{
    Q_OBJECT

public:
    Widgets_Test(QWidget *parent = nullptr);
    ~Widgets_Test();

signals:
    void camera_open_failed();

private slots:
    // 槽函数:on_<object name>_<signal name> : 自动关联,无需使用connect将信号与槽关联
    // 在Qt Creator中,打开ui,然后选中按钮,点击右键,然后选择"转到槽...",双击clicked(),会自动生成槽函数

    void start_capture();
    void close_caputre();
    void update_frame(const QImage& image);

private:
    void capture();
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

    Ui::Widgets_TestClass ui_;
    std::thread capture_thread_{};
    std::atomic<bool> is_running_{ false };
    DisplayLabel* label_cap_{ nullptr };
    QImage latest_frame_{};

    QAction* act_open_camera_{ nullptr };
    QAction* act_close_camera_{ nullptr };
};

