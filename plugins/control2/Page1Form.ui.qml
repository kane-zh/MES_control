import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    width: 600
    height: 700

    title: qsTr("Page 1")

    Label {
        text: qsTr("You are on Page 1.")
        anchors.centerIn: parent
    }

    TextEdit {
        id: textEdit
        x: 58
        y: 57
        width: 80
        height: 81
        text: qsTr("Text Edit")
        font.pixelSize: 12
    }

    Text {
        id: element
        x: 174
        y: 50
        width: 51
        height: 61
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Image {
        id: image
        x: 338
        y: 50
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "qrc:/qtquickplugin/images/template_image.png"
    }

    Rectangle {
        id: rectangle
        x: 32
        y: 100
        width: 200
        height: 200
        color: "#ffffff"

        Text {
            id: element1
            x: 57
            y: 67
            text: qsTr("Text")
            font.pixelSize: 12
        }
    }
}
