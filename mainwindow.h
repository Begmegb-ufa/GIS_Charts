#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QTimer>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <qcustomplot.h>
#include <gisclass.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void transpose(QList<QList<QByteArray>> &matrix);
    void fill_list_widget(QList<GisClass> &gislist);
    QList<QList<QByteArray>> prepare_data(QString &path);

    void make_graph();
    void fill_table();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_action_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void vertScrollBarChanged(int value);

    void yAxisChanged(QCPRange range);

private:
    Ui::MainWindow *ui;
    //переменные графа
    double xBegin; //начало промежутка
    double xEnd; //конец промежутка
    double h; //шаг мужду двумя точками
    double X; //временная переменная для заполнения вектора
    int N; //колличество точек

    QVector<double> x,y; //массив для координат точек

    double xMin,xMax;
    int yMin, yMax;


    //переменная данных из файла
    //QList<QList<QByteArray>> matrix;

    //переменные списка
    QList <GisClass> gislist;
    QStringList NamesForListWidget;
    int IndexList;

    //переменные таблицы
    QStandardItemModel *tableModel;


};
#endif // MAINWINDOW_H
