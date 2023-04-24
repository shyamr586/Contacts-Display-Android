/****************************************************************************
** Meta object code from reading C++ file 'contactslist.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ContactsDisplay/contactslist.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contactslist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ContactsList_t {
    uint offsetsAndSizes[22];
    char stringdata0[13];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[17];
    char stringdata5[15];
    char stringdata6[6];
    char stringdata7[16];
    char stringdata8[11];
    char stringdata9[11];
    char stringdata10[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ContactsList_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ContactsList_t qt_meta_stringdata_ContactsList = {
    {
        QT_MOC_LITERAL(0, 12),  // "ContactsList"
        QT_MOC_LITERAL(13, 15),  // "contactsChanged"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 15),  // "preItemAppended"
        QT_MOC_LITERAL(46, 16),  // "postItemAppended"
        QT_MOC_LITERAL(63, 14),  // "preItemRemoved"
        QT_MOC_LITERAL(78, 5),  // "index"
        QT_MOC_LITERAL(84, 15),  // "postItemRemoved"
        QT_MOC_LITERAL(100, 10),  // "appendItem"
        QT_MOC_LITERAL(111, 10),  // "removeItem"
        QT_MOC_LITERAL(122, 8)   // "contacts"
    },
    "ContactsList",
    "contactsChanged",
    "",
    "preItemAppended",
    "postItemAppended",
    "preItemRemoved",
    "index",
    "postItemRemoved",
    "appendItem",
    "removeItem",
    "contacts"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ContactsList[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       1,   65, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    2 /* Public */,
       3,    0,   57,    2, 0x06,    3 /* Public */,
       4,    0,   58,    2, 0x06,    4 /* Public */,
       5,    1,   59,    2, 0x06,    5 /* Public */,
       7,    0,   62,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   63,    2, 0x0a,    8 /* Public */,
       9,    0,   64,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::QStringList, 0x00015107, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject ContactsList::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ContactsList.offsetsAndSizes,
    qt_meta_data_ContactsList,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ContactsList_t,
        // property 'contacts'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ContactsList, std::true_type>,
        // method 'contactsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'preItemAppended'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'postItemAppended'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'preItemRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'postItemRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'appendItem'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeItem'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ContactsList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContactsList *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->contactsChanged(); break;
        case 1: _t->preItemAppended(); break;
        case 2: _t->postItemAppended(); break;
        case 3: _t->preItemRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->postItemRemoved(); break;
        case 5: _t->appendItem(); break;
        case 6: _t->removeItem(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ContactsList::*)();
            if (_t _q_method = &ContactsList::contactsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ContactsList::*)();
            if (_t _q_method = &ContactsList::preItemAppended; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ContactsList::*)();
            if (_t _q_method = &ContactsList::postItemAppended; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ContactsList::*)(int );
            if (_t _q_method = &ContactsList::preItemRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ContactsList::*)();
            if (_t _q_method = &ContactsList::postItemRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ContactsList *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->getContacts(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ContactsList *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setContacts(*reinterpret_cast< QStringList*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
        auto *_t = static_cast<ContactsList *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->resetContacts(); break;
        default: break;
        }
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *ContactsList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContactsList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContactsList.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ContactsList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ContactsList::contactsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ContactsList::preItemAppended()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ContactsList::postItemAppended()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ContactsList::preItemRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ContactsList::postItemRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
