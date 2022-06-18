#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include "themewidget.h"
#include "chart.h"

class MainWindow : public QWidget
{
	Q_OBJECT
private slots:

    void slotSelectionChanged(
            const QItemSelection &selected,
            const QItemSelection &deselected
    );

    void slotChooseDirectory();

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
};

#endif // MAINWINDOW_H
