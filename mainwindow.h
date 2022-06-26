#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTableView>
#include <QtCharts>
#include "chart.h"

class MainWindow : public QWidget
{
	Q_OBJECT
private slots:

    /// @brief слот, отрабатываемый при выборе файла
    void slotSelectionChanged(
            const QItemSelection &selected,
            const QItemSelection &deselected
    );

    /// @brief слот, отрабатываемый при выборе директории
    void slotChooseDirectory();

    /// @brief слот, отрабатываемый при выборе типа рисования диаграммы
    void slotChooseChartDraw();

    /// @brief слот, отрабатываемый при выборе цвета рисования диаграммы
    void slotColorSwitch();

    /// @brief слот, отрабатываемый при выборе директории для сохранения диаграммы
    /// в .pdf формате
    void slotSaveChartToPdf();

public:

    /// @brief конструктор
    explicit MainWindow(QWidget *parent = nullptr);

    /// @brief деструктор
    ~MainWindow() override;

private:

    // для взаимодействия с диаграммой
    struct
    {
        Chart* chart;
        QChartView* chartView;
    } chartManipulation;

    // текущий путь директории
    QString currentPath;

    // модель файловой системы
    QFileSystemModel* fileModel;

    // модель табличного представления
    QTableView* tableView;

    // лист с типами диаграммы
    QComboBox* boxType;

    // чекбокс для цвета диаграммы
    QCheckBox* checkColor;

    // вывод пути текущей директории
    // и мануала
    struct
    {
        QString serialize(const QString& Path)
        {
            return QString{"Current path: " + Path};
        }
        QLabel* labelInfo;
        QLabel* labelPath;
    } labelsOutput;
};

#endif // MAINWINDOW_H
