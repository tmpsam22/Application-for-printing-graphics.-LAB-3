#include <QtDebug>
#include <QtSql>
#include "data_manipulation.h"
#include "message_box.h"

namespace data_manipulation
{

// чтение из .sqlite
container getDataFromSql(const QString& path)
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

// чтение из .json
container getDataFromJson(const QString& path)
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

container getData(const QString& path)
{
    if (path.endsWith(".sqlite"))
    {
        return getDataFromSql(path);
    }
    if (path.endsWith(".json"))
    {
        return getDataFromJson(path);
    }
    messageBox{"Expect .json or .sqlite"};
    return container{};
}

} // data_manipulation
