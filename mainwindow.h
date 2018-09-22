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

    void on_txt_leave_datetime_box_dateTimeChanged(const QDateTime &dateTime);

    void on_txt_return_back_date_time_dateTimeChanged(const QDateTime &dateTime);

    void on_menu_CreateNewEmployee_triggered();

    void on_btn_check_clicked();

    void setEnableFunc(bool isEnable);

    void on_txt_id_returnPressed();

    void on_tabWidget_tabBarClicked(int index);

    void on_btn_addrecord_clicked();

    void on_btn_refresh_clicked();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
