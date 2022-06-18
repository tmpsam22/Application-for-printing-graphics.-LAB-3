#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include "themewidget.h"

class MainWindow : public QWidget
{
	Q_OBJECT
private slots:

    void slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void slotChooseDirectory();

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
    QString currentPath;
    QFileSystemModel *fileModel;
    QTableView *tableView;
};

#endif // MAINWINDOW_H
