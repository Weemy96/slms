#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDateTime CurrentDateTime= QDateTime::currentDateTime();
    ui->dateTimeEdit->setMinimumDateTime(CurrentDateTime);
}

MainWindow::~MainWindow()
{
    delete ui;
}
