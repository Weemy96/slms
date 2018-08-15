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
    ui->txt_day_num_box->setValue(0);
    setMinTimeForLeaveBox();
}

void MainWindow::setMinTimeForLeaveBox()
{
    QDateTime CurrentDateTime = QDateTime::currentDateTime(); //get current DateTime when run programm.
    ui->txt_leave_datetime_box->setMinimumDateTime(CurrentDateTime); //set time to minimum Date and Time.
    ui->txt_leave_datetime_box->setDateTime(CurrentDateTime); //display date time for current
    ui->txt_return_back_date_time->setMinimumDateTime(CurrentDateTime.addSecs(3600)); //1 hour min leave time
    ui->txt_return_back_date_time->setMaximumDateTime(CurrentDateTime.addDays(1095)); //limit 3y
    ui->txt_return_back_date_time->setDateTime(CurrentDateTime.addSecs(3600)); //add 1 hour min leave time
}


void MainWindow::on_txt_leave_datetime_box_dateTimeChanged(const QDateTime &getLeaveOnDateTime)
{
    ui->txt_return_back_date_time->setDateTime(getLeaveOnDateTime.addSecs(3600));
    ui->txt_return_back_date_time->setMinimumDateTime(getLeaveOnDateTime.addSecs(3600));
    ui->txt_return_back_date_time->setMaximumDateTime(getLeaveOnDateTime.addDays(1095));
}

void MainWindow::on_txt_day_num_box_valueChanged(int days) //when day/s number_box change, it will sync change "back on" date time
{
    QDateTime leaveDateTime = ui->txt_leave_datetime_box->dateTime();
    ui->txt_return_back_date_time->setDateTime(leaveDateTime.addDays(days));
}

void MainWindow::on_rb_days_clicked() //when day/s radio box clicked
{
    ui->txt_day_num_box->setEnabled(true);
    ui->txt_return_back_date_time->setEnabled(false);
}

void MainWindow::on_rb_backon_clicked() //when "back on" radio box clicked
{
    ui->txt_return_back_date_time->setEnabled(true);
    ui->txt_day_num_box->setEnabled(false);
}

void MainWindow::on_txt_return_back_date_time_dateChanged(const QDate &backOnDateTime)//when "back on" date & time on changed
{
    QDate getDateOnLeave = ui->txt_leave_datetime_box->date(), getBackOn = backOnDateTime;
    ui->txt_day_num_box->setValue(static_cast<int>(getDateOnLeave.daysTo(getBackOn)));
        //because the datetime.daysTo(<64bit int>) data type is <long long>(64bit int), spin box only <int>(32bit int).
        //So I convert it and set limit for datetime max days is 1095 days (3 years).
}

