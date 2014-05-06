/****************************************************************************
** Meta object code from reading C++ file 'lambda.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "lambda.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lambda.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_lambda[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,   14,   14,   14, 0x08,
      15,   14,   14,   14, 0x08,
      21,   14,   14,   14, 0x08,
      27,   14,   14,   14, 0x08,
      33,   14,   14,   14, 0x08,
      39,   14,   14,   14, 0x08,
      52,   14,   14,   14, 0x08,
      60,   14,   14,   14, 0x08,
      67,   14,   14,   14, 0x08,
      74,   14,   14,   14, 0x08,
      87,   14,   14,   14, 0x08,
     100,   14,   14,   14, 0x08,
     114,   14,   14,   14, 0x08,
     124,   14,   14,   14, 0x08,
     134,   14,   14,   14, 0x08,
     147,   14,   14,   14, 0x08,
     162,   14,   14,   14, 0x08,
     175,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_lambda[] = {
    "lambda\0open()\0\0vis()\0rce()\0rco()\0avi()\0"
    "showbounds()\0start()\0stop()\0snap()\0"
    "processSim()\0processRep()\0setContrast()\0"
    "setZoom()\0setSkip()\0setQuality()\0"
    "setFramerate()\0setSamples()\0checkScreen()\0"
};

void lambda::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        lambda *_t = static_cast<lambda *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->vis(); break;
        case 2: _t->rce(); break;
        case 3: _t->rco(); break;
        case 4: _t->avi(); break;
        case 5: _t->showbounds(); break;
        case 6: _t->start(); break;
        case 7: _t->stop(); break;
        case 8: _t->snap(); break;
        case 9: _t->processSim(); break;
        case 10: _t->processRep(); break;
        case 11: _t->setContrast(); break;
        case 12: _t->setZoom(); break;
        case 13: _t->setSkip(); break;
        case 14: _t->setQuality(); break;
        case 15: _t->setFramerate(); break;
        case 16: _t->setSamples(); break;
        case 17: _t->checkScreen(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData lambda::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject lambda::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_lambda,
      qt_meta_data_lambda, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &lambda::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *lambda::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *lambda::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_lambda))
        return static_cast<void*>(const_cast< lambda*>(this));
    return QWidget::qt_metacast(_clname);
}

int lambda::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
