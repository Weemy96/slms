#include "edit.h"
#include "ui_edit.h"
#include "mainwindow.h"
#include <QtDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <savetofile.h>

QString countDateTime(QDateTime leave, QDateTime back);
QString secondToDHMString(int64_t get_second);
bool isValidDateTimeInBetween(QString id, QString leave);
QString notChangeLeaveDate,notChangeReturnDate;

edit::edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
    int row_number=MainWindow::get_record_data_passing.value(7).toInt();
    this->setWindowTitle("View - Row No. "+QString::number(row_number+1));
    qDebug()<<"form stat edit ui:"<< MainWindow::get_record_data_passing;
    ui->txt_id_edit->setText(MainWindow::get_record_data_passing.value(0));
    ui->txt_name_edit->setText(MainWindow::get_record_data_passing.value(1));
    ui->date_time_leave_on_edit->setDateTime(QDateTime::fromString(MainWindow::get_record_data_passing.value(2),"dd/MM/yyyy h:m AP"));
    ui->date_time_back_edit->setDateTime(QDateTime::fromString(MainWindow::get_record_data_passing.value(3),"dd/MM/yyyy h:m AP"));
    ui->lbl_total_count_edit->setText(MainWindow::get_record_data_passing.value(5));
    ui->txt_remark_edit->setPlainText(MainWindow::get_record_data_passing.value(6));

    if(MainWindow::isedit_req)
    {
        on_btn_save_edit_clicked();
    }
}

edit::~edit()
{
    delete ui;
}

void edit::on_btn_cancel_edit_clicked()
{
    this->close();
}

