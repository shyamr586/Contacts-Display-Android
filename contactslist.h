#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QObject>
#include <QProperty>
#include <QVector>

struct ContactsStruct {
    QString name;
    QString number;
};

class ContactsList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList contacts READ getContacts WRITE setContacts RESET resetContacts NOTIFY contactsChanged)
public:
    explicit ContactsList(QObject *parent = nullptr);

    QVector<ContactsStruct> items() const;

    QStringList contacts;
    QStringList getContacts() const;
    void setContacts(const QStringList &newContacts);
    void resetContacts();

    bool setItemAt(int index, const ContactsStruct &item);
signals:
    void contactsChanged();

    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeItem();
private:
    QVector<ContactsStruct> mItems;
};

#endif // CONTACTSLIST_H
