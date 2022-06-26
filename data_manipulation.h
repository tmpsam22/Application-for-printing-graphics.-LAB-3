/// @file data_manipulation.h представлены реализации для чтения данных
/// из .sqlite, .json файлов

#pragma once

#ifndef DATA_MANIPULATION_H
#define DATA_MANIPULATION_H

#include "message_box.h"
#include "common_container.h"

namespace data_manipulation
{

/// @brief получить данные из файла
container getData(const QString& path);

} // data_manipulation

#endif // DATA_MANIPULATION_H
