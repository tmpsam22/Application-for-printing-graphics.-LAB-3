/// @file data_reader_if.h представлен интерфейс извлечения данных из файлов разного типа
#pragma once

#ifndef DATA_IF_H
#define DATA_IF_H

#include <memory>
#include "common_container.h"

/// @brief интерфейс извлечения данных
struct IDataReader
{
    /// @brief деструктор
    virtual ~IDataReader() = default;

    /// @brief получение данных из файла
    /// @param path название файла для извлечения из нее данных
    /// @return контейнер с данными
    virtual container getData(const QString& path) = 0;
};

/// @brief получение интерфейса извлечения данных
std::shared_ptr<IDataReader> getDataReader();

#endif // DATA_IF_H
