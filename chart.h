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
    virtual void drawChart(const chartParameters& chartParameter) = 0;
};

struct barChartDrawing : ChartDrawing
{
    ~barChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter) override;
};

struct pieChartDrawing : ChartDrawing
{
    ~pieChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter) override;
};

struct chartParameters
{
    bool isColorized_;

    QChart* chart_;

    container data_;

    chartParameters()
        : isColorized_{ true }
        , chart_{ new QChart() }
        , data_{ }
    {

    }

    ~chartParameters()
    {
        delete chart_;
    }
};

class Chart
{
public:

    Chart() = default;

    virtual ~Chart() = default;

    QChart* getChart();

    void drawChart(const QString& title, const container& data);

    void cleanSeries();

    void switchColor();

private:

    void drawChart() const;

private:

    chartParameters parameters_;
};


#endif // CHART_H
