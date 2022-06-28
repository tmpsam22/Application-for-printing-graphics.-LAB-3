#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
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
#include "chart_drawing_impls.h"
#include "data_reader_impls.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget{ parent }
    , chart{ new Chart() }
    , currentPath{ QDir::homePath() }
    , boxType{ }
    , checkColor{ }
    , labelsOutput {}
{
    // Window setup
    setWindowTitle("PrintChart");
    setGeometry(50, 50, 1200, 600);

    // label setup
    labelsOutput.labelPath = new QLabel{};
    labelsOutput.labelInfo = new QLabel{};
    labelsOutput.labelInfo->setText("Choose .sqllite or .json format files");
    labelsOutput.labelPath->setText(labelsOutput.serialize(currentPath));

    // file model
    fileModel = new QFileSystemModel{this};
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(currentPath);

    // declare layots
    auto layoutMain  = new QHBoxLayout{this};
    auto layoutOptions = new QHBoxLayout{};
    auto layoutVert = new QVBoxLayout{};
    auto layoutVertDir = new QVBoxLayout{};

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
    setChartDrawing(chart_type::bar);

    // declare checkBox
    checkColor = new QCheckBox("Black&White", this);

    // table view
    tableView = new QTableView;
    tableView->setModel(fileModel);
    tableView->setRootIndex(fileModel->setRootPath(currentPath));

    // splitter setup
    splitter->addWidget(tableView);
    vertSplitter->addWidget(chart->getChartView());

    // layout setup
    layoutOptions->stretch(1);
    layoutOptions->addWidget(boxLabel, 1, Qt::AlignLeft | Qt::AlignTop);
    layoutOptions->addWidget(boxType, 1, Qt::AlignLeft | Qt::AlignTop);
    layoutOptions->addWidget(checkColor, 1,  Qt::AlignTop);
    layoutOptions->addWidget(buttonWritePdf, 1,  Qt::AlignRight | Qt::AlignTop);
    layoutOptions->addWidget(buttonChooseDirectory, 1,  Qt::AlignRight | Qt::AlignTop);
    layoutVert->addLayout(layoutOptions);

    layoutVertDir->addWidget(splitter, 1);
    layoutVertDir->addWidget(labelsOutput.labelInfo, 0, Qt::AlignLeft | Qt::AlignBottom);
    layoutVertDir->addWidget(labelsOutput.labelPath, 0, Qt::AlignLeft | Qt::AlignBottom);
    layoutMain->addLayout(layoutVertDir);

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
            buttonWritePdf,
            &QPushButton::clicked,
            this,
            &MainWindow::slotSaveChartToPdf
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

// слоты
void MainWindow::slotSaveChartToPdf()
{
   if (!chart->isDataEmpty())
   {
        QFileDialog dialog{this};
        dialog.setDirectory(currentPath);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        return chart->saveChartToPdf(
                    dialog.getSaveFileUrl().path()
        );
    }
    messageBox{"There is no chart to save .pdf format"};
}

void MainWindow::slotColorSwitch()
{
    chart->switchColor();
    chart->reDrawChart();
}

void MainWindow::slotChooseChartDraw()
{
    QString chartType{boxType->currentText()};
    if (chartType.compare("PieChart") == 0)
    {
        setChartDrawing(chart_type::pie);
        chart->reDrawChart();
        return;
    }
    if (chartType.compare("BarChart") == 0)
    {
        setChartDrawing(chart_type::bar);
        chart->reDrawChart();
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
        labelsOutput.labelPath->setText(labelsOutput.serialize(currentPath));
    }
    tableView->setRootIndex(fileModel->setRootPath(currentPath));
}

void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();
    if (indexs.count() < 1)
    {
        return;
    }

    QString filePath{""};
    filePath = fileModel->filePath(indexs.constFirst());

    bool isExpectedFile = true
            && (filePath.endsWith(".sqlite")
            || filePath.endsWith(".json"));
    if (!isExpectedFile)
    {
        chart->resetChar();
        messageBox{"Expect .json or .sqlite"};
        return;
    }

    if (filePath.endsWith(".sqlite"))
    {
        setDataReader(file_type::sql);
    }
    if (filePath.endsWith(".json"))
    {
        setDataReader(file_type::json);
    }

    chart->drawChart(
           #ifdef TEST
           "Max values in months"
           #else
           "Default data output"
           #endif
           , filePath
    );
}

MainWindow::~MainWindow()
{

}
