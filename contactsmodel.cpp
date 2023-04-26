#include "contactsmodel.h"
#include <QJniObject>
#include <QGuiApplication>

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_addToModel(JNIEnv *env, jobject, jlong ptr, jstring elem, jint index) {
    const char* chars = env->GetStringUTFChars(elem, nullptr);
    if (chars){
        QString qStr = QString::fromUtf8(chars);
        env->ReleaseStringUTFChars(elem, chars);
        ContactsModel* model = reinterpret_cast<ContactsModel*>(ptr);
        model->addNewContact(index,qStr);
    }
}
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_removeFromModel(JNIEnv *env, jobject, jlong ptr, jint index) {
    ContactsModel* model = reinterpret_cast<ContactsModel*>(ptr);
    model->removeContact(index);
}
}


ContactsModel::ContactsModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    QJniObject arrayList = javaClass.callObjectMethod("getContacts", "(Z)Ljava/util/ArrayList;", true);
    javaClass.callMethod<void>("setPointer","(J)V", (long long)(ContactsModel*)this);
    //javaClass.callMethod<void>("setInitialArrayList","()V");
    int size = (int)arrayList.callMethod<jint>("size","()I");
    QStringList finalList;

    for(int i = 0; i < size; i++) {
        QJniObject element = arrayList.callObjectMethod("get", "(I)Ljava/lang/Object;", i);
        QString qstring = element.toString();
        finalList.append(qstring);
    }
    setContacts(finalList);

}

void ContactsModel::addNewContact(int index, QString value)
{
    beginInsertRows(QModelIndex(), index, index);
    contacts.insert(index, value);
    endInsertRows();
}

void ContactsModel::removeContact(int index)
{
    beginRemoveRows(QModelIndex(), index,index);
    contacts.removeAt(index);
    endRemoveRows();
}

QStringList ContactsModel::getContacts() const
{
    return contacts;
}

void ContactsModel::setContacts(const QStringList &newContacts)
{
    if (contacts == newContacts)
        return;
    contacts = newContacts;
    emit contactsChanged();
}

QStringList ContactsModel::datas() const
{
    return mDatas;
}

int ContactsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return contacts.size();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    switch (role) {
    case NameRole:
        return QVariant(contacts.at(index.row()).split(":")[0]);
    case NumberRole:
        return QVariant(contacts.at(index.row()).split(":")[1]);
    }

    return QVariant();
}

bool ContactsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[NumberRole] = "number";
    return names;
}
