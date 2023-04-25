#include "contactsmodel.h"
#include <QJniObject>
#include <QGuiApplication>


extern "C" {
JNIEXPORT void JNICALL
Java_com_example_contactsdisplay_MainActivity_nativeFunction(JNIEnv *env, jobject, jlong ptr, jobject arrList) {
    QJniObject javaClass2 = QNativeInterface::QAndroidApplication::context();
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID sizeMethod = env->GetMethodID(arrayListClass, "size", "()I");
    jmethodID getMethod = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
    jint len = env->CallIntMethod(arrList, sizeMethod);
    QStringList editedList;

    for (jint i = 0; i < len; ++i) {
        jobject javaString = env->CallObjectMethod(arrList, getMethod, i);
        const char* rawString = env->GetStringUTFChars((jstring)javaString, nullptr);
        editedList.append(QString::fromUtf8(rawString));
        env->ReleaseStringUTFChars((jstring)javaString, rawString);
        env->DeleteLocalRef(javaString);
    }

    ContactsModel* model = reinterpret_cast<ContactsModel*>(ptr);
    QStringList modelContacts = model->contacts;
    model->addOrRemoveContacts(editedList);

    //model->loader(editedList);
    }
}


ContactsModel::ContactsModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
    QJniObject javaClass2 = QNativeInterface::QAndroidApplication::context();
    QJniObject arrayList = javaClass2.callObjectMethod("getContacts", "()Ljava/util/ArrayList;");
    javaClass2.callMethod<void>("setPointer","(J)V", (long long)(ContactsModel*)this);
    int size = (int)arrayList.callMethod<jint>("size","()I");
    setSize(size);
    QStringList finalList;

    for(int i = 0; i < size; i++) {
        QJniObject element = arrayList.callObjectMethod("get", "(I)Ljava/lang/Object;", i);

        QString qstring = element.toString();
        finalList.append(qstring);
    }
    setContacts(finalList);

}

void ContactsModel::loader(QStringList newData)
{
    qDebug() << "The size of the existing contacts is: " << contacts.size();
    qDebug() << "The size of the newData is: " << newData.size();
    beginResetModel();
    setContacts(newData);
    endResetModel();
}

void ContactsModel::addOrRemoveContacts(QStringList newList)
{
    qDebug() << "\n\n Size of contacts is: " << contacts.size();
    qDebug() << "\n\n Size of newList is: " << newList.size();
    bool removed = false;
    if (contacts.size() > newList.size()) {
        qDebug() << "!!!!!!!!Contacts have been removed from the phone";
        for (int i = 0; i < newList.size(); i++) {
            if (!newList.contains(contacts.at(i))){
                qDebug() << "Removing: " << contacts.at(i);
                removeContact(i);
            }
        }

    }

    else if (contacts.size() < newList.size()) {
        qDebug() << "!!!!!!!!Contacts have been added on the phone";
        for (int i = 0; i < newList.size(); i++) {
            if (!contacts.contains(newList.at(i))){
                qDebug() << "Need to append: " << newList.at(i);
                addNewContact(i, newList.at(i));
            }
        }

    }

    else {
        for (int i = 0; i < newList.size(); i++) {
            if (newList.at(i)!=contacts.at(i)){
                removeContact(i);
                addNewContact(i, newList.at(i));
            }
        }
    }
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

void ContactsModel::resetContacts()
{
    setContacts({}); // TODO: Adapt to use your actual default value
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
    names[NameRole] = "Name";
    names[NumberRole] = "Number";
    return names;
}

int ContactsModel::getSize() const
{
    return size;
}

void ContactsModel::setSize(int newSize)
{
    if (size == newSize)
        return;
    size = newSize;
    emit sizeChanged();
}

void ContactsModel::resetSize()
{
    setSize({}); // TODO: Adapt to use your actual default value
}
