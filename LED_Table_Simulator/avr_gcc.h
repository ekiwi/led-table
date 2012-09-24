#ifndef AVR_GCC_H
#define AVR_GCC_H

#include <QtGlobal>
#include <QtDebug>
// integer types


typedef qint8 int8_t;
typedef quint8 uint8_t;
typedef qint16 int16_t;
typedef quint16 uint16_t;
typedef qint32 int32_t;
typedef quint32 uint32_t;

#define deb(x) qDebug() << "Deb: " << x << endl
#define deb_color(c) qDebug() << "Color: {" << c.red << ", " << c.green << ", " << c.blue << "}" << endl


#endif // AVR_GCC_H
