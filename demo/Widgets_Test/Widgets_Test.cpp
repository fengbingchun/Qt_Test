#include "Widgets_Test.h"
#include <QMessageBox>

Widgets_Test::Widgets_Test(QWidget *parent)
    : QMainWindow(parent)
{
    ui_.setupUi(this);

    // 菜单栏
    auto menu_url = ui_.menu_bar->addMenu("网址");
    auto act_csdn = menu_url->addAction("CSDN网址");
    menu_url->addSeparator(); // 分隔线
    connect(act_csdn, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "CSDN网址", "https://blog.csdn.net/fengbingchun/", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    auto act_github = menu_url->addAction("GitHub网址");
    connect(act_github, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "GitHub网址", "https://github.com/fengbingchun", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    auto act_camera = ui_.menu_bar->addAction("Camera");
    connect(act_camera, &QAction::triggered, this, &Widgets_Test::show_camera);

    // 工具栏
    ui_.main_tool_bar->addAction(act_csdn);
    ui_.main_tool_bar->addSeparator();
    ui_.main_tool_bar->addAction(act_github);
    ui_.main_tool_bar->addSeparator();

    // 禁止拖拽工具栏
    ui_.main_tool_bar->setMovable(false);
    ui_.main_tool_bar->setFloatable(false);
}

Widgets_Test::~Widgets_Test()
{
    FBC_LOG_INFO("successfully exits Widgets_Test destructor");
}

void Widgets_Test::show_camera()
{
    CameraDialog dialog(this);

    dialog.exec();
}
