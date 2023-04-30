#include "contactsmodel.h"
#include <QJniObject>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_setQStringList(JNIEnv *env, jobject, jlong ptr, jstring jstr, bool initial) {

    QMap<QString, QStringList> contactsMap;

    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString contacts = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());
    //QString qstr = QString::fromUtf8(env->GetStringUTFChars(jstr, NULL));
    env->ReleaseStringUTFChars(jstr, nullptr);
    // Extract the data from the document
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        // Iterate through the array
        for (int i = 0; i < jsonArray.size(); i++) {
            QJsonObject jsonObj = jsonArray[i].toObject();
            QString contactID = jsonObj.value("contactID").toString();
            QString name = jsonObj.value("name").toString();
            QString phoneNumber = jsonObj.value("phoneNumber").toString();

            QStringList contactData;
            contactData << name << phoneNumber;
            contactsMap.insert(contactID, contactData);
        }
    }

    ContactsModel* contactsModelInstance = reinterpret_cast<ContactsModel*>(ptr);
    qDebug() << "FROM QT, The contacts map is: " << contactsMap;

    if (initial){
        contactsModelInstance->initializeData(contactsMap);
    } else {
        contactsModelInstance->addNewContact(contactsMap);
    }
}
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_removeFromModel(JNIEnv *env, jobject, jlong ptr, jstring jContactId) {

    const char *cContactId = env->GetStringUTFChars(jContactId, NULL);
    QString contactId = QString::fromUtf8(cContactId);
    env->ReleaseStringUTFChars(jContactId, cContactId);
    ContactsModel* model = reinterpret_cast<ContactsModel*>(ptr);
    model->removeContact(contactId);
}
}


ContactsModel::ContactsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("setPointer","(J)V", (long long)(ContactsModel*)this);
    javaClass.callMethod<void>("getContacts", "(Z)V", true);

}

void ContactsModel::addNewContact(QMap<QString, QStringList> newContacts)
{
    beginResetModel();
    //beginInsertRows(QModelIndex(), 0, 0);
    //    contacts.insert(index, value);
    qDebug()<< "The size of newContacts is: " << newContacts.size();
    for (auto it = newContacts.constBegin(); it != newContacts.constEnd(); ++it) {
        const QString contactId = it.key();
        const QStringList values = it.value();
        qDebug()<< "The value of newContacts contactid is: " << contactId;
        if (contacts.contains(contactId)) {
            // Update an existing contact
            contacts[contactId] = values;
        } else {
            if (!contactId.isNull()){
                contacts.insert(contactId, values);
            }
            // Add a new contact
        }
    }
    qDebug() << "THE VALUE OF CONTACTS IS: " << contacts;
    for (auto it = contacts.begin(); it != contacts.end(); ) {
        if (it.key().isNull() || it.value().isEmpty()) {
            it = contacts.erase(it);
        } else {
            ++it;
        }
    }
    //endInsertRows();
    endResetModel();
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
}

void ContactsModel::removeContact(QString key)
{
    qDebug() << "Need to remove " << contacts[key];
    beginRemoveRows(QModelIndex(), 0,0);
    contacts.remove(key);
    endRemoveRows();
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
}

void ContactsModel::initializeData(QMap<QString, QStringList> value)
{
    beginResetModel();
    setContacts(value);
    endResetModel();
}

QMap<QString, QStringList> ContactsModel::getContacts() const
{
    return contacts;
}

void ContactsModel::setContacts(const QMap<QString, QStringList> &newContacts)
{
    if (contacts == newContacts)
        return;
    contacts = newContacts;
    emit contactsChanged();
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
        //        return "name";
        return contacts.values().at(index.row()).at(0);
        //return QVariant(contacts.at(index.row()).split(":")[0]);
    case NumberRole:
        //        return "number";
        return contacts.values().at(index.row()).at(1);
        //return QVariant(contacts.at(index.row()).split(":")[1]);
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
