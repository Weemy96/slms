#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_clear_clicked();
    void setMinTimeForLeaveBox();

    void on_txt_day_num_box_valueChanged(int days);

    void on_rb_days_clicked();

    void on_rb_backon_clicked();

    void on_txt_return_back_date_time_dateChanged(const QDate &date);

    void on_txt_leave_datetime_box_dateTimeChanged(const QDateTime &dateTime);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
