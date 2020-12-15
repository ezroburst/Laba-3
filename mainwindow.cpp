#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "QTableView"
#include "QDir"
#include "QList"
#include "QGridLayout"
#include "QtCharts/QPieSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include "QBarSet"
#include "QBarSeries"
#include "QBarCategoryAxis"
#include "QValueAxis"
#include "vector"
#include <QVector>
#include <QTableView>
#include<QLabel>
#include <QMessageBox>
#include<QStatusBar>
#include "strategy.h"
#include <QAbstractItemModel>


#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
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

void printt(const QModelIndex &, const QModelIndex &)
{
    QMessageBox m;
    m.setText("text");
    m.show();
}

Element element;
using namespace QtCharts;
typedef QList<QString> DirList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    this->setGeometry(100,100,1000,700);
            this->setStatusBar(new QStatusBar(this));
           // this->statusBar()->showMessage("Choosen path: ");

            QString homePath = QDir::homePath();
            setWindowTitle(tr("File Manager"));
           // setupUi(this);
            model = new QFileSystemModel(this);
            model->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
            model->setRootPath(QDir::rootPath());


            treeView = new QTreeView();
           treeView->setModel(model);
    //treeView->expandToDepth(1);

          widget=new QWidget();
           widget->setContentsMargins(0,0,0,0);


           horizontalLayout=new QHBoxLayout;

           tableView = new QTableView;
           tableView->setModel(model);
            tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
           splitter = new QSplitter(parent);
           splitter->addWidget(treeView);
            splitter->addWidget(widget);
          // this->setCentralWidget(splitter);
            splitter->setOrientation(Qt::Orientation::Horizontal);
            QLabel *lable11 = new  QLabel();
            QLabel *lable22 = new  QLabel();
           // splitter->addWidget(lable11);
            //splitter->addWidget(treeView);
            splitter->addWidget(lable22);
          //  splitter->addWidget(tableView);


        comboBox=new QComboBox();
        comboBox_2=new QComboBox();


        comboBox->addItem("по папкам");
            comboBox->addItem("по файлам");

           comboBox_2->addItem("таблица");
            comboBox_2->addItem("диаграмма");
           comboBox_2->addItem("гистограмма");
            QLabel *lable1 = new  QLabel("Display in: ");
            QLabel *lable2 = new  QLabel("Group by: ");
            QSplitter *ssplitter = new QSplitter(parent);
            ssplitter->setOrientation(Qt::Orientation::Horizontal);
            ssplitter->addWidget(lable2);
            ssplitter->addWidget(comboBox);
            ssplitter->addWidget(lable1);
            ssplitter->addWidget(comboBox_2);
        this->setMenuWidget(ssplitter);
            this->setCentralWidget(splitter);


          //  treeView->model()->dataChanged(treeView->currentIndex(), treeView->currentIndex());
 QObject::connect(treeView,(&QTreeView::activated), this, [=](const QModelIndex &text) { on_treeView_clicked(text); });

//         QObject:: connect(model,
//                           SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this,
//                           SLOT(on_treeView_clicked(const QModelIndex &, const QModelIndex &)));
//         QObject:: connect(treeView,
//                           SIGNAL(expanded(const QModelIndex &, const QModelIndex &)), this,
//                           SLOT(on_treeView_clicked(const QModelIndex &, const QModelIndex &)));


            QObject::connect(comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_currentTextChanged(const QString &)));
    //
          QObject:: connect(comboBox_2, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_2_currentTextChanged(const QString &)));

           // groupBox->setLayout(vbox);


            QObject::connect(model,
                    SIGNAL(dataChanged ( const QModelIndex &, const QModelIndex &)),
                    this, SLOT(reprint(const QModelIndex &, const QModelIndex &)));

            //connect(comboBox_2, SIGNAL(toggled(bool)), this, SLOT(on_barToggled(bool)));
            QItemSelectionModel *selectionModel = treeView->selectionModel();
            connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
                    SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

}

