import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    Rectangle {
        anchors.centerIn: parent
        width: 200
        height: 200
        color: "red"

        MouseArea {
            anchors.fill: parent

            onClicked:  {
                appCore.runMyService1();
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("QtServiceHelloWorld!")
        }

    }

}
