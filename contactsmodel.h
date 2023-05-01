#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVector>

class ContactsList;

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QList<QVector<QString>> contacts READ getContacts WRITE setContacts NOTIFY contactsChanged)

public:
    explicit ContactsModel(QObject *parent = nullptr);
    void addNewContact(QList<QVector<QString>>);
    void removeContact(QString);
    void initializeData(QList<QVector<QString>>);
    QList<QVector<QString>> contacts;

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

    QList<QVector<QString>> getContacts() const;
    void setContacts(const QList<QVector<QString>> &newContacts);

    QStringList datas() const;

signals:
    void contactsChanged();

private:
    QList<QVector<QString>> mContacts;

};

#endif // CONTACTSMODEL_H
