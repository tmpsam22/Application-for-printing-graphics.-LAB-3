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

	void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
    QFileSystemModel *fileModel;
    QTableView *tableView;
};

#endif // MAINWINDOW_H
