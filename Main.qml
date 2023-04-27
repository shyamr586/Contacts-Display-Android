import QtQuick
import QtQuick.Window
import ContactsModel

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    ListView {
        id: listView
        height: parent.height; width: parent.width
        model: ContactsModel{}
        spacing: 20
        clip: true
        delegate : Rectangle {
            width: ListView.view.width; height: 40
            color: "light blue"
            Text { text: model.name + " - " + model.number }
        }
    }

    Text {anchors.centerIn: parent; visible: listView.count===0; text: "The data is currently loading."; }
}
