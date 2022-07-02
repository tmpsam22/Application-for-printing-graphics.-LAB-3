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
    chartView_->setRenderHint(QPainter::Antialiasing); // включить сглаживание
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
    if (data_.isEmpty()) // если контейнер пуст, то выходим из функции
    {
        qDebug() << "Can't draw a chart: data is empty";
        return;
    }

    // рисуем диаграмму с помощью полученного интерфейса
    // получение интерфейса и реализация рисования диаграммы скрыты
    getChartDrawing()->drawChart(
        chart_,
        data_,
        isColorized_
    );

    chartView_->setChart(chart_); // отображение диаграммы
}


// т.к. предложенные данные очень большие, то для тестирования
// сборка идёт с DEFINES += TEST
// из данных выбираются максимальные значения по месяцам
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

/// функция выбирает максимальные значения в месяцах
/// @return  контейнер с 12 элементами
container maxValuesInMonths(const container& data)
{
    QMap<QString, double> map_;
    QRegularExpression regex("^(\\d+).(\\d+).");
    for (auto& keyVal : data)
    {
        auto& date = keyVal.first;
        auto month = months[(regex.match(date).captured(2).toInt() - 1)]; // получили месяц

        auto it = map_.find(month);
        if (it == map_.end()) // если такого месяца нет,то
        {
            // добавляем в контейнер месяц и его текущее значение
            map_.insert(month, keyVal.second);
            continue;
        }
        // иначе - проверяем значения в месяце
        // если текущее значение больше, то
        if (it.value() < keyVal.second)
        {
            // обновляем значение в этом месяце
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
    return data_; // возвращаем контейнер с данными
    // return container{map_.begin(), map_.end()};
}

#endif

void Chart::drawChart(const QString& title, const QString& path)
{
    chart_->setTitle(title); // устанвливаем название диаграммы

    // получаем данные из файла c помощью интерфейса извлечения данных
    // получение интерфейса и реализация извлечения данных скрыты
    data_ = getDataReader()->getData(path);

    #ifdef TEST
    // т.к. предложенные данные очень большие, то для тестирования
    // сборка идёт с DEFINES += TEST
    // из данных выбираются максимальные значения по месяцам
    data_ = maxValuesInMonths(data_); // данные с максимальными значениями в месяцах
    #endif

    reDrawChart(); // перерисовка диаграммы
}

void Chart::switchColor()
{
    isColorized_ = (isColorized_ == true) ? false : true;
}

void Chart::saveChartToPdf(const QString& path)
{
    QString path_{ path };
    if (path_.endsWith(".pdf")) // если пользователь ввел самостоятельно формат файла, то
        // пробуем получить имя без указанного формата
    {
        path_ = path_.split(".pdf").first();
    }

    QPdfWriter writer_{path_ + ".pdf"};
    writer_.setCreator("Creator");
    QPainter painter(&writer_);
    chartView_->render(&painter); // печатаем диаграмму
    painter.end();
}

void Chart::resetChar()
{
    chart_->removeAllSeries(); // удаляем все серии
    data_.clear(); // очищаем контейнер с данными
}

bool Chart::isDataEmpty() const
{
    return data_.isEmpty();
}
