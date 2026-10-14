#pragma once

#include <thread>
#include <atomic>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QEvent>

class DisplayLabel : public QLabel { // 解决QLabel控件在运行时不能缩小的情况
    Q_OBJECT

public:
    explicit DisplayLabel(QWidget* parent = nullptr) {}
    QSize minimumSizeHint() const override {
        return QSize(1, 1);
    }
};

class CameraDialog : public QDialog {
	Q_OBJECT

public:
	explicit CameraDialog(QWidget* parent = nullptr);
	~CameraDialog();

signals:
    void camera_open_failed();

private slots:
    // 槽函数:on_<object name>_<signal name> : 自动关联,无需使用connect将信号与槽关联
    // 在Qt Creator中,打开ui,然后选中按钮,点击右键,然后选择"转到槽...",双击clicked(),会自动生成槽函数

    void start_capture();
    void close_capture();
    void update_frame(const QImage& image);

private:
    void capture();
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

    std::thread capture_thread_{};
    std::atomic<bool> is_running_{ false };

    DisplayLabel* label_cap_{ nullptr };
    QImage latest_frame_{};
    QPushButton* button_open_{ nullptr };
    QPushButton* button_close_{ nullptr };
};
