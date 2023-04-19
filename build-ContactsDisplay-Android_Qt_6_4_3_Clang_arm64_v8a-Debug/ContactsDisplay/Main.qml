import QtQuick
import QtQuick.Window
import Contacts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Contacts {id: myContacts}
    ListView {
        height: parent.height; width: parent.width
        model: myContacts.contacts
        spacing: 20
        clip: true
        delegate : Rectangle {
            width: ListView.view.width; height: 40
            color: "light blue"
            Text { text: myContacts.contacts[index] }
        }
    }
}
