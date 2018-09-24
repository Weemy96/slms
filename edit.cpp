#include "edit.h"
#include "ui_edit.h"
#include "mainwindow.h"
#include <QtDebug>
#include <QMessageBox>

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
    ui->date_time_leave_on_edit->setDateTime(QDateTime::fromString(MainWindow::get_record_data_passing.value(2),"dd/M/yy h:m AP"));
    ui->date_time_back_edit->setDateTime(QDateTime::fromString(MainWindow::get_record_data_passing.value(3),"dd/M/yy h:m AP"));
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
        MainWindow::isedit_saved=true;
        msgbox.setWindowTitle("Edit");
        msgbox.setText("Edit saved.");
        msgbox.setIcon(QMessageBox::Information);
        msgbox.exec();
        on_btn_cancel_edit_clicked();
    }
    else
    {

    }
}
