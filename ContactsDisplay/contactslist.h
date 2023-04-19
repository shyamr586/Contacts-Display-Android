#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QObject>
#include <QProperty>

class ContactsList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList contacts READ getContacts WRITE setContacts RESET resetContacts NOTIFY contactsChanged)
public:
    explicit ContactsList(QObject *parent = nullptr);
    QStringList contacts;
    QStringList getContacts() const;
    void setContacts(const QStringList &newContacts);
    void resetContacts();
signals:
    void contactsChanged();
private:

};

#endif // CONTACTSLIST_H
