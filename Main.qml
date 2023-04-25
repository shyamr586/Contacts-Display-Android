import QtQuick
import QtQuick.Window
import ContactsModel

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    ListView {
        height: parent.height; width: parent.width
        model: ContactsModel{}
        spacing: 20
        clip: true
        delegate : Rectangle {
            width: ListView.view.width; height: 40
            color: "light blue"
            Text { text: model.Name + " - " + model.Number }
        }
    }
}