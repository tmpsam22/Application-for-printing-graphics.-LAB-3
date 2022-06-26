/// @file data_manipulation.h header-only файл
/// представлены реализации для чтения данных
/// из .sqlite, .json файлов

#pragma once

#ifndef DATA_MANIPULATION_H
#define DATA_MANIPULATION_H

#include <QtDebug>
#include <QtSql>

#include "message_box.h"
#include "common_container.h"

/// @brief тип файла
enum class type_file : int
{
    sql,
    json,

    unknown
};

/// @brief класс, в котором определена реализация чтение данных
/// для неизвестных форматов
template <type_file type>
struct dataManipulation
{
    container getData(const QString&/*path*/) const
    {
        return QVector<dataInContainer>{};
    }
};

/// @brief класс, в котором определена реализация чтение данных из .sqlite файла
template <>
struct dataManipulation<type_file::sql>
{
    container getData(const QString& path) const
    {
        static QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
        dbase.setDatabaseName(path);
        if (!dbase.open())
        {
            messageBox{ dbase.lastError().text() };
            return QVector<dataInContainer>{};
        }

        QVector<dataInContainer> data_;
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

/// @brief класс, в котором определена реализация чтение данных из .json файла
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
            return QVector<dataInContainer>{};
        }

        QString val;
        val = file.readAll();
        file.close();
        auto doc = QJsonDocument::fromJson(val.toUtf8());
        if (!doc.isArray())
        {
            messageBox{ "Expect json in an array" };
            return QVector<dataInContainer>{};
        }

        auto array_ = doc.array();
        QVector<dataInContainer> data_;
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
