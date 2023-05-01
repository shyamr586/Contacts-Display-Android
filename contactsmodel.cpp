#include "contactsmodel.h"
#include <QJniObject>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_setQStringList(JNIEnv *env, jobject, jlong ptr, jstring jstr, bool initial) {

    QList<QVector<QString>> contactsList;

    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString contacts = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());

    // Extract the data from the document
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        // Iterate through the array
        for (int i = 0; i < jsonArray.size(); i++) {
            QJsonObject jsonObj = jsonArray[i].toObject();
            QString contactID = jsonObj.value("contactID").toString();
            QString name = jsonObj.value("name").toString();
            QString phoneNumber = jsonObj.value("phoneNumber").toString();

            QVector<QString> contactData;
            contactData.append(contactID);
            contactData.append(name);
            contactData.append(phoneNumber);

            contactsList.append(contactData);
        }
    }

    ContactsModel* contactsModelInstance = reinterpret_cast<ContactsModel*>(ptr);
    qDebug() << "FROM QT, The contacts list is: " << contactsList;

    if (initial)
        contactsModelInstance->initializeData(contactsList);
    else
        contactsModelInstance->addNewContact(contactsList);

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

bool compareContacts(const QVector<QString> &a, const QVector<QString> &b)
{
    return a[1] < b[1]; // Compare based on the name field
}


void ContactsModel::addNewContact(QList<QVector<QString>> newContacts)
{
    for (const QVector<QString> &newContact : newContacts)
    {
        bool found = false;

        for (int i = 0; i < contacts.size(); ++i)
        {
            QVector<QString> &existingContact = contacts[i];

            if (existingContact[0] == newContact[0]) // [0] is the 'id' field
            {
                found = true;

                // Update the data in the existing contact
                contacts[i] = QVector<QString>({newContact[0], newContact[1], newContact[2]});
                std::sort(contacts.begin(), contacts.end(), compareContacts);
                emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
                break; // No need to search further since we've found a match
            }
        }

        if (!found) // If the id does not exist, add the new contact
        {
            int insertPosition = 0;

            // Find the correct insert position based on the 'name' field
            while (insertPosition < contacts.size() && contacts[insertPosition][1] < newContact[1]) {
                ++insertPosition;
            }

            beginInsertRows(QModelIndex(), insertPosition, insertPosition);
            contacts.insert(insertPosition, newContact);
            endInsertRows();
        }
    }
}

void ContactsModel::removeContact(QString id)
{

    for (int i = 0; i < contacts.size(); ++i)
    {
        if (contacts.at(i).at(0) == id)
        {
            // Remove the contact with the matching id
            beginRemoveRows(QModelIndex(), i,i);
            contacts.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void ContactsModel::initializeData(QList<QVector<QString>> value)
{
    beginResetModel();
    setContacts(value);
    endResetModel();
}

QList<QVector<QString>> ContactsModel::getContacts() const
{
    return contacts;
}

void ContactsModel::setContacts(const QList<QVector<QString>> &newContacts)
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
        return contacts.at(index.row()).at(1); // 1 corresponds to the 'name' field
    case NumberRole:
        return contacts.at(index.row()).at(2); // 2 corresponds to the 'number' field
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
