#ifndef CHART_H
#define CHART_H

#include "data_manipulation.h"
#include <QtCharts>
#include "message_box.h"

#include "ioc_container.h"

struct ChartDrawing
{
    virtual ~ChartDrawing() = default;
    virtual void drawChart(const QString& title, const container& data) = 0;
};

struct barChartDrawing : ChartDrawing
{
    ~barChartDrawing() override = default;
    void drawChart(const QString& title, const container& data) override
    {

    }
};


struct pieChartDrawing : ChartDrawing
{
    ~pieChartDrawing() override = default;
    virtual void drawChart(const QString& title, const container& data) = 0;
};

class Chart
{
public:
    Chart()
        : chart_{ new QChart() }
    {
        // default chart
        IOCContainer::IOCContainerInstance().RegisterInstance<QAbstractSeries, QBarSeries>();
    }
    virtual ~Chart()
    {
        delete chart_;
    }
    QChart* getChart()
    {
        return chart_;
    }
    void drawChart(const QString& title, const container& data);

private:

    template<typename T>
    void setupChart(const QString&/*title*/, const container&/*data*/);

private:

    QChart* chart_;
};


#endif // CHART_H
