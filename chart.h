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

    /// @brief получить qt-шный класс для отображения диаграммы
    QChartView* getChartView();

    /// @brief рисование диаграммы
    /// @param title название диаграммы
    /// @param path название файла с данными
    void drawChart(const QString& title, const QString& path);

    /// @brief перерисовка диаграммы
    void reDrawChart() const;

    /// @brief смена цвета диаграммы на ЧБ//цветной
    /// @note необходимо перерисовать диаграмму после применения данной функции
    void switchColor();

    /// @brief сохрание диаграммы в pdf
    /// @param path путь для сохранения файла
    void saveChartToPdf(const QString& path);

    /// @brief очистка диаграммы вместе с данными
    void resetChar();

    /// @brief проверка на пустоту данных
    /// @return true - если данные пусты, иначе - false
    bool isDataEmpty() const;

private:

    // для работы с диаграммой
    QChart* chart_;

    // для отображения диаграммы
    QChartView* chartView_;

    // контраст цвета диаграммы
    bool isColorized_;

    // контейнер с данными для отрисовки
    container data_;
};


#endif // CHART_H
