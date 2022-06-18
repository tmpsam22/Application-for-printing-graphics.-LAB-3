#ifndef DATA_MANIPULATION_H
#define DATA_MANIPULATION_H

#include <QString>
#include <QPair>
#include <QVector>
#include <QtDebug>
#include <QtSql>
#include "message_box.h"

using data = QPair<QString, double>;
using container = QVector< QPair<QString, double> >;

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

template <>
struct dataManipulation<type_file::sql>
{
    container getData(const QString& path) const
    {
        QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
        dbase.setDatabaseName(path);
        if (!dbase.open())
        {
            messageBox{ dbase.lastError().text() };
            return QVector<data>{};
        }

        QVector<data> data_;
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
        while (query.next())
        {
            auto date_ = query.value(0).toString();
            auto value_ = query.value(1).toDouble();
            data_.push_back(qMakePair(date_, value_));
        }
#ifdef DEBUG
        for (auto const& val: data_)
        {
            qDebug() << val.first << " | " << val.second << '\n';
        }
#endif
        return data_;
    }
};

template<>
struct dataManipulation<type_file::json>
{
    container getData(const QString& path) const
    {
        QFile file;
        file.setFileName(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!file.isOpen())
        {
            messageBox{ "File was not open!" };
            return QVector<data>{};
        }

        QString val;
        val = file.readAll();
        file.close();
        auto doc = QJsonDocument::fromJson(val.toUtf8());
        if (!doc.isArray())
        {
            messageBox{ "Expect json in an array" };
            return QVector<data>{};
        }

        auto array_ = doc.array();
        QVector<data> data_;
        foreach (const QJsonValue & value, array_)
        {
            QJsonObject obj = value.toObject();
            if (obj.contains("Time") && obj.contains("Value"))
            {
                data_.push_back(
                            qMakePair(obj["Time"].toString(),
                            obj["Value"].toDouble())
                );
            }
        }

#ifdef DEBUG
        for (auto const& val: data_)
        {
            qDebug() << val.first << " | " << val.second << '\n';
        }
#endif
        return data_;
    }
};
#endif // DATA_MANIPULATION_H
