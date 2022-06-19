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
#include <QFileDialog>
#include "data_manipulation.h"
#include "message_box.h"
#include "chart.h"
#include <QValueAxis>

int IOCContainer::s_typeId = 17;

MainWindow::MainWindow(QWidget *parent)
    : QWidget{ parent }
    , chartManipulation{ }
    , currentPath{ QDir::homePath() }
    , boxType{ }
    , checkColor{ }
{
    // Window setup
    setGeometry(80, 80, 1000, 600);

    // file model
    fileModel = new QFileSystemModel{this};
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(currentPath);

    // declare layots
    auto layoutMain  = new QHBoxLayout{this};
    auto layoutOptions = new QHBoxLayout{};
    auto layoutVert = new QVBoxLayout{};

    // declare buttons
    auto buttonChooseDirectory = new QPushButton{"Choose directory", this};
    auto buttonWritePdf = new QPushButton{"Save to PDF", this};

    // declare splitter
    auto splitter = new QSplitter(Qt::Horizontal);
    auto vertSplitter = new QSplitter(Qt::Vertical);

    // declare combobox and setup for choose type of chart
    QStringList chartTypes = {"BarChart", "PieChart"};
    boxType = new QComboBox{};
    auto boxLabel = new QLabel{"Choose type of diagram", this};
    boxType->addItems(chartTypes);

    // declare default draw
    IOCContainer::IOCContainerInstance().RegisterInstance<ChartDrawing, barChartDrawing>();

    // declare checkBox
    checkColor = new QCheckBox("Black&White", this);

    // declare chartManipulation
    chartManipulation.chart = new Chart{};
    chartManipulation.chartView = new QChartView{};

    // table view
    tableView = new QTableView;
    tableView->setModel(fileModel);
    tableView->setRootIndex(fileModel->setRootPath(currentPath));

    // splitter setup
    splitter->addWidget(tableView);
    vertSplitter->addWidget(chartManipulation.chartView);

    // layout setup
    layoutOptions->stretch(1);
    layoutOptions->addWidget(boxLabel, 1, Qt::AlignLeft | Qt::AlignTop);
    layoutOptions->addWidget(boxType, 1, Qt::AlignLeft | Qt::AlignTop);
    layoutOptions->addWidget(checkColor, 1,  Qt::AlignTop);
    layoutOptions->addWidget(buttonWritePdf, 1,  Qt::AlignRight | Qt::AlignTop);
    layoutOptions->addWidget(buttonChooseDirectory, 1,  Qt::AlignRight | Qt::AlignTop);
    layoutVert->addLayout(layoutOptions);
    layoutMain->addWidget(splitter);
    layoutVert->addWidget(vertSplitter);
    layoutMain->addLayout(layoutVert);
    setLayout(layoutMain);

    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // setup signals
    connect(
            selectionModel,
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &))
    );

    connect(
            buttonChooseDirectory,
            &QPushButton::clicked,
            this,
            &MainWindow::slotChooseDirectory
    );

    connect(
            boxType,
            SIGNAL(currentTextChanged(const QString&)),
            this,
            SLOT(slotChooseChartDraw())
    );

    connect(
            checkColor,
            SIGNAL(clicked()),
            this,
            SLOT(slotColorSwitch())
    );

}
// slots
// switched color check
void MainWindow::slotColorSwitch()
{
    chartManipulation.chart->switchColor();
}

// draw chart
void MainWindow::setupChart(const container& dataToDraw)
{
    auto& chart = chartManipulation.chart;
    chart->drawChart("DEFAULT TITLE", dataToDraw);
    chartManipulation.chartView->setChart(chart->getChart());
}

void MainWindow::slotChooseChartDraw()
{
    QString chartType{boxType->currentText()};
    if (chartType.compare("PieChart") == 0)
    {
        IOCContainer::IOCContainerInstance().RegisterInstance<ChartDrawing, pieChartDrawing>();
        return;
    }
    if (chartType.compare("BarChart") == 0)
    {
        IOCContainer::IOCContainerInstance().RegisterInstance<ChartDrawing, barChartDrawing>();
        return;
    }
    messageBox{"there is no implementation for this type: "
               + chartType};
}

void MainWindow::slotChooseDirectory()
{
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory);
    if ( dialog.exec() )
    {
        currentPath = dialog.selectedFiles().first();
    }
    tableView->setRootIndex(fileModel->setRootPath(currentPath));
}

void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    if (indexs.count() < 1)
    {
        return;
    }

    QModelIndex ix =  indexs.constFirst();
    filePath = fileModel->filePath(ix);
    // @todo: insert ioc container, status bar
    //statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    bool isExpectedFile = true
            && (filePath.endsWith(".sqlite")
            || filePath.endsWith(".json"));
    if (!isExpectedFile)
    {
        messageBox{"Expect .json or .sqlite"};
        return;
    }

    auto data = dataManipulation<type_file::unknown>{}.getData( " " );

    if (filePath.endsWith(".sqlite"))
    {
        data = dataManipulation<type_file::sql>{}.getData(filePath);
    }
    if (filePath.endsWith(".json"))
    {
        data = dataManipulation<type_file::json>{}.getData(filePath);
    }

    if (data.isEmpty())
    {
        messageBox{ "Data in file is empty" };
        return;
    }
    setupChart(data);
}

MainWindow::~MainWindow()
{

}
