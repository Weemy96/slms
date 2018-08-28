#include "createnewemployee.h"
#include "ui_createnewemployee.h"
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>
#include <QMessageBox>
#include "savetofile.h"

CreateNewEmployee::CreateNewEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewEmployee)
{
    ui->setupUi(this);
    ui->txt_createNewID->setPlaceholderText("10 char for id");
    ui->txt_createNewID->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]{0,10}")));
}

CreateNewEmployee::~CreateNewEmployee()
{
    delete ui;
}

void CreateNewEmployee::on_btn_cancel_clicked()
{
    this->close();
}

void CreateNewEmployee::on_btn_Create_clicked()
{
    QMessageBox msgBox;
    if(checkInput())
    {
//        QString savefile="Data/employee.txt";
//        QFile file(savefile);
//        if(file.open(QIODevice::Text|QIODevice::Append))
//        {
//            QTextStream stream(&file);
//            stream << ui->txt_createNewID->text()+"|"+ui->txt_CreateNewName->text() <<endl;
//            file.close();
//        }
        bool isDone=false;

        isDone = savetofile::saveTxtToFile("Data/employee.txt",ui->txt_createNewID->text()+"|"+ui->txt_CreateNewName->text());
        if(isDone)
        {
            msgBox.setText("Create Done.");
            msgBox.exec();
            this->close();
        }
        else
        {
            msgBox.setText("Sorry Fail");
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setText("Pls fill all text box.");
        msgBox.exec();
    }

}

bool CreateNewEmployee::checkInput()
{
    if(ui->txt_createNewID->text()==""||ui->txt_CreateNewName->text()=="")
    {
        return false;
    }
    else
    {
        return true;
    }
}
