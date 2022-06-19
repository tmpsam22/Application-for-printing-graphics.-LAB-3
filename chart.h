#ifndef CHART_H
#define CHART_H

#include "data_manipulation.h"
#include <QtCharts>
#include "message_box.h"

#include "ioc_container.h"

struct chartParameters
{
    bool isColorized;
    QChart* chart;
};

struct ChartDrawing
{
    virtual ~ChartDrawing() = default;
    virtual void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) = 0;
};

struct barChartDrawing : ChartDrawing
{
    ~barChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) override
    {
        chartParameter.chart->setTitle(title);
        QBarSeries *series = new QBarSeries{chartParameter.chart};
        for (int i = 0; i < data.count(); i++)
        {
            QBarSet *set = new QBarSet{data[i].first};
            if (!chartParameter.isColorized)
            {
                auto color_ = i % 2 ? Qt::black : Qt::gray;
                set->setBrush(QBrush(color_, Qt::SolidPattern));
            }
            *set << data[i].second;
            series->append(set);
        }
        chartParameter.chart->removeAllSeries();
        chartParameter.chart->addSeries(series);
        chartParameter.chart->createDefaultAxes();
    }
};

struct pieChartDrawing : ChartDrawing
{
    ~pieChartDrawing() override = default;
    void drawChart(const chartParameters& chartParameter, const QString& title, const container& data) override
    {
        chartParameter.chart->setTitle(title);
        QPieSeries *series = new QPieSeries{chartParameter.chart};

        auto dataCount = data.count();
        bool isOdd = false;
        if (!chartParameter.isColorized && (dataCount % 2))
        {
            isOdd = true;
        }

        for (int i = 0; i < dataCount; i++)
        {
            series->append(data[i].first, data[i].second);
            if (!chartParameter.isColorized)
            {
                auto color_ = i % 2 ? Qt::black : Qt::gray;
                if (isOdd && (i == dataCount - 1))
                {
                    color_ = Qt::darkGray;
                }
                series->slices().at(i)->setBrush(QBrush(color_, Qt::SolidPattern));
            }
            series->slices().at(i);
        }
        chartParameter.chart->removeAllSeries();
        chartParameter.chart->addSeries(series);
        chartParameter.chart->createDefaultAxes();
    }
};

class Chart
{
public:

    Chart()
        : isColorized_ { true }
        , chart_{ new QChart() }
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
        IOCContainer::IOCContainerInstance().GetObject<ChartDrawing>()->drawChart(
                    {isColorized_, chart_},
                    title,
                    data
        );
    }

    void switchColor()
    {
        isColorized_ = (isColorized_ == true) ? false : true;
    }

private:
    bool isColorized_;
    QChart* chart_;
};


#endif // CHART_H
