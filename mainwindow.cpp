#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtDebug>

void setMinTimeForLeaveBox();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinTimeForLeaveBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_clear_clicked() //clear and reset input
{
    ui->txt_id->clear();
    ui->txt_name->clear();
    ui->txt_remark->clear();
    ui->txt_day_num_box->setValue(1);
    setMinTimeForLeaveBox();
}

void MainWindow::setMinTimeForLeaveBox()
{
    QDateTime CurrentDateTime = QDateTime::currentDateTime(); //get current DateTime when run programm.
    ui->txt_leave_datetime_box->setMinimumDateTime(CurrentDateTime); //set time to minimum Date and Time.
    ui->txt_leave_datetime_box->setDateTime(CurrentDateTime); //display date time for current
    ui->txt_return_back_date_time->setDateTime(CurrentDateTime.addDays(1)); //add 1 day by default for day/s number_box
}

void MainWindow::on_txt_day_num_box_valueChanged(int days) //when day/s number_box change, it will sync change "back on" date time
{
    QDateTime leaveDateTime = ui->txt_leave_datetime_box->dateTime();
    ui->txt_return_back_date_time->setDateTime(leaveDateTime.addDays(days));
}
