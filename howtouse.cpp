#include "howtouse.h"
#include "ui_howtouse.h"

howtouse::howtouse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::howtouse)
{
    ui->setupUi(this);
}

howtouse::~howtouse()
{
    delete ui;
}

void howtouse::on_btn_ok_clicked()
{
    this->close();
}
