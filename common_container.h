/// @file common_container.h содержит алиасы данных
#pragma once

#ifndef COMMON_CONTAINER_H
#define COMMON_CONTAINER_H

#include <QString>
#include <QVector>
#include <QPair>

using dataInContainer = QPair<QString, double>;
using container = QVector<dataInContainer>;

#endif // COMMON_CONTAINER_H
