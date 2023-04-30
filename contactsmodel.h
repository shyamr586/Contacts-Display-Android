#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
#include <QMap>

class ContactsList;

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QMap<QString, QStringList> contacts READ getContacts WRITE setContacts NOTIFY contactsChanged)

public:
    explicit ContactsModel(QObject *parent = nullptr);
    void addNewContact(QMap<QString, QStringList>);
    void removeContact(QString);
    void initializeData(QMap<QString, QStringList>);
    QMap<QString, QStringList> contacts;

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

    QMap<QString, QStringList> getContacts() const;
    void setContacts(const QMap<QString, QStringList> &newContacts);

    QStringList datas() const;

signals:
    void contactsChanged();

private:
    QMap<QString, QStringList> mContacts;
};

#endif // CONTACTSMODEL_H
