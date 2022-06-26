#ifndef CHART_DRAWING_IF_H
#define CHART_DRAWING_IF_H

#include <memory>
#include "common_container.h"

// forward declaration
class QChart;

struct IChartDrawing
{
    virtual ~IChartDrawing() = default;

    virtual void drawChart(
            QChart* chart,
            const container& data,
            bool isColorized = true
    ) = 0;
};

std::shared_ptr<IChartDrawing> getChartDrawing();

#endif // CHART_DRAWING_IF_H
