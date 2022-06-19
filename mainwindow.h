#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
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

    void slotChooseChartDraw();

    void slotColorSwitch();

    void slotSaveChartToPdf();

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

    QCheckBox* checkColor;

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
