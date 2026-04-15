/****************************************************************************
** Meta object code from reading C++ file 'ApplicationInstance.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ApplicationInstance.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApplicationInstance.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ApplicationInstance_t {
    QByteArrayData data[14];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApplicationInstance_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApplicationInstance_t qt_meta_stringdata_ApplicationInstance = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ApplicationInstance"
QT_MOC_LITERAL(1, 20, 15), // "messageReceived"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "message"
QT_MOC_LITERAL(4, 45, 23), // "fileOpenRequestReceived"
QT_MOC_LITERAL(5, 69, 8), // "fileName"
QT_MOC_LITERAL(6, 78, 21), // "globalShortcutPressed"
QT_MOC_LITERAL(7, 100, 3), // "key"
QT_MOC_LITERAL(8, 104, 8), // "modifier"
QT_MOC_LITERAL(9, 113, 22), // "registerGlobalShortcut"
QT_MOC_LITERAL(10, 136, 9), // "modifiers"
QT_MOC_LITERAL(11, 146, 11), // "sendMessage"
QT_MOC_LITERAL(12, 158, 7), // "timeout"
QT_MOC_LITERAL(13, 166, 14) // "activateWindow"

    },
    "ApplicationInstance\0messageReceived\0"
    "\0message\0fileOpenRequestReceived\0"
    "fileName\0globalShortcutPressed\0key\0"
    "modifier\0registerGlobalShortcut\0"
    "modifiers\0sendMessage\0timeout\0"
    "activateWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApplicationInstance[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    2,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   60,    2, 0x0a /* Public */,
      11,    2,   65,    2, 0x0a /* Public */,
      11,    1,   70,    2, 0x2a /* Public | MethodCloned */,
      13,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,   10,
    QMetaType::Bool, QMetaType::QString, QMetaType::Int,    3,   12,
    QMetaType::Bool, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void ApplicationInstance::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ApplicationInstance *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->fileOpenRequestReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->globalShortcutPressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->registerGlobalShortcut((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: { bool _r = _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->activateWindow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ApplicationInstance::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ApplicationInstance::messageReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ApplicationInstance::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ApplicationInstance::fileOpenRequestReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ApplicationInstance::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ApplicationInstance::globalShortcutPressed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ApplicationInstance::staticMetaObject = { {
    &QApplication::staticMetaObject,
    qt_meta_stringdata_ApplicationInstance.data,
    qt_meta_data_ApplicationInstance,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ApplicationInstance::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApplicationInstance::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ApplicationInstance.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QObject"))
        return static_cast< QObject*>(this);
    return QApplication::qt_metacast(_clname);
}

int ApplicationInstance::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ApplicationInstance::messageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ApplicationInstance::fileOpenRequestReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ApplicationInstance::globalShortcutPressed(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
