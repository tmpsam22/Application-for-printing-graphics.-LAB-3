#include "chart.h"
#include "chart_drawing_impls.h"
#include <QtCharts>

Chart::Chart()
    : chart_ { new QChart() }
    , isColorized_ { true } // по умолчанию диаграмма будет цветной
    , data_ { }
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

void Chart::reDrawChart() const
{
    getChartDrawing()->drawChart(
        chart_,
        data_,
        isColorized_
    );
}

void Chart::drawChart(const QString& title, const container& data)
{
    chart_->setTitle(title);
    data_ = data;
    reDrawChart();
}

void Chart::cleanSeries()
{
   chart_->removeAllSeries();
}

void Chart::switchColor()
{
    isColorized_ = (isColorized_ == true) ? false : true;
}
