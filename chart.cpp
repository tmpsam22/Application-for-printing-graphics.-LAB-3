#include "chart.h"
#include "chart_drawing_impls.h"
#include <QtCharts>
#include "data_reader_impls.h"

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

    getChartDrawing()->drawChart(
        chart_,
        data_,
        isColorized_
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
