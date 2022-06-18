#include "chart.h"
#include <iostream>
template<typename chartType>
void Chart::setupChart(const QString&/*title*/, const container&/*data*/)
{
    messageBox{"Drawing for this type for chart not implemented"};
}

template<>
void Chart::setupChart<QPieSeries>(const QString& title, const container& data)
{
    chart_->setTitle(title);
    QPieSeries *series = new QPieSeries{chart_};
    for (int i = 0; i < data.count(); i++) {
        series->append(data[i].first, data[i].second);
        series->slices().at(i);
    }
    chart_->addSeries(series);
    chart_->createDefaultAxes();
}

template<>
void Chart::setupChart<QBarSeries>(const QString& title, const container& data)
{
    chart_->setTitle(title);
    QBarSeries *series = new QBarSeries{chart_};
    for (int i = 0; i < data.count(); i++)
    {
        QBarSet *set = new QBarSet{data[i].first};
        *set << data[i].second;
        series->append(set);
    }
    chart_->addSeries(series);
    chart_->createDefaultAxes();
}

void Chart::drawChart(const QString& title, const container& data)
{
     qDebug() << "TYPEID:\t"<< typeid(decltype(IOCContainer::IOCContainerInstance().GetObject<QAbstractSeries>())).name();
     setupChart<std::decay_t<
             decltype(IOCContainer::IOCContainerInstance().GetObject<QAbstractSeries>())
             >
     >(title, data);
}

