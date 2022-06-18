#ifndef DATA_MANIPULATION_H
#define DATA_MANIPULATION_H

#include <QString>
#include <QPair>
#include <QVector>

using data = QPair<QString, double>;
using container = QVector< QPair<QString, double> > ;

enum class type_file : int
{
    sql,
    json,

    unknown
};

template <type_file type>
struct dataManipulation
{
    container getData(const QString&/*path*/) const
    {
        return QVector<data>{};
    }
};


#endif // DATA_MANIPULATION_H
