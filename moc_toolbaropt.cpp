/****************************************************************************
** Meta object code from reading C++ file 'toolbaropt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../github/CVProject/toolbaropt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolbaropt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ToolbarOpt_t {
    QByteArrayData data[5];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolbarOpt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolbarOpt_t qt_meta_stringdata_ToolbarOpt = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ToolbarOpt"
QT_MOC_LITERAL(1, 11, 20), // "on_act_add_triggered"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 21), // "on_act_diff_triggered"
QT_MOC_LITERAL(4, 55, 20) // "on_act_mul_triggered"

    },
    "ToolbarOpt\0on_act_add_triggered\0\0"
    "on_act_diff_triggered\0on_act_mul_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolbarOpt[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ToolbarOpt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolbarOpt *_t = static_cast<ToolbarOpt *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_act_add_triggered(); break;
        case 1: _t->on_act_diff_triggered(); break;
        case 2: _t->on_act_mul_triggered(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ToolbarOpt::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_ToolbarOpt.data,
      qt_meta_data_ToolbarOpt,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ToolbarOpt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolbarOpt::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ToolbarOpt.stringdata0))
        return static_cast<void*>(const_cast< ToolbarOpt*>(this));
    return QToolBar::qt_metacast(_clname);
}

int ToolbarOpt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
