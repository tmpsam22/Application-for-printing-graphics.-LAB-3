#include "data_manipulation.h"
#include <QtSql>

template <>
struct dataManipulation<type_file::sql>
{
    QVector<data> getData(const QString& path) const
    {
        QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
        dbase.setDatabaseName(path);
        if (!dbase.open())
        {
            // todo: simple output with message box
            qDebug() << dbase.lastError().text();
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
        for (auto& i : data_)
        {
            qDebug() << i << '\t';
        }
#endif
        return data_;
    }
};

template<>
struct dataManipulation<type_file::json>
{
    QVector<data> getData(const QString& path) const
    {
        QFile file;
        file.setFileName(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!file.isOpen())
        {
            // todo: msg box
            qDebug() << "File was not open!";
            return QVector<data>{};
        }

        QString val;
        val = file.readAll();
        file.close();
        auto doc = QJsonDocument::fromJson(val.toUtf8());
        if (!doc.isArray())
        {
            // todo: msg box
            qDebug() << "Expect json in an array";
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
