#include "camera_dialog.h"
#include <opencv2/opencv.hpp>
#include "logger.hpp"

// CameraDialog继承自QDialog,所以创建CameraDialog对象时,必须先构造它的基类QDialog,然后才能执行CameraDialog自己的构造函数体;
// 若没有显示指定基类构造函数(QDialog(parent)),编译器会尝试调用QDialog的默认构造函数，相当于没有把parent传给基类
CameraDialog::CameraDialog(QWidget* parent):QDialog(parent) 
{
    setWindowTitle(tr("摄像头"));
    resize(800, 600);

    label_cap_ = new DisplayLabel(this);
    label_cap_->setStyleSheet("background-color:#000000;");
    label_cap_->setAlignment(Qt::AlignCenter);
    label_cap_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label_cap_->setMinimumSize(1, 1);
    label_cap_->installEventFilter(this);

    button_open_ = new QPushButton("Open", this);
    button_close_ = new QPushButton("Close", this);
    button_close_->setEnabled(false);

    // 虽然layout是局部变量，但它指向的QVBoxLayout对象并不会在构造函数结束时销毁,this是它的父对象. Qt的对象树会管理它的生命周期
    // 如果以后还需要动态调整布局,例如动态增加控件,可以将其定义为成员变量: QVBoxLayout* layout_{ nullptr };
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2); // 设置布局内边距(布局边缘和它内部子控件之间的空白距离,单位像素)
    layout->addWidget(label_cap_);
    layout->addWidget(button_open_);
    layout->addWidget(button_close_);

    connect(button_open_, &QPushButton::clicked, this, &CameraDialog::start_capture);
    connect(button_close_, &QPushButton::clicked, this, &CameraDialog::close_capture);

    connect(this, &CameraDialog::camera_open_failed, this, [this]() {
        button_open_->setEnabled(true);
        button_close_->setEnabled(false);
        
        latest_frame_ = QImage();

        if (label_cap_)
            label_cap_->clear();
    });
}

CameraDialog::~CameraDialog()
{
	close_capture();
	FBC_LOG_INFO("successfully exits CameraDialog destructor");
}

bool CameraDialog::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == label_cap_ && event->type() == QEvent::Resize) {
        if (!latest_frame_.isNull() && label_cap_) {
            QPixmap pix = QPixmap::fromImage(latest_frame_).scaled(label_cap_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label_cap_->setPixmap(pix);
        }
    }
    
    return QDialog::eventFilter(watched, event);
}

void CameraDialog::start_capture()
{
    if (is_running_.load())
        return;

    if (capture_thread_.joinable())
        capture_thread_.join();

    latest_frame_ = QImage(); // 清空的是内存中的图像数据

    if (label_cap_)
        label_cap_->clear(); // 清空的是QLabel的显示内容

    is_running_.store(true);

    capture_thread_ = std::thread([this] {
        capture();
    });

    button_open_->setEnabled(false);
    button_close_->setEnabled(true);
}

void CameraDialog::close_capture()
{
    is_running_.store(false);

    if (capture_thread_.joinable())
        capture_thread_.join();

    latest_frame_ = QImage();

    if (label_cap_)
        label_cap_->clear();

    button_open_->setEnabled(true);
    button_close_->setEnabled(false);
}

void CameraDialog::update_frame(const QImage& image)
{
    if (!is_running_.load())
        return;

    if (!label_cap_)
        return;

    latest_frame_ = image;

    auto pix = QPixmap::fromImage(image).scaled(label_cap_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label_cap_->setPixmap(pix);
}

void CameraDialog::capture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        FBC_LOG_ERROR("failed to open camera"); // QMetaObject::invokeMethod, UI操作必须在主线程中执行
        is_running_.store(false);
        emit camera_open_failed();
        return;
    }

    cv::Mat frame{}, rgb{};
    QImage image{}, img_copy{};
    while (is_running_.load()) {
        cap >> frame;
        if (frame.empty())
            continue;

        cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        img_copy = image.copy(); // 必须copy, Mat会被下一轮覆盖

        // 跨线程调用UI槽函数 QueuedConnection
        QMetaObject::invokeMethod(this, "update_frame", Qt::QueuedConnection, Q_ARG(QImage, img_copy));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