void edit::on_btn_save_edit_clicked()
{
    QMessageBox msgbox;
    if(ui->btn_save_edit->text()=="Edit")
    {
        int row_number=MainWindow::get_record_data_passing.value(7).toInt();
        this->setWindowTitle("Edit - Row No."+QString::number(row_number+1));
        ui->btn_save_edit->setText("Save");

        ui->txt_remark_edit->setReadOnly(false);
        ui->date_time_leave_on_edit->setReadOnly(false);
        ui->date_time_back_edit->setReadOnly(false);
        notChangeLeaveDate=ui->date_time_leave_on_edit->text();
        notChangeReturnDate=ui->date_time_back_edit->text();
    }
    else if (ui->btn_save_edit->text()=="Save")
    {
        if(notChangeLeaveDate==ui->date_time_leave_on_edit->text() && notChangeReturnDate==ui->date_time_back_edit->text())
        {
            int row_number=MainWindow::get_record_data_passing.value(7).toInt();
            QString read_all_data;
            QStringList data_list;
            QFile file("Data/record.txt");
            QTextStream stream(&file);
            if(file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QTextStream stream(&file);
                read_all_data=stream.readAll();
            }
            file.close();
            data_list=read_all_data.split("\n");
            savetofile *clean_space_and_enter = new savetofile;
            data_list[row_number]=ui->txt_id_edit->text()+"|"+ui->txt_name_edit->text()+"|"+ui->date_time_leave_on_edit->text()+"|"+ui->date_time_back_edit->text()+"|"+QString::number(ui->date_time_leave_on_edit->dateTime().daysTo(ui->date_time_back_edit->dateTime()))+"|"
                    +ui->lbl_total_count_edit->text()+"|"+ clean_space_and_enter->clean_space_front_n_back(ui->txt_remark_edit->toPlainText().simplified());
            delete clean_space_and_enter;
            data_list.removeLast();

            if(file.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                qDebug()<<data_list[row_number];
                for(int line=0;line<data_list.count();line++)
                {
                    if(line==read_all_data.count()-1)
                    {
                        stream<<data_list.at(line);
                    }
                    else
                    {
                        stream<<data_list.at(line)<<"\n";
                    }
                }
            }
            file.close();

            savetofile *savelog = new savetofile;
            savelog->saveTxtToFile("Data/log.txt","Record Changed: ["+ui->txt_id_edit->text()+"] on "+QDateTime::currentDateTime().toString());
            delete savelog;

            MainWindow::isedit_saved=true;
            msgbox.setWindowTitle("Edit");
            msgbox.setText("Edit saved.");
            msgbox.setIcon(QMessageBox::Information);
            msgbox.exec();
            on_btn_cancel_edit_clicked();

        }
        else
        {
            if(!isValidDateTimeInBetween(ui->txt_id_edit->text(),ui->date_time_leave_on_edit->text()))
            {
                if(!isValidDateTimeInBetween(ui->txt_id_edit->text(),ui->date_time_back_edit->text()))
                {
                    int row_number=MainWindow::get_record_data_passing.value(7).toInt();
                    QString read_all_data;
                    QStringList data_list;
                    QFile file("Data/record.txt");
                    QTextStream stream(&file);
                    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
                    {
                        QTextStream stream(&file);
                        read_all_data=stream.readAll();
                    }
                    file.close();
                    data_list=read_all_data.split("\n");
                    savetofile *clean_space_and_enter = new savetofile;
                    data_list[row_number]=ui->txt_id_edit->text()+"|"+ui->txt_name_edit->text()+"|"+ui->date_time_leave_on_edit->text()+"|"+ui->date_time_back_edit->text()+"|"+QString::number(ui->date_time_leave_on_edit->dateTime().daysTo(ui->date_time_back_edit->dateTime()))+"|"
                            +ui->lbl_total_count_edit->text()+"|"+ clean_space_and_enter->clean_space_front_n_back(ui->txt_remark_edit->toPlainText().simplified());
                    delete clean_space_and_enter;
                    data_list.removeLast();

                    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
                    {
                        qDebug()<<data_list[row_number];
                        for(int line=0;line<data_list.count();line++)
                        {
                            if(line==read_all_data.count()-1)
                            {
                                stream<<data_list.at(line);
                            }
                            else
                            {
                                stream<<data_list.at(line)<<"\n";
                            }
                        }
                    }
                    file.close();

                    savetofile *savelog = new savetofile;
                    savelog->saveTxtToFile("Data/log.txt","Record Changed: ["+ui->txt_id_edit->text()+"] on "+QDateTime::currentDateTime().toString());
                    delete savelog;

                    MainWindow::isedit_saved=true;
                    msgbox.setWindowTitle("Edit");
                    msgbox.setText("Edit saved.");
                    msgbox.setIcon(QMessageBox::Information);
                    msgbox.exec();
                    on_btn_cancel_edit_clicked();
                }
                else
                {
                    msgbox.setWindowTitle("Warning");
                    msgbox.setText("Can't select return date for record, because crash with your saved.");
                    msgbox.setIcon(QMessageBox::Warning);
                    msgbox.exec();
                }
            }
            else
            {
                msgbox.setWindowTitle("Warning");
                msgbox.setText("Can't select leave date for record, because crash with your saved.");
                msgbox.setIcon(QMessageBox::Warning);
                msgbox.exec();
            }
        }
    }
    else
    {
        msgbox.setWindowTitle("Error");
        msgbox.setText("Error! Internal corrupt.");
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.exec();
    }
}

void edit::on_date_time_leave_on_edit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->date_time_back_edit->setDateTime(dateTime.addSecs(3600));
    ui->date_time_back_edit->setMinimumDateTime(dateTime.addSecs(3600));
    ui->date_time_back_edit->setMaximumDateTime(dateTime.addDays(1095));
}

void edit::on_date_time_back_edit_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime getDateOnLeave = ui->date_time_leave_on_edit->dateTime(), getBackOn = dateTime;
        //because the datetime.daysTo(<64bit int>) data type is <long long>(64bit int), spin box only <int>(32bit int).
        //So I convert it and set limit for datetime max days is 1095 days (3 years).
    QDateTime backDateTime = ui->date_time_back_edit->dateTime();
    ui->lbl_total_count_edit->setText(countDateTime(getDateOnLeave,backDateTime));
}

