#include "chart.h"
#include "chart_drawing_impls.h"
#include <QtCharts>
#include <QChartView>
#include <QPdfWriter>
#include "data_reader_impls.h"

Chart::Chart()
    : chart_ { new QChart() }
    , chartView_ { new QChartView() }
    , isColorized_ { true } // по умолчанию диаграмма будет цветной
    , data_ { }
{
    chartView_->setRenderHint(QPainter::Antialiasing);
}

Chart::~Chart()
{
    delete chart_;
    delete chartView_;
}

QChart* Chart::getChart()
{
    return chart_;
}

QChartView* Chart::getChartView()
{
    return chartView_;
}

void Chart::reDrawChart() const
{
    if (data_.isEmpty())
    {
        qDebug() << "Can't draw a chart: data is empty";
        return;
    }

    getChartDrawing()->drawChart(
        chart_,
        data_,
        isColorized_
    );

    chartView_->setChart(chart_);
}

#ifdef TEST

namespace
{

const char* months[12] =
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "June",
    "July",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

}

container maxValuesInMonths(const container& data)
{
    QMap<QString, double> map_;
    QRegularExpression regex("^(\\d+).(\\d+).");
    for (auto& keyVal : data)
    {
        auto& date = keyVal.first;
        auto month = months[(regex.match(date).captured(2).toInt() - 1)];
        auto it = map_.find(month);

        if (it == map_.end())
        {
            map_.insert(month, keyVal.second);
            continue;
        }
        if (it.value() < keyVal.second)
        {
            it.value() = keyVal.second;
        }
    }
    container data_;
    for (auto it = map_.begin(); it != map_.end(); ++it)
    {
        data_.push_back(
                    qMakePair(
                        it.key(),
                        it.value()
                    )
        );
    }
    return data_;
    // return container{map_.begin(), map_.end()};
}

#endif

void Chart::drawChart(const QString& title, const QString& path)
{
    chart_->setTitle(title);

    data_ = getDataReader()->getData(path);

    #ifdef TEST
    data_ = maxValuesInMonths(data_);
    #endif

    reDrawChart();
}

void Chart::switchColor()
{
    isColorized_ = (isColorized_ == true) ? false : true;
}

void Chart::saveChartToPdf(const QString& path)
{
    QString path_{ path };
    if (path_.endsWith(".pdf"))
    {
        path_ = path_.split(".pdf").first();
    }

    QPdfWriter writer_{path_ + ".pdf"};
    writer_.setCreator("Creator");
    QPainter painter(&writer_);
    chartView_->render(&painter);
    painter.end();
}

void Chart::resetChar()
{
    chart_->removeAllSeries();
    data_.clear();
}

bool Chart::isDataEmpty() const
{
    return data_.isEmpty();
}
