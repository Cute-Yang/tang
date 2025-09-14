#pragma once

#include <QMainWindow>
#include <QChart>
#include <QPieSlice>

 class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){}
 
    void initChart();
    QWidget* s;

    QPieSeries* pie_series;
 
private slots:
    //点击
    void onPieSeriesClicked(QPieSlice*);

};
