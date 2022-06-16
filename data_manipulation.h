#ifndef DATA_MANIPULATION_H
#define DATA_MANIPULATION_H

#include <QString>
#include <QPair>
#include <QVector>

// todo: to date
using data = QPair<QString, double>;

enum class type_file : int
{
    sql,
    json
};

template <type_file type>
struct dataManipulation
{
    QVector<data> getData(const QString&/*path*/) const
    {
        return QVector<data>{};
    }
};


#endif // OPENDATA_H
