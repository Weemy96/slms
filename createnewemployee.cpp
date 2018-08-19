#include "createnewemployee.h"
#include "ui_createnewemployee.h"

CreateNewEmployee::CreateNewEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewEmployee)
{
    ui->setupUi(this);
}

CreateNewEmployee::~CreateNewEmployee()
{
    delete ui;
}

void CreateNewEmployee::on_btn_cancel_clicked()
{
    this->close();
}
