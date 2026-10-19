#include "Widgets_Test.h"
#include <QMessageBox>
#include <QLocale>
#include <QVBoxLayout>
#include <QFont>
#include <QDateTime>

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

    // 在主界面中间显示日期时间
    label_datetime_ = new QLabel(ui_.central_widget);
    label_datetime_->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    label_datetime_->setFont(font);

    auto* layout = new QVBoxLayout(ui_.central_widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label_datetime_, 1, Qt::AlignCenter);

    update_datetime(); // 立即显示日期时间

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &Widgets_Test::update_datetime);
    timer_->start(1000); // 每秒更新
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

void Widgets_Test::update_datetime()
{
    const auto now = QDateTime::currentDateTime();

    label_datetime_->setText(QLocale(QLocale::Chinese, QLocale::China).toString(now, "yyyy年MM月dd日 dddd\nHH:mm:ss"));
}
