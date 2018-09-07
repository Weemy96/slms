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
    ui->txt_createNewID->setPlaceholderText("Only accept 10 character of number and alphabet");
    ui->txt_createNewID->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]{0,10}")));
    ui->txt_createNewID->setFocusPolicy(Qt::StrongFocus);
    ui->txt_createNewID->setFocus();
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
        bool isDone=false;
        savetofile *save_new_employee = new savetofile();
        isDone = save_new_employee->saveTxtToFile("Data/employee.txt",ui->txt_createNewID->text()+"|"+ui->txt_CreateNewName->text());
        if(isDone)
        {
            msgBox.setText("Create Done.");
            msgBox.exec();
            this->close();
        }
        delete save_new_employee;
    }
    else
    {
        msgBox.setText("Pls fill all text box.");
        msgBox.setWindowTitle("Warning");
        msgBox.setIcon(QMessageBox::Warning);
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
