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
#include <data_manipulation.h>
#include "message_box.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget{ parent }
    , currentPath{ QDir::homePath() }
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
    tableView->setRootIndex(fileModel->setRootPath(currentPath));

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
}

// draw diagram

void drawDiagram(const container& outputToDraw)
{

}

// slots
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
            && filePath.endsWith(".sqlite")
            && filePath.endsWith(".json");
    if (!isExpectedFile)
    {
        messageBox{"Expect .json or .sqlite"};
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

    if (data.empty())
    {
        messageBox{ "Data in file is empty" };
    }
}

MainWindow::~MainWindow()
{

}
