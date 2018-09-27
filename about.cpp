#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
}

about::~about()
{
    delete ui;
}

void about::on_btn_go_github_clicked()
{
    QString link = "https://github.com/Weemy96";
    QDesktopServices::openUrl(QUrl(link));
}

void about::on_btn_ok_clicked()
{
    this->close();
}
