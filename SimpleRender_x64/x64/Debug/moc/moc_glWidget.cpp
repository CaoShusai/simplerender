/****************************************************************************
** Meta object code from reading C++ file 'glWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../glWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_glWidget_t {
    QByteArrayData data[13];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_glWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_glWidget_t qt_meta_stringdata_glWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "glWidget"
QT_MOC_LITERAL(1, 9, 15), // "mySignalAddList"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "mySignalShowFps"
QT_MOC_LITERAL(4, 42, 7), // "mySlot2"
QT_MOC_LITERAL(5, 50, 11), // "mySlotMosue"
QT_MOC_LITERAL(6, 62, 14), // "mySlotOpenFile"
QT_MOC_LITERAL(7, 77, 14), // "mySlotSaveFile"
QT_MOC_LITERAL(8, 92, 12), // "mySlotImport"
QT_MOC_LITERAL(9, 105, 20), // "mySlotCameraFreeMode"
QT_MOC_LITERAL(10, 126, 22), // "mySlotCameraCenterMode"
QT_MOC_LITERAL(11, 149, 17), // "mySlotImportLight"
QT_MOC_LITERAL(12, 167, 15) // "mySlotClickList"

    },
    "glWidget\0mySignalAddList\0\0mySignalShowFps\0"
    "mySlot2\0mySlotMosue\0mySlotOpenFile\0"
    "mySlotSaveFile\0mySlotImport\0"
    "mySlotCameraFreeMode\0mySlotCameraCenterMode\0"
    "mySlotImportLight\0mySlotClickList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_glWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       3,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    4,   75,    2, 0x08 /* Private */,
       5,    2,   84,    2, 0x08 /* Private */,
       6,    0,   89,    2, 0x08 /* Private */,
       7,    0,   90,    2, 0x08 /* Private */,
       8,    0,   91,    2, 0x08 /* Private */,
       9,    0,   92,    2, 0x08 /* Private */,
      10,    0,   93,    2, 0x08 /* Private */,
      11,    0,   94,    2, 0x08 /* Private */,
      12,    1,   95,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Float,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void glWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<glWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mySignalAddList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->mySignalShowFps((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->mySlot2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->mySlotMosue((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 4: _t->mySlotOpenFile(); break;
        case 5: _t->mySlotSaveFile(); break;
        case 6: _t->mySlotImport(); break;
        case 7: _t->mySlotCameraFreeMode(); break;
        case 8: _t->mySlotCameraCenterMode(); break;
        case 9: _t->mySlotImportLight(); break;
        case 10: _t->mySlotClickList((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (glWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&glWidget::mySignalAddList)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (glWidget::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&glWidget::mySignalShowFps)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject glWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_glWidget.data,
    qt_meta_data_glWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *glWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *glWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_glWidget.stringdata0))
        return static_cast<void*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int glWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void glWidget::mySignalAddList(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void glWidget::mySignalShowFps(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
