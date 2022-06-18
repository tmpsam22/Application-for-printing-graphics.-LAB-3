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
#include <QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QWidget{ parent }
    , chartManipulation{ }
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
    auto layoutVert = new QVBoxLayout{};

    // declare buttons
    auto buttonChooseDirectory = new QPushButton{"Choose directory", this};
    auto buttonWritePdf = new QPushButton{"Save to PDF", this};

    // declare splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    QSplitter *vertSplitter = new QSplitter(Qt::Vertical);

    // declare combobox and setup for choose type of diagram
    QStringList diagrams = {"BarChart", "Pie"};
    auto boxType = new QComboBox{};
    auto boxLabel = new QLabel{"Choose type of diagram", this};
    boxType->addItems(diagrams);

    // declare checkBox for colorblind
    auto checkColor = new QCheckBox("Show Title", this);

    // declare chartView
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
}

// draft
/*
QBarSet *centralFedDistr = new QBarSet("Центральный");
    QBarSet *northwesternDistr = new QBarSet("Северо-Западный");
    QBarSet *southerhFedDistr = new QBarSet("Южный");
    QBarSet *northCaucasianFedDistr = new QBarSet("Северо-Кавказский");
    QBarSet *volgaFedDistr = new QBarSet("Приволжский");
    QBarSet *uralFedDistr = new QBarSet("Уральский");
    QBarSet *siberianFedDistr = new QBarSet("Сибирский");
    QBarSet *farEasternFedDistr = new QBarSet("Дальневосточный");

    *centralFedDistr        << 3378.83 << 3304.0 << 3261.0 << 2927.4;
    *northwesternDistr      << 1261.83 << 1268.6 << 1253.4 << 1096.3;
    *southerhFedDistr       << 823.45  << 821.8  << 781.5  << 821.6;
    *northCaucasianFedDistr << 295.39  << 313.7  << 312.8  << 197.1;
    *volgaFedDistr          << 2291.97 << 2312.6 << 2222.4 << 1992.1;
    *uralFedDistr           << 921.92  << 891.5  << 862.1  << 834.8;
    *siberianFedDistr       << 1349.41 << 1362.9 << 1313.8 << 1139.8;
    *farEasternFedDistr     << 455.14  << 469.7  << 458.0  << 382.7;


    QBarSeries *series = new QBarSeries();
    series->append(centralFedDistr);
    series->append(northwesternDistr);
    series->append(southerhFedDistr);
    series->append(northCaucasianFedDistr);
    series->append(volgaFedDistr);
    series->append(uralFedDistr);
    series->append(siberianFedDistr);
    series->append(farEasternFedDistr);

    QList<QBarSet *> sets = series->barSets();
    float currentHue = 0.0;
    for(int i = 0; i < sets.size(); ++i)
    {
        QColor col = QColor::fromHslF(currentHue, 0.7, 0.5);
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
        sets[i]->setColor(col);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Среднесписочная численность работников предприятий малого и среднего бизнеса по федеральным округам");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "01.01.2014" << "01.01.2015" << "01.01.2016" << "01.01.2017";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,4000);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto l  = new QHBoxLayout{this};
    l->addWidget(chartView);
    setLayout(l);
*/

// draw diagram


void MainWindow::setupChart(const container& dataToDraw)
{
    QBarSet *centralFedDistr = new QBarSet("Центральный");
        QBarSet *northwesternDistr = new QBarSet("Северо-Западный");
        QBarSet *southerhFedDistr = new QBarSet("Южный");
        QBarSet *northCaucasianFedDistr = new QBarSet("Северо-Кавказский");
        QBarSet *volgaFedDistr = new QBarSet("Приволжский");
        QBarSet *uralFedDistr = new QBarSet("Уральский");
        QBarSet *siberianFedDistr = new QBarSet("Сибирский");
        QBarSet *farEasternFedDistr = new QBarSet("Дальневосточный");

        *centralFedDistr        << 3378.83 << 3304.0 << 3261.0 << 2927.4;
        *northwesternDistr      << 1261.83 << 1268.6 << 1253.4 << 1096.3;
        *southerhFedDistr       << 823.45  << 821.8  << 781.5  << 821.6;
        *northCaucasianFedDistr << 295.39  << 313.7  << 312.8  << 197.1;
        *volgaFedDistr          << 2291.97 << 2312.6 << 2222.4 << 1992.1;
        *uralFedDistr           << 921.92  << 891.5  << 862.1  << 834.8;
        *siberianFedDistr       << 1349.41 << 1362.9 << 1313.8 << 1139.8;
        *farEasternFedDistr     << 455.14  << 469.7  << 458.0  << 382.7;


        QBarSeries *series = new QBarSeries();
        series->append(centralFedDistr);
        series->append(northwesternDistr);
        series->append(southerhFedDistr);
        series->append(northCaucasianFedDistr);
        series->append(volgaFedDistr);
        series->append(uralFedDistr);
        series->append(siberianFedDistr);
        series->append(farEasternFedDistr);

        QList<QBarSet *> sets = series->barSets();
        float currentHue = 0.0;
        for(int i = 0; i < sets.size(); ++i)
        {
            QColor col = QColor::fromHslF(currentHue, 0.7, 0.5);
            currentHue += 0.618033988749895f;
            currentHue = std::fmod(currentHue, 1.0f);
            sets[i]->setColor(col);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Среднесписочная численность работников предприятий малого и среднего бизнеса по федеральным округам");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QStringList categories;
        categories << "01.01.2014" << "01.01.2015" << "01.01.2016" << "01.01.2017";
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0,4000);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignRight);
        chartManipulation.chartView->setChart(chart);
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
            && (filePath.endsWith(".sqlite")
            || filePath.endsWith(".json"));
    if (!isExpectedFile)
    {
        messageBox{"Expect .json or .sqlite"};
        return;
    }

    auto data = dataManipulation<type_file::unknown>{}.getData( " " );
    qDebug() <<"File path:"<<filePath;
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
