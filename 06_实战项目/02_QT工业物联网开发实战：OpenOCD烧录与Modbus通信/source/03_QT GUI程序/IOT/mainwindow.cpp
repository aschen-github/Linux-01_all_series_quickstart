#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    /* burn button */
    qDebug() << "Burn Button clicked!";
}

void MainWindow::on_pushButton_2_clicked()
{
    /* test button */
    qDebug() << "Test Button clicked!";
}
