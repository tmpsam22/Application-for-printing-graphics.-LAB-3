#ifndef CHART_DRAWING_IMPLS_H
#define CHART_DRAWING_IMPLS_H

#include "chart_drawing_if.h"

struct barChartDrawing : IChartDrawing
{
    ~barChartDrawing() override = default;

    void drawChart(
                QChart* chart,
                const container& data,
                bool isColorized
    ) override;
};

struct pieChartDrawing : IChartDrawing
{
    ~pieChartDrawing() override = default;

    void drawChart(
                QChart* chart,
                const container& data,
                bool isColorized
    ) override;
};

enum class chart_type : int
{
    bar,
    pie
};

void setChartDrawing(chart_type type);

#endif // CHART_DRAWING_IMPLS_H
