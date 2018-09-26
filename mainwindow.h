#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QStringList get_record_data_passing;
    static bool isedit_saved;
    static bool isedit_req;
    void del_Line_in_file(int line_number_to_delete, QString filename);

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

    void on_btn_view_clicked();

    void call_edit_n_view_subwindow(bool isedit_requ);

    void on_btn_edit_clicked();

    void on_btn_del_clicked();
    void del_one_line(int line_number, QString &strall);

    void on_actionAbout_F12_triggered();

    void on_actionHow_to_use_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