MainWindow::~MainWindow()
{

}
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{ //Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndexList indexs = selected.indexes();
    QString filePath = ""; // Размещаем инфо в statusbar относительно выделенного модельного индекса
    if (indexs.count() >= 1)
    { QModelIndex ix = indexs.constFirst();
        filePath = model->filePath(ix);
      //  this->statusBar()->showMessage("Выбранный путь:"+dirModel->filePath(indexs.constFirst())); } //Установка крсора
    tableView->setRootIndex(model->setRootPath(filePath)); }
}
//сортировка элементов по возрастанию размера
void sortElement(Element& element)
{
    Element tmp = element;
    std::sort(element.elementSize.begin(), element.elementSize.end());
    for(int i=0; i<element.elementName.size(); i++)
    {
        for(int j=0; j<tmp.elementName.size(); j++)
        {
            if(element.elementSize[i]==tmp.elementSize[j])
            {
                if(element.elementName[i] != tmp.elementName[j])
                {
                    element.elementName[i] = tmp.elementName[j];
                    tmp.elementSize.remove(j);
                    tmp.elementName.removeAt(j);
                    break;
                }
            }
        }
    }
}

//выборка топ 5
void TopElement(Element& element)
{
    Element tmp;
    tmp.elementName.clear();
    tmp.elementSize.clear();
    if(element.elementName.size()>5)
    {
        tmp.elementName<<"other";
        tmp.elementSize<<0;
        for(int i=0; i<element.elementName.size()-4; i++)
        {
            tmp.elementSize[0]+=element.elementSize[i];
        }
        for(int i=element.elementName.size()-4; i<element.elementName.size(); i++)
        {
            tmp.elementName<<element.elementName[i];
            tmp.elementSize<<element.elementSize[i];
        }
        element.elementName.clear();
        element.elementSize.clear();
        element = tmp;
    }
}
//клик на дерево
void MainWindow::on_treeView_clicked(const QModelIndex &index )
{
    QFileInfo fileinfo = model->fileInfo(index);//получаем информацию о файле
    if(comboBox->currentText()=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
            PrintTable( element);
        if(comboBox_2->currentText()=="диаграмма")
            PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
            PrintBarChart(element);
        delete p;
    }
    if(comboBox->currentText()=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
            PrintTable( element);
        if(comboBox_2->currentText()=="диаграмма")
            PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
            PrintBarChart(element);
        delete p;
    }
}
QVBoxLayout *vlayout = new QVBoxLayout;
QHBoxLayout *hlayout = new QHBoxLayout;
//void recMethod(QDir directory, double& size)//запись
//{
//    QFileInfoList filesInfo = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);//предоставляет независимую от системы информацию о файле
//    QString dirName = directory.dirName();

//    if (filesInfo.size())
//    {
//        for (auto i : filesInfo)
//        {
//            size += i.size();
//        }
//    }

