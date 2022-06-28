/// @file data_reader_impls.h представлены реализации для извлечения данных из файлов разного типа
#pragma once

#ifndef DATA_READER_IMPLS_H
#define DATA_READER_IMPLS_H

#include "data_reader_if.h"

/// @brief извлечение данных из .sqlite
struct DataReaderSql : IDataReader
{
    /// @copydoc
    ~DataReaderSql() override = default;

    /// @copydoc
    container getData(const QString& path) override;
};

/// @brief извлечение данных из .json
struct DataReaderJson : IDataReader
{
    /// @copydoc
    ~DataReaderJson() override = default;

    /// @copydoc
    container getData(const QString& path) override;
};

/// @brief enum содержит тип файла
enum class file_type : int
{
    sql,
    json
};

/// @brief установка типа извлечения данных
void setDataReader(file_type type);

#endif // DATA_READER_IMPLS_H
