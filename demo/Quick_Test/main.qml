import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

// Blog: https://blog.csdn.net/fengbingchun/article/details/128764570

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Quick Test")

    Image { // 背景色
        id: image_backgroud
        width: 640; height: 480
        focus: true
        //fillMode: Image.PreserveAspectFit
        source: "test.jpg"
        anchors.centerIn:parent
    }

    Button { // 关闭窗口按钮
        id: button_close
        // 此按钮的起始点坐标
        x: 580; y: 420
        text: "Close"
        highlighted: true
        flat: true
        onClicked: Qt.exit(0) // 槽函数
    }

    MessageDialog { // csdn对话框
        id: dialog_csdn
        title: qsTr("csdn address")
        text: qsTr("https://blog.csdn.net/fengbingchun/")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
    }

    Button { // 单击弹出csdn对话框
        id: button_csdn_address
        x: 20; y: 20
        text: "csdn address"
        highlighted: true
        flat: true
        onClicked: dialog_csdn.open()
    }

    MessageDialog {
        id: dialog_github
        title: qsTr("github address")
        text: qsTr("https://github.com/fengbingchun")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
    }

    Button {
        id: button_github_address
        x: 120; y: 20
        text: "github address"
        highlighted: true
        flat: true
        onClicked: dialog_github.open()
    }
}
