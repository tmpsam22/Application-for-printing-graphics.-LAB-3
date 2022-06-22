#include "chart.h"

void barChartDrawing::drawChart(const chartParameters& chartParameter)
{
    QBarSeries *series = new QBarSeries{chartParameter.chart_};
    for (int i = 0; i < chartParameter.data_.count(); i++)
    {
        QBarSet *set = new QBarSet{chartParameter.data_[i].first};
        if (!chartParameter.isColorized_)
        {
            auto color_ = i % 2 ? Qt::black : Qt::gray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        *set << chartParameter.data_[i].second;
        series->append(set);
    }
    chartParameter.chart_->removeAllSeries();
    chartParameter.chart_->addSeries(series);
    chartParameter.chart_->setAnimationOptions(QChart::SeriesAnimations);
    chartParameter.chart_->legend()->setAlignment(Qt::AlignRight);
    chartParameter.chart_->createDefaultAxes();
}

void pieChartDrawing::drawChart(const chartParameters& chartParameter)
{
    QPieSeries *series = new QPieSeries{chartParameter.chart_};
    auto dataCount = chartParameter.data_.count();

    bool isOdd = false;
    if (!chartParameter.isColorized_ && (dataCount % 2))
    {
        isOdd = true;
    }

    for (int i = 0; i < dataCount; i++)
    {
        series->append(
                    chartParameter.data_[i].first,
                    chartParameter.data_[i].second
        );
        if (!chartParameter.isColorized_)
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
    chartParameter.chart_->removeAllSeries();
    chartParameter.chart_->addSeries(series);
    chartParameter.chart_->setAnimationOptions(QChart::SeriesAnimations);
    chartParameter.chart_->legend()->setAlignment(Qt::AlignRight);
    chartParameter.chart_->createDefaultAxes();
}

QChart* Chart::getChart()
{
    return parameters_.chart_;
}

void Chart::reDrawChart() const
{
    IOCContainer::IOCContainerInstance().GetObject<ChartDrawing>()->drawChart(
        parameters_
    );
}

void Chart::drawChart(const QString& title, const container& data)
{
    parameters_.chart_->setTitle(title);
    parameters_.data_ = data;
    reDrawChart();
}

void Chart::cleanSeries()
{
    parameters_.chart_->removeAllSeries();
}

void Chart::switchColor()
{
    parameters_.isColorized_ = (parameters_.isColorized_ == true) ? false : true;
}
