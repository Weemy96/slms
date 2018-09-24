#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class edit;
}

class edit : public QDialog
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = nullptr);
    ~edit();
    QStringList get_record_value;

private slots:
    void on_btn_cancel_edit_clicked();

    void on_btn_save_edit_clicked();

private:
    Ui::edit *ui;
};

#endif // EDIT_H
