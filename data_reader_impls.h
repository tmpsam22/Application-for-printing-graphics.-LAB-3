#ifndef DATA_READER_IMPLS_H
#define DATA_READER_IMPLS_H

#include "data_reader_if.h"

struct DataReaderSql : IDataReader
{
    ~DataReaderSql() override = default;
    container getData(const QString& path) override;
};

struct DataReaderJson : IDataReader
{
    ~DataReaderJson() override = default;
    container getData(const QString& path) override;
};

enum class data_type : int
{
    sql,
    json
};

void setDataReader(data_type type);

#endif // DATA_READER_IMPLS_H
