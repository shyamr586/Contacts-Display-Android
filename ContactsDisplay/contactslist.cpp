#include "contactslist.h"
#include <QJniObject>
#include <QGuiApplication>

QJniObject javaClass = QNativeInterface::QAndroidApplication::context();

QStringList loadData(){
    QJniObject arrayList = javaClass.callObjectMethod("getContacts", "()Ljava/util/ArrayList;");
    int size = (int)arrayList.callMethod<jint>("size","()I");

    QStringList finalList;

    for(int i = 0; i < size; i++) {
        QJniObject element = arrayList.callObjectMethod("get", "(I)Ljava/lang/Object;", i);
        QString qstring = element.toString();
        finalList.append(qstring);
    }
    return finalList;
    qDebug() << "The list is: " << finalList;
}

ContactsList::ContactsList(QObject *parent)
    : QObject{parent}
{
    QStringList returnedList = loadData();
    setContacts(returnedList);
}

QStringList ContactsList::getContacts() const
{
    return contacts;
}

void ContactsList::setContacts(const QStringList &newContacts)
{
    if (contacts == newContacts)
        return;
    contacts = newContacts;
    emit contactsChanged();
}

void ContactsList::resetContacts()
{
    setContacts({}); // TODO: Adapt to use your actual default value
}
