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
    // Настраиваем окно
    setWindowTitle("PrintChart"); // Название приложения
    setGeometry(50, 50, 1200, 600); // Задаем размеры окна

    // Вывод текста о текущей директории
    // и поддерживаемых типов у файла
    // * в нижней левой части главного окна
    labelsOutput.labelPath = new QLabel{};
    labelsOutput.labelInfo = new QLabel{};
    labelsOutput.labelInfo->setText("Choose .sqlite or .json format files");
    labelsOutput.labelPath->setText(labelsOutput.serialize(currentPath));

    // Определяем файловую систему
    fileModel = new QFileSystemModel{this};
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(currentPath);

    // Определяем вертикальные и горизонтальные компоновки
    auto layoutMain  = new QHBoxLayout{this};
    auto layoutOptions = new QHBoxLayout{};
    auto layoutVert = new QVBoxLayout{};
    auto layoutVertDir = new QVBoxLayout{};

    // Определяем кнопки
    auto buttonChooseDirectory = new QPushButton{"Choose directory", this};
    auto buttonWritePdf = new QPushButton{"Save to PDF", this};

    // Определяем сплиттеры
    auto splitter = new QSplitter(Qt::Horizontal);
    auto vertSplitter = new QSplitter(Qt::Vertical);

    // Определяем выпадающий список для выбора графика
    QStringList chartTypes = {"BarChart", "PieChart"};
    boxType = new QComboBox{};
    auto boxLabel = new QLabel{"Choose type of diagram", this};
    boxType->addItems(chartTypes);

    // Устанавливаем вывод диаграммы в виде гистограммы
    setChartDrawing(chart_type::bar);

    // Определяем флажок для выбора цвета
    checkColor = new QCheckBox("Black&White", this);

    // Табличное представление файлов
    tableView = new QTableView;
    tableView->setModel(fileModel);
    tableView->setRootIndex(fileModel->setRootPath(currentPath));

    // Настройка сплиттеров
    splitter->addWidget(tableView);
    vertSplitter->addWidget(chart->getChartView());

    // Настройка компоновок
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

    // Для отслеживания выбранных элементов
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // Подключение сигналов и слотов
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

// определение слотов

// определение слота сохранения диаграммы в файл формата .pdf
void MainWindow::slotSaveChartToPdf()
{
    // если данные пусты, то нечего печатать
    if (chart->isDataEmpty())
    {
        messageBox{"There is no chart to save .pdf format"};
        return;
    }

    // создаем диалоговое окно в текущей директорией
    // с сохранением файла
    QFileDialog dialog{this};
    dialog.setDirectory(currentPath);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    chart->saveChartToPdf(
                dialog.getSaveFileUrl().path()
    ); // сохраняем диаграмму в .pdf
}

// определение слота смены цвета диаграммы
void MainWindow::slotColorSwitch()
{
    chart->switchColor(); // меняем цвет у диаграммы
    chart->reDrawChart(); // перерисовка диаграммы
}

// определение слота выбора типа диаграммы
void MainWindow::slotChooseChartDraw()
{
    QString chartType{boxType->currentText()}; // вид диаграммы

    if (chartType.compare("PieChart") == 0) // вид диаграммы в виде круга
    {
        setChartDrawing(chart_type::pie); // устанавливаем отрисовку диаграммы
        chart->reDrawChart(); // перерисовка диаграммы
        return;
    }
    if (chartType.compare("BarChart") == 0) // вид диаграммы в виде гистограммы
    {
        setChartDrawing(chart_type::bar); // устанавливаем отрисовку диаграммы
        chart->reDrawChart(); // перерисовка диаграммы
        return;
    }
    // иначе отрисовка ещё не реализована
    messageBox{"there is no implementation for this type: "
               + chartType};
}

// определение слота выбора директории
void MainWindow::slotChooseDirectory()
{
    // создаем диалоговое окно с текущей директорией
    // для выбора директории
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory);
    if ( dialog.exec() )
    {
        // получаем директорию из диалогового окна
        currentPath = dialog.selectedFiles().first();
        // обновляем вывод о текущей директории
        labelsOutput.labelPath->setText(labelsOutput.serialize(currentPath));
    }
    // устанавливаем в табличное представление текущую директорию
    tableView->setRootIndex(fileModel->setRootPath(currentPath));
}

// определение слота выбора файла
void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    // определяем индекс у выбранного файла
    QModelIndexList indexs =  selected.indexes();
    if (indexs.count() < 1)
    {
        return;
    }

    // определяем путь к выбранному файлу
    QString filePath{ fileModel->filePath(indexs.constFirst()) };

    // проверка ожидаемого типа у файла
    bool isExpectedFile = true
            && (filePath.endsWith(".sqlite")
            || filePath.endsWith(".json"));
    if (!isExpectedFile)
    {
        // обнуляем диаграмму с данными
        chart->resetChar();
        // выводим пользователю соотвествующее сообщение о поддерживаемых типах
        messageBox{"Expect .json or .sqlite"};
        return;
    }

    // устанавливаем соотвествующий тип файла
    if (filePath.endsWith(".sqlite"))
    {
        setDataReader(file_type::sql);
    }
    if (filePath.endsWith(".json"))
    {
        setDataReader(file_type::json);
    }

    // т.к. предложенные данные очень большие, то для тестирования
    // сборка идёт с DEFINES += TEST
    // из данных выбираются максимальные значения по месяцам

    // рисуем диаграмму
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
