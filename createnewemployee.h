#ifndef CREATENEWEMPLOYEE_H
#define CREATENEWEMPLOYEE_H

#include <QDialog>

namespace Ui {
class CreateNewEmployee;
}

class CreateNewEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewEmployee(QWidget *parent = nullptr);
    ~CreateNewEmployee();

private slots:
    void on_btn_cancel_clicked();

    void on_btn_Create_clicked();
    bool checkInput();

private:
    Ui::CreateNewEmployee *ui;
};

#endif // CREATENEWEMPLOYEE_H
