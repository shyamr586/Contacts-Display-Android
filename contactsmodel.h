#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>

class ContactsList;

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList contacts READ getContacts WRITE setContacts RESET resetContacts NOTIFY contactsChanged)

public:
    explicit ContactsModel(QObject *parent = nullptr);
    void loader(QStringList);
    void addOrRemoveContacts(QStringList);
    void addNewContact(int, QString);
    void removeContact(int);
    QStringList contacts;

    enum {
        NameRole,
        NumberRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void resetSize();

    QStringList getContacts() const;
    void setContacts(const QStringList &newContacts);
    void resetContacts();

    QStringList datas() const;

signals:

    void contactsChanged();

private:
    ContactsList *mList;
    QStringList mContacts;
    QStringList mDatas;

};

#endif // CONTACTSMODEL_H
