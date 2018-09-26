#ifndef HOWTOUSE_H
#define HOWTOUSE_H

#include <QDialog>

namespace Ui {
class howtouse;
}

class howtouse : public QDialog
{
    Q_OBJECT

public:
    explicit howtouse(QWidget *parent = nullptr);
    ~howtouse();

private slots:
    void on_btn_ok_clicked();

private:
    Ui::howtouse *ui;
};

#endif // HOWTOUSE_H
