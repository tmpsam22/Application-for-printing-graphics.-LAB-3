#include <QtDebug>
#include <QtSql>
#include "data_reader_impls.h"
#include "message_box.h"
#include "ioc_container.h"

// чтение из .sqlite
container DataReaderSql::getData(const QString &path)
{
    static QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
    dbase.setDatabaseName(path);
    if (!path.endsWith(".sqlite") || !dbase.open())
    {
        messageBox{ "File could not open!" };
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
container DataReaderJson::getData(const QString &path)
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!path.endsWith(".json") || !file.isOpen())
    {
        messageBox{ "File could not open!" };
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

std::shared_ptr<IDataReader> getDataReader()
{
    return IOCContainer::IOCContainerInstance().GetObject<IDataReader>();
}

void setDataReader(data_type type)
{
    switch (type)
    {
        case data_type::sql :
            IOCContainer::IOCContainerInstance().RegisterInstance<IDataReader, DataReaderSql>();
            break;
        case data_type::json :
            IOCContainer::IOCContainerInstance().RegisterInstance<IDataReader, DataReaderJson>();
            break;
    }
}

