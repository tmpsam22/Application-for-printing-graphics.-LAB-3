/// @file chart.h представлен класс для взаимодействия с диаграммой
#pragma once

#ifndef CHART_H
#define CHART_H

#include "common_container.h"

// forward declaration
class QChart; // класс QT для работы с диаграммами
class QChartView; // класс Qt для отображения диаграммы

///  @brief класс для взаимодействия с диаграммой
class Chart
{
public:

    /// @brief конструктор
    Chart();

    /// @brief деструктор
    virtual ~Chart();

    /// @brief получить qt-шный класс для работы с диаграммой
    /// @return QChart
    QChart* getChart();

    QChartView* getChartView();

    /// @brief рисование диаграммы
    /// @param title название диаграммы
    // ! !!!! ! ! ~!! ! !
    void drawChart(const QString& title, const QString& path);

    /// @brief перерисовка диаграммы
    void reDrawChart() const;

    /// @brief смена цвета диаграммы на ЧБ//цветной
    /// @note необходимо перерисовать диаграмму после применения данной функции
    void switchColor();

    void saveChartToPdf(const QString& path);

    void resetChar();

    bool isDataEmpty() const;

private:

    // для работы с диаграммой
    QChart* chart_;

    QChartView* chartView_;

    // контраст цвета диаграммы
    bool isColorized_;

    // контейнер с данными для отрисовки
    container data_;
};


#endif // CHART_H
