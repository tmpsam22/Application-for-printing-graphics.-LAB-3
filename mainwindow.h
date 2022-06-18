#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>

#include "themewidget.h"
#include "chart.h"
#include "data_manipulation.h"

class MainWindow : public QWidget
{
	Q_OBJECT
private slots:

    void slotSelectionChanged(
            const QItemSelection &selected,
            const QItemSelection &deselected
    );

    void slotChooseDirectory();

    void slotChooseChartDraw();

private:

    void setupChart(const container& dataToDraw);

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:

    struct
    {
        Chart* chart;
        QChartView* chartView;
    } chartManipulation;

    QString currentPath;

    QFileSystemModel* fileModel;

    QTableView* tableView;

    QComboBox* boxType;
};

#endif // MAINWINDOW_H
