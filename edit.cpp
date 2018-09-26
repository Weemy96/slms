#include "edit.h"
#include "ui_edit.h"
#include "mainwindow.h"
#include <QtDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

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
    }
    else if (ui->btn_save_edit->text()=="Save")
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
        data_list[row_number]="1232130|lel|25/09/2018 1:53 PM|25/09/2018 2:53 PM|0|0 Day 1 Hour 0 Minute |iuq iqiwueiquw m quemqwueiqwu hihfi uh";
        data_list.removeLast();
        qDebug()<<data_list;

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

        MainWindow::isedit_saved=true;
        msgbox.setWindowTitle("Edit");
        msgbox.setText("Edit saved.");
        msgbox.setIcon(QMessageBox::Information);
        msgbox.exec();
        on_btn_cancel_edit_clicked();
    }
}
