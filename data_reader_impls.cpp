#include <QtDebug>
#include <QtSql>
#include "data_reader_impls.h"
#include "message_box.h"
#include "ioc_container.h"

// чтение из .sqlite
container DataReaderSql::getData(const QString &path)
{
    // для соединения с базой данных необходимо определить драйвер
    static QSqlDatabase dbase {QSqlDatabase::addDatabase("QSQLITE")};
    dbase.setDatabaseName(path);

    // если файл не типа .sqlite
    // или файл не открылся
    // выводим соответствующее оконное сообщение
    if (!path.endsWith(".sqlite") || !dbase.open())
    {
        messageBox{ "File could not open!" };
        return container{};
    }

    container data_;

    // запрос на получение данных из sql таблицы
    QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
    while (query.next())
    {
        auto date_ = query.value(0).toString();
        auto value_ = query.value(1).toDouble();
        data_.push_back(qMakePair(date_, value_)); // добавляем данные в контейнер
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

    // если файл не типа .json
    // или файл не открылся
    // выводим соответствующее оконное сообщение
    if (!path.endsWith(".json") || !file.isOpen())
    {
        messageBox{ "File could not open!" };
        return container{};
    }

    QString val; // содержимое из файла
    val = file.readAll();
    file.close();
    auto doc = QJsonDocument::fromJson(val.toUtf8());

    /// @note
    /// программа "DB Browser for SQlite"
    /// при переводе данных из .sqlite в .json формат
    /// завернула все json-объекты в один массив

    if (!doc.isArray())
    {
        messageBox{ "Expect json in an array" };
        return container{};
    }

    auto array_ = doc.array();
    container data_;
    foreach (const QJsonValue & value, array_)
    {
        QJsonObject obj = value.toObject();

        // проверяем содержимое json-объекта
        if (obj.contains("Time") && obj.contains("Value"))
        {
            data_.push_back(
                        qMakePair(obj["Time"].toString(),
                        obj["Value"].toDouble())
            ); // добавляем данные в контейнер
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

// функция получения интерфейса извлечения данных
std::shared_ptr<IDataReader> getDataReader()
{
    return IOCContainer::IOCContainerInstance().GetObject<IDataReader>();
}

/// установка типа извлечения данных
void setDataReader(file_type type)
{
    switch (type)
    {
        case file_type::sql :
            IOCContainer::IOCContainerInstance().RegisterInstance<IDataReader, DataReaderSql>();
            break;
        case file_type::json :
            IOCContainer::IOCContainerInstance().RegisterInstance<IDataReader, DataReaderJson>();
            break;
    }
}

