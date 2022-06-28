/// @file chart_drawing_if.h представлен интерфейс рисования диаграммы
#pragma once

#ifndef CHART_DRAWING_IF_H
#define CHART_DRAWING_IF_H

#include <memory>
#include "common_container.h"

// forward declaration
class QChart;  // класс QT для работы с диаграммой

/// @brief интерфейс рисования диаграммы
struct IChartDrawing
{
    /// @brief деструктор
    virtual ~IChartDrawing() = default;

    /// @brief рисование диаграммы
    /// @param chart класс для работы с диаграммой
    /// @param data контейнер с данными для рисования диаграммы
    /// @param isColorized true - график будет цветным, иначе - ЧБ
    virtual void drawChart(
            QChart* chart,
            const container& data,
            bool isColorized = true
    ) = 0;
};

/// @brief получение интерфейса рисования диаграммы
std::shared_ptr<IChartDrawing> getChartDrawing();

#endif // CHART_DRAWING_IF_H
