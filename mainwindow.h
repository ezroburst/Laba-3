#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"
#include "QDir"
#include "QGridLayout"
#include "QtCharts/QPieSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include"QComboBox"
#include"QSplitter"
#include"QTreeView"
#include"QMenuBar"
#include<QTableView>
#include"QToolBar"
#include"QStatusBar"
#include "strategy.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PrintTable(Element element);
    void PrintPieChart(Element element);
    void PrintBarChart(Element element);
public slots:
    void on_treeView_clicked(const QModelIndex &index);
void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_exit_triggered();

    void on_aboutApp_triggered();

    void reprint(const QModelIndex &, const QModelIndex &);

private:
   //MainWindow *ui;
    QFileSystemModel *model;
    QFileSystemModel *dirModel;
    QTableView *tableView;
    QWidget *centraWiget;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QSplitter *splitter;
    QTreeView *treeView;
    QWidget *widget;
    QMenuBar *menuBar;
    QMenu *menu;
    QAction *exit;
    QMenu *menu_2;
    QAction *aboutApp;
    QToolBar *meinToolBar;
    QStatusBar *statusBar;


};

#endif // MAINWINDOW_H
