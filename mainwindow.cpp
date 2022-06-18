#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QSplitter>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    // Window setup
    setGeometry(80, 80, 1000, 600);

    // Path
    QString homePath = QDir::homePath();

    // file model
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);

    // declare layots
    auto layoutMain  = new QHBoxLayout{this};
    auto layoutOptions = new QHBoxLayout{};

    // declare buttons
    auto buttonChooseDirectory = new QPushButton{"Choose directory", this};
    auto buttonWritePdf = new QPushButton{"Save to PDF", this};

    // declare splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // declare combobox and setup for choose type of diagram
    QStringList diagrams = {"BarChart", "Pie"};
    auto boxType = new QComboBox{};
    auto boxLabel = new QLabel{"Choose type of diagram", this};
    boxType->addItems(diagrams);

    // declare checkBox for colorblind
    auto checkColor = new QCheckBox("Show Title", this);

    // table view
    tableView = new QTableView;
    tableView->setModel(fileModel);
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // signals
    connect(
            selectionModel,
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &))
    );

    // splitter setup
    splitter->addWidget(tableView);

    // layout setup
    layoutOptions->stretch(1);
    layoutOptions->addWidget(splitter);
    layoutOptions->addWidget(boxLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutOptions->addWidget(boxType, 0, Qt::AlignTop);
    layoutOptions->addWidget(checkColor, 0, Qt::AlignTop);
    layoutOptions->addWidget(buttonWritePdf, 0, Qt::AlignTop);
    layoutOptions->addWidget(buttonChooseDirectory, 0,  Qt::AlignRight | Qt::AlignTop);
    layoutMain->addLayout(layoutOptions);
    setLayout(layoutMain);

}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndex index = tableView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        //statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    }

    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
    int length = 200;
    int dx = 30;

    if (fileModel->fileName(index).length() * dx > length) {
        length = length + fileModel->fileName(index).length() * dx;
        qDebug() << "r = " << index.row() << "c = " << index.column() << fileModel->fileName(index) << fileModel->fileInfo(
                     index).size();

    }
}

MainWindow::~MainWindow()
{

}
