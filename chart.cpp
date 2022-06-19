#include "chart.h"

struct chartParameters
{
    bool isColorized;
    QChart* chart;
};

void barChartDrawing::drawChart(const chartParameters& chartParameter, const QString& title, const container& data)
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
    chartParameter.chart->setAnimationOptions(QChart::SeriesAnimations);
    chartParameter.chart->legend()->setAlignment(Qt::AlignRight);
    chartParameter.chart->createDefaultAxes();
}

void pieChartDrawing::drawChart(const chartParameters& chartParameter, const QString& title, const container& data)
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
    chartParameter.chart->setAnimationOptions(QChart::SeriesAnimations);
    chartParameter.chart->legend()->setAlignment(Qt::AlignRight);
    chartParameter.chart->createDefaultAxes();
}

Chart::Chart()
: isColorized_ { true }
, chart_{ new QChart() }
{
}
Chart::~Chart()
{
    delete chart_;
}

QChart* Chart::getChart()
{
    return chart_;
}

void Chart::drawChart(const QString& title, const container& data)
{
    IOCContainer::IOCContainerInstance().GetObject<ChartDrawing>()->drawChart(
                {isColorized_, chart_},
                title,
                data
    );
}

void Chart::cleanSeries()
{
    chart_->removeAllSeries();
}

void Chart::switchColor()
{
    isColorized_ = (isColorized_ == true) ? false : true;
}
