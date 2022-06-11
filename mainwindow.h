#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include "themewidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
private slots:

	void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
    void testInit(QWidget*);
private:
	QFileSystemModel *fileModel;
	QFileSystemModel *dirModel;
	QTreeView *treeView;
	QTableView *tableView;
    ThemeWidget *themeWidget;
};

#endif // MAINWINDOW_H
