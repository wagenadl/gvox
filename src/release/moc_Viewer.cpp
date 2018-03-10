/****************************************************************************
** Meta object code from reading C++ file 'Viewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Viewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Viewer_t {
    QByteArrayData data[16];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Viewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Viewer_t qt_meta_stringdata_Viewer = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Viewer"
QT_MOC_LITERAL(1, 7, 16), // "selectionChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "setMode"
QT_MOC_LITERAL(4, 33, 4), // "Mode"
QT_MOC_LITERAL(5, 38, 7), // "setName"
QT_MOC_LITERAL(6, 46, 8), // "doExport"
QT_MOC_LITERAL(7, 55, 11), // "showOverlay"
QT_MOC_LITERAL(8, 67, 10), // "showTraces"
QT_MOC_LITERAL(9, 78, 4), // "copy"
QT_MOC_LITERAL(10, 83, 3), // "add"
QT_MOC_LITERAL(11, 87, 3), // "del"
QT_MOC_LITERAL(12, 91, 9), // "toggleIDs"
QT_MOC_LITERAL(13, 101, 4), // "find"
QT_MOC_LITERAL(14, 106, 2), // "id"
QT_MOC_LITERAL(15, 109, 4) // "name"

    },
    "Viewer\0selectionChanged\0\0setMode\0Mode\0"
    "setName\0doExport\0showOverlay\0showTraces\0"
    "copy\0add\0del\0toggleIDs\0find\0id\0name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Viewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   77,    2, 0x0a /* Public */,
       5,    1,   80,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x0a /* Public */,
       7,    1,   84,    2, 0x0a /* Public */,
       8,    1,   87,    2, 0x0a /* Public */,
       9,    0,   90,    2, 0x0a /* Public */,
      10,    0,   91,    2, 0x0a /* Public */,
      11,    0,   92,    2, 0x0a /* Public */,
      12,    0,   93,    2, 0x0a /* Public */,
      13,    1,   94,    2, 0x0a /* Public */,
      13,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void Viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Viewer *_t = static_cast<Viewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setMode((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        case 2: _t->setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->doExport(); break;
        case 4: _t->showOverlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->showTraces((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->copy(); break;
        case 7: _t->add(); break;
        case 8: _t->del(); break;
        case 9: _t->toggleIDs(); break;
        case 10: _t->find((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->find((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Viewer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Viewer::selectionChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Viewer::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_Viewer.data,
      qt_meta_data_Viewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Viewer.stringdata0))
        return static_cast<void*>(const_cast< Viewer*>(this));
    return QLabel::qt_metacast(_clname);
}

int Viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Viewer::selectionChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
