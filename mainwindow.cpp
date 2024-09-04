#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tableModel = new QStandardItemModel(this);
    //tableModel->setColumnCount(2);
    tableModel->setHorizontalHeaderLabels(QStringList() << "Глубина" << "Значение") ;
    ui->tableView->setModel(tableModel);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    make_graph();
    ui->verticalScrollBar->setRange(787, 1400);



}

MainWindow::~MainWindow()
{
    delete ui;
    delete tableModel;

}
//Создание графа
void MainWindow::make_graph()
{
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    ui->widget->xAxis->setVisible(false);
    ui->widget->xAxis2->setVisible(true);
    ui->widget->yAxis->setRangeReversed(true);
    ui->widget->yAxis->setTickLabelPadding(50);
    ui->widget->yAxis->setTickLength(0,0);
    ui->widget->yAxis->setSubTickLength(0,0);
    ui->widget->yAxis->setPadding(50);

    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));

    //ui->widget->setInteraction(QCP::iRangeZoom,true);
    //ui->widget->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->axisRect()->setRangeDrag(Qt::Horizontal);

}

//метод наполнение таблицы QTableView значений кривых и глубин по индексу элемента из QListView
void MainWindow::fill_table()
{
tableModel->clear();
tableModel->setHorizontalHeaderLabels(QStringList() << "Глубина" << "Значение") ;

    for(int i=0;i<gislist[IndexList].GetGisData().size();++i){
        QList<QStandardItem *> standardItemsList;
        //standardItemsList.append(new QStandardItem(QString::number(gislist[0].GetGisData()[i])));
        //standardItemsList.push_back(new QStandardItem(QString::number(gislist[IndexList].GetGisData()[i]))));

        auto pDep=new QStandardItem(QString::number(gislist[0].GetGisData()[i]));
        pDep->setTextAlignment(Qt::AlignCenter);
       // pDep->set
        standardItemsList.append(pDep);

        auto pGisdata= new QStandardItem(QString::number(gislist[IndexList].GetGisData()[i]));
        pGisdata->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(pGisdata);

        tableModel->appendRow(standardItemsList);
        //delete pDep;

    }

}



//СЛОТ нажатия на кнопку открыть
void MainWindow::on_pushButton_clicked()
{
    QString path;
    //path=QFileDialog::getOpenFileName(this,"Открыть файл","..//data.csv","*.csv");
    path=QFileDialog::getExistingDirectory(this,"Открыть директорию с файлом data.csv")+"//data.csv";
    //QFileDialog a();
    if (path.isEmpty())
    {
    return;
    }
    for (auto matrix_tmp1 : prepare_data(path)) {
        GisClass topush(matrix_tmp1,prepare_data(path)[0]);
        this->gislist.push_back(topush);
    }
    fill_list_widget(gislist);
}

//СЛОТ меню Quit
void MainWindow::on_action_triggered()
{
    QApplication::quit();
}

//СЛОТ нажатия на кнопку отобразить
void MainWindow::on_pushButton_3_clicked()
{

    x.clear();
    y.clear();
    xMin=gislist[IndexList].GetMin();
    xMax=gislist[IndexList].GetMax();
    yMin=gislist[IndexList].GetMinDepVal().toDouble();
    yMax=gislist[IndexList].GetMaxDepVal().toDouble();



    ui->widget->xAxis2->setRange(xMin,xMax);


    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->axisRect()->setRangeDrag(Qt::Horizontal);

    QCPCurve *mycurve=new QCPCurve(ui->widget->xAxis2,ui->widget->yAxis);
    mycurve->setData(gislist[IndexList].GetGisData().toVector(),
                     gislist[IndexList].GetGisDepths().toVector());
    //ui->widget->addGraph(ui->widget->xAxis2,ui->widget->yAxis);
    //ui->widget->graph(0)->setData(gislist[IndexList].GetGisData().toVector(),
    //                              gislist[IndexList].GetGisDepths().toVector());


    ui->widget->xAxis2->rescale();
    ui->widget->xAxis->rescale();

    //ui->widget->yAxis->setScaleRatio(ui->widget->yAxis,0.01);
    //ui->widget->yAxis->moveRange(-285);
    //ui->widget->yAxis->se



    ui->widget->replot();
}

//СЛОТ нажатия на кнопку очистить
void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->clearPlottables();
    ui->widget->replot();
}



//Метод транспонирования матрицы с сырыми даными
void MainWindow::transpose(QList<QList<QByteArray> > &matrix){
    QList<QList<QByteArray>> temp_matrix(matrix[0].size());
    for (int i = 0; i <temp_matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {

            temp_matrix[i].push_back(matrix[j][i]);

        }
    }
    matrix=temp_matrix;

}

//Метод заполнения QListWidget данными из списка класса GisClass
void MainWindow::fill_list_widget(QList<GisClass> &gislist)
{
    for (int i=0;i<gislist.size();++i) {
        this->NamesForListWidget.push_back(gislist[i].GetGisName());
        QString tmp_str;
        if(gislist[i].GetOnliBlanksInData()){
            tmp_str="(only blanks)";
        }else{
            tmp_str="("+gislist[i].GetMinDepVal()+" - "+gislist[i].GetMaxDepVal()+")";
        }
        this->NamesForListWidget[i].append(tmp_str);
    }
    ui->listWidget->addItems(this->NamesForListWidget);




}

//Метод чтения файла и создания матрицы
QList<QList<QByteArray>> MainWindow::prepare_data(QString &path)
{
    QFile file(path);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
        QList<QList<QByteArray>> matrix;
        return matrix;
    }
    // Создаём поток для извлечения данных из файла
    QTextStream in(&file);
    //создаем список списков
    QList<QList<QByteArray>> matrix;
    // Считываем данные до конца файла построчно в список с разделителем и добавляем в список списков
    while (!in.atEnd()){
        matrix.push_back(file.readLine().split(';'));
    }
    // Транспонируем
    transpose(matrix);
    //qDebug() << matrix[1][1];
    file.close();
    return(matrix);

}


//СЛОТ обработки сигнала клика по элементу списка QListWidget
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    IndexList=(item->listWidget()->indexFromItem(item->listWidget()->currentItem())).row();
    QString label=this->NamesForListWidget[IndexList];
    ui->label_5->setText(label);
    ui->lineEdit->setText(QString::number(gislist[IndexList].GetMin()));
    ui->lineEdit_2->setText(QString::number(gislist[IndexList].GetMax()));
    fill_table();
    ui->pushButton_2->setDisabled(0);
    ui->pushButton_3->setDisabled(0);
}

void MainWindow::vertScrollBarChanged(int value)
{
    if (qAbs(ui->widget->yAxis->range().lower+value/1.0) > 0.01) // if user is dragging plot, we don't want to replot twice
      {
        ui->widget->yAxis->setRange(value, ui->widget->yAxis->range().size(), Qt::AlignLeft);
        ui->widget->replot();
    }
}

void MainWindow::yAxisChanged(QCPRange range)
{
   ui->verticalScrollBar->setValue(qRound(range.center()-range.size()/2)); // adjust position of scroll bar slider
   ui->verticalScrollBar->setPageStep(qRound(range.size()*0.001)); // adjust size of scroll bar slider

}

