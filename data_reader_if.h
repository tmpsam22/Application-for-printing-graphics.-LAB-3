#pragma once

#ifndef DATA_IF_H
#define DATA_IF_H

#include <memory>
#include "common_container.h"

struct IDataReader
{
    virtual ~IDataReader() = default;

    virtual container getData(const QString& path) = 0;
};

std::shared_ptr<IDataReader> getDataReader();

#endif // DATA_IF_H
