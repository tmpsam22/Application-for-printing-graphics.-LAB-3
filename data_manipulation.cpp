#include "data_manipulation.h"
#include <QtSql>

template <>
struct dataManipulate<type_file::sql>
{
    QVector<data> getData(const QString& path) const
    {
        QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
#ifdef DEBUG
        qDebug() << "Path is:\t" << path << '\n';
#endif
        dbase.setDatabaseName(path);
        if (!dbase.open())
        {
            // todo: simple output with message box
            qDebug() << dbase.lastError().text();
            return QVector<data>();
        }

        QVector<data> data_;
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
        while (query.next()) {
            auto date_ = query.value(0).toString();
#ifdef DEBUG
            qDebug() << "date:\t" << date_ << '\t';
            qDebug() << "value:\t" << query.value(1).toDouble() << '\n';
#endif
            auto value_ = query.value(1).toDouble();
            data_.push_back(qMakePair(date_, value_));
        }
#ifdef DEBUG
        for (auto& i : data_)
        {
            qDebug() << i << '\t';
        }
#endif
        return data_;
    }
};

template <>
struct dataManipulate<type_file::json>
{
    QVector<data> getData(const QString& path) const
    {
        return QVector<data>{};
    }
}
