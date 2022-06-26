/// @file chart_drawing_impls.h представлены два класса рисования диаграммы
#pragma once

#ifndef CHART_DRAWING_IMPLS_H
#define CHART_DRAWING_IMPLS_H

#include "chart_drawing_if.h"

/// @brief класс рисования диаграммы в виде гистограммы
struct barChartDrawing : IChartDrawing
{
    /// @copydoc
    ~barChartDrawing() override = default;

    /// @copydoc
    void drawChart(
                QChart* chart,
                const container& data,
                bool isColorized
    ) override;
};

/// @brief класс рисования диаграммы в виде круга
struct pieChartDrawing : IChartDrawing
{
    /// @copydoc
    ~pieChartDrawing() override = default;

    /// @copydoc
    void drawChart(
                QChart* chart,
                const container& data,
                bool isColorized
    ) override;
};

/// @brief enum содержит тип рисования
enum class chart_type : int
{
    bar,
    pie
};

/// @brief установка типа рисования диаграммы
void setChartDrawing(chart_type type);

#endif // CHART_DRAWING_IMPLS_H
