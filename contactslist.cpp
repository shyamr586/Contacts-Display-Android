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

QVector<ContactsStruct> ContactsList::items() const
{
    return mItems;
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

bool ContactsList::setItemAt(int index, const ContactsStruct &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const ContactsStruct &oldItem = mItems.at(index);
    if (item.name == oldItem.name && item.number == oldItem.number)
        return false;

    mItems[index] = item;
    return true;
}

void ContactsList::appendItem()
{
    emit preItemAppended();

    ContactsStruct item;
    mItems.append(item);

    emit postItemAppended();
}

void ContactsList::removeItem()
{
    // remove here
}