//    QStringList childs = directory.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
//    for (auto& c : childs)
//    {
//        QDir dir = QDir(directory.absolutePath() + "\\" + c);
//        recMethod(dir, size);
//    }
//}
//вывод информации в виде таблицы
void MainWindow::PrintTable(Element element)
{
QString filePath = "";


   // sortElement(element);
   // TopElement(element);
    //QTableView * tableView;
    tableView = new QTableView(widget);
    //Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("Имя");
    horizontalHeader.append("Процент");
    horizontalHeader.append("Размер");

    QStandardItemModel *Tmodel = new QStandardItemModel;
    QStandardItem *item;
    Tmodel->setHorizontalHeaderLabels(horizontalHeader);
    int rowcounter = 0;
  //  QFileInfo ii;
   // QFileInfoList filesInfo = element.elementName.en
    // QFileInfoList filesInfo = element.elementName.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
QDir directory = model->rootDirectory();//обеспечивает доступ к структурам каталогов и их содержимому
double size = 0;
   // for(int i=0; i<element.elementSize.size(); i++)
   // {
        // recMethod(directory, size);//считаем size
         size;

   // }
    for(int i=0; i<element.elementSize.size(); i++)
    {
        if(element.elementSize[i]!=0)
        {
            item = new QStandardItem(element.elementName[i]);
            Tmodel->setItem(rowcounter, 0, item);

            item = new QStandardItem(QString::number(element.elementSize[i]));
            Tmodel->setItem(rowcounter, 1, item);
            item = new QStandardItem(QString::number(element.elementPro[i]));//--------------------
            Tmodel->setItem(rowcounter, 2, item);

            rowcounter++;
            //item=new QStandardItem(element.elemrntProzent[i]);
        }
    }
    tableView->setModel(Tmodel);
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    //QVBoxLayout *vlayout = new QVBoxLayout;
    //QHBoxLayout *hlayout = new QHBoxLayout;
    if(vlayout->count()!=0)
    {
        while(vlayout->count())
        {
          QLayoutItem * item = vlayout->itemAt(0);
          vlayout->removeItem(item);
        }
    }
    vlayout->setMargin(0);
    hlayout->setMargin(0);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(tableView);
    widget->setLayout(vlayout);

}
//вывод информации в виде диаграммы
void MainWindow::PrintPieChart(Element element)
{
    sortElement(element);
    TopElement(element);
    QPieSeries *series = new QPieSeries();
    for(int i=0; i<element.elementName.size(); i++)
    {
        series->append(element.elementName[i], element.elementSize[i]);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pie Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if(vlayout->count()!=0)
    {
        while(vlayout->count())
        {
          QLayoutItem * item = vlayout->itemAt(0);
          vlayout->removeItem(item);
        }
    }
    vlayout->setMargin(0);
    hlayout->setMargin(0);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(chartView);
    widget->setLayout(vlayout);

}
//вывод инвормации в виде гистограммы
void MainWindow::PrintBarChart(Element element)
{
    sortElement(element);
    TopElement(element);
    QBarSeries *series = new QBarSeries();
    for(int i=0; i<element.elementName.size(); i++)
    {
        QBarSet *set0 = new QBarSet(element.elementName[i]);
        *set0<<element.elementSize[i];
        series->append(set0);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Bar Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QValueAxis *axisY = new QValueAxis();
    //    axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if(vlayout->count()!=0)
    {
        while(vlayout->count())
        {
          QLayoutItem * item = vlayout->itemAt(0);
          vlayout->removeItem(item);
        }
    }
    vlayout->setMargin(0);
    hlayout->setMargin(0);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(chartView);
    widget->setLayout(vlayout);


}

//изменение вида вывода информации
void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    statusBar=new QStatusBar();
    if(arg1=="таблица")
    {
        PrintTable( element);
        statusBar->showMessage("выбранна таблица");
    }
    if(arg1=="диаграмма")
    {
       PrintPieChart(element);
        statusBar->showMessage("выбранна диаграмма");
    }
    if(arg1=="гистограмма")
    {
        PrintBarChart(element);
        statusBar->showMessage("выбранна гистограмма");
    }
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
          PrintTable( element);
        if(comboBox_2->currentText()=="диаграмма")
           PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
            PrintBarChart(element);
        delete p;
    }
    if(arg1=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
           PrintTable(element);
        if(comboBox_2->currentText()=="диаграмма")
            PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
            PrintBarChart(element);
        delete p;
    }
}


void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_aboutApp_triggered()
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setWindowTitle("О программе");
    msgBox->setText("Какая то информация о программе");
    msgBox->exec();
    delete msgBox;
}

void MainWindow::reprint(const QModelIndex & index, const QModelIndex & index1)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if(comboBox->currentText()=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
            PrintTable( element);
        if(comboBox_2->currentText()=="диаграмма")
            PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
           PrintBarChart(element);
        delete p;
    }
    if(comboBox->currentText()=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(treeView->currentIndex(), model, element);
        if(comboBox_2->currentText()=="таблица")
           PrintTable( element);
        if(comboBox_2->currentText()=="диаграмма")
            PrintPieChart( element);
        if(comboBox_2->currentText()=="гистограмма")
            PrintBarChart(element);
        delete p;
    }
}
