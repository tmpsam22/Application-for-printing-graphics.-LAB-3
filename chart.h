#ifndef CHART_H
#define CHART_H

#include "data_manipulation.h"
#include <QtCharts>
#include "message_box.h"

#include "ioc_container.h"

struct chartParameters;

struct ChartDrawing
{
    virtual ~ChartDrawing() = default;
    virtual void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) = 0;
};

struct barChartDrawing : ChartDrawing
{
    ~barChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) override;
};

struct pieChartDrawing : ChartDrawing
{
    ~pieChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) override;
};

class Chart
{
public:

    Chart();

    virtual ~Chart();

    QChart* getChart();

    void drawChart(const QString& title, const container& data);

    void cleanSeries();

    void switchColor();

private:

    bool isColorized_;

    QChart* chart_;
};


#endif // CHART_H
