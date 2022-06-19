#ifndef CHART_H
#define CHART_H

#include "data_manipulation.h"
#include <QtCharts>
#include "message_box.h"

#include "ioc_container.h"

struct ChartDrawing
{
    virtual ~ChartDrawing() = default;
    virtual void drawChart(QChart* chart, const QString& title, const container& data) = 0;
};

struct barChartDrawing : ChartDrawing
{
    ~barChartDrawing() override = default;
    void drawChart(QChart* chart, const QString& title, const container& data) override
    {
        chart->setTitle(title);
        QBarSeries *series = new QBarSeries{chart};
        for (int i = 0; i < data.count(); i++)
        {
            QBarSet *set = new QBarSet{data[i].first};
            *set << data[i].second;
            series->append(set);
        }
        chart->removeAllSeries();
        chart->addSeries(series);
        chart->createDefaultAxes();
    }
};


struct pieChartDrawing : ChartDrawing
{
    ~pieChartDrawing() override = default;
    void drawChart(QChart* chart, const QString& title, const container& data) override
    {
        chart->setTitle(title);
        QPieSeries *series = new QPieSeries{chart};
        for (int i = 0; i < data.count(); i++) {
            series->append(data[i].first, data[i].second);
            series->slices().at(i);
        }
        chart->removeAllSeries();
        chart->addSeries(series);
        chart->createDefaultAxes();
    }
};

class Chart
{
public:

    Chart()
        : chart_{ new QChart() }
    {
    }

    virtual ~Chart()
    {
        delete chart_;
    }

    QChart* getChart()
    {
        return chart_;
    }

    void drawChart(const QString& title, const container& data)
    {
        IOCContainer::IOCContainerInstance().GetObject<ChartDrawing>()->drawChart(chart_, title, data);
    }

private:

    QChart* chart_;
};


#endif // CHART_H
