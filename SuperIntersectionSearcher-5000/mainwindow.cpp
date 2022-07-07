#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->graphic_widget, SIGNAL(distanceChanged(float)), this, SLOT(setDistance(float)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDistance(float distance)
{
    ui->lineEdit_Distance->setText(QString::number(distance));
}

