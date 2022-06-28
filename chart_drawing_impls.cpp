#include "chart_drawing_impls.h"

#include <QtCharts>
#include "ioc_container.h"

void barChartDrawing::drawChart(
        QChart* chart,
        const container& data,
        bool isColorized
)
{
    QBarSeries *series = new QBarSeries{chart};
    for (int i = 0; i < data.count(); i++)
    {
        QBarSet *set = new QBarSet{data[i].first};
        if (!isColorized)
        {
            auto color_ = i % 2 ? Qt::black : Qt::gray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        *set << data[i].second;
        series->append(set);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->createDefaultAxes();
}

void pieChartDrawing::drawChart(
        QChart* chart,
        const container& data,
        bool isColorized
)
{
    QPieSeries *series = new QPieSeries{chart};
    auto dataCount = data.count();

    bool isOdd = false;
    if (!isColorized && (dataCount % 2))
    {
        isOdd = true;
    }

    for (int i = 0; i < dataCount; i++)
    {
        series->append(
                    data[i].first,
                    data[i].second
        );
        if (!isColorized)
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
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->createDefaultAxes();
}

std::shared_ptr<IChartDrawing> getChartDrawing()
{
    return IOCContainer::IOCContainerInstance().GetObject<IChartDrawing>();
}

void setChartDrawing(chart_type type)
{
    switch (type)
    {
        case chart_type::bar :
            IOCContainer::IOCContainerInstance().RegisterInstance<IChartDrawing, barChartDrawing>();
            break;
        case chart_type::pie :
            IOCContainer::IOCContainerInstance().RegisterInstance<IChartDrawing, pieChartDrawing>();
            break;
    }
}
