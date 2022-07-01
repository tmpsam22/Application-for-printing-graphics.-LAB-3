#include "chart_drawing_impls.h"

#include <QtCharts>
#include "ioc_container.h"

void barChartDrawing::drawChart(
        QChart* chart,
        const container& data,
        bool isColorized
)
{
    // серия данных в виде гистограммы
    QBarSeries *series = new QBarSeries{chart};
    for (int i = 0; i < data.count(); i++)
    {
        // определяем столбец
        QBarSet *set = new QBarSet{data[i].first}; //  ключ
        if (!isColorized)
        {
            auto color_ = i % 2 ? Qt::black : Qt::gray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        *set << data[i].second; // значение
        series->append(set); // добавляем в серию
    }
    chart->removeAllSeries(); // очищаем предыдущие серию
    chart->addSeries(series); // добавляем новую серию
    chart->setAnimationOptions(QChart::SeriesAnimations); // вывод диаграммы с анимацией
    chart->legend()->setAlignment(Qt::AlignRight); // ключи будут справа от диаграммы
    chart->createDefaultAxes(); // оси
}

void pieChartDrawing::drawChart(
        QChart* chart,
        const container& data,
        bool isColorized
)
{
    // серия данных в виде круговой диаграммы
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
        ); // добавление ключа и значения в серию
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
    chart->removeAllSeries(); // очищаем предыдущие серию
    chart->addSeries(series); // добавляем новую серию
    chart->setAnimationOptions(QChart::SeriesAnimations); // вывод диаграммы с анимацией
    chart->legend()->setAlignment(Qt::AlignRight); // ключи будут справа от диаграммы
    chart->createDefaultAxes(); // оси
}

/// получение интерфейса рисования диаграммы
std::shared_ptr<IChartDrawing> getChartDrawing()
{
    return IOCContainer::IOCContainerInstance().GetObject<IChartDrawing>();
}

/// установка типа рисования диаграммы
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
