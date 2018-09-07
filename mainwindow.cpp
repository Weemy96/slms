#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtDebug> //debug lib
#include "createnewemployee.h"
#include "savetofile.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <exception>

void setMinTimeForLeaveBox();
void setEnableFunc(bool isEnable);
QString countDateTime(QDateTime leave, QDateTime back);
QString secondToDHMString(int64_t get_second);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinTimeForLeaveBox();
    ui->txt_id->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]{0,10}")));
    savetofile *save_log = new savetofile();
    bool isSaved = save_log->saveTxtToFile("Data/log.txt","testing! "+QDateTime::currentDateTime().toString());
    delete  save_log;
    qDebug()<<"Log Issaved: " << isSaved; //Debug: get Infomation


    //User friendly setting
    ui->txt_id->setFocusPolicy(Qt::StrongFocus);
    ui->txt_id->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEnableFunc(bool isEnable)
{
    ui->txt_remark->setEnabled(isEnable);
    ui->lbl_remark->setEnabled(isEnable);
    ui->lbl_leave_on->setEnabled(isEnable);
    ui->lbl_total->setEnabled(isEnable);
    ui->txt_day_num_box->setEnabled(isEnable);
    ui->rb_days->setEnabled(isEnable);
    ui->rb_backon->setEnabled(isEnable);
    ui->txt_leave_datetime_box->setEnabled(isEnable);
    ui->txt_id->setReadOnly(false);
    if(!isEnable) ui->rb_days->setChecked(1);
    ui->btn_addrecord->setEnabled(isEnable);
    ui->lbl_total_count->setEnabled(isEnable);
    ui->txt_return_back_date_time->setEnabled(false);
    //User friendly setting
    ui->txt_id->setFocusPolicy(Qt::StrongFocus);
    ui->txt_id->setFocus();

}

void MainWindow::on_btn_clear_clicked() //clear and reset input
{
    ui->txt_id->clear();
    ui->txt_name->clear();
    ui->txt_remark->clear();
    ui->txt_day_num_box->setValue(0);
    setMinTimeForLeaveBox();
    setEnableFunc(false);
}

void MainWindow::setMinTimeForLeaveBox()
{
    QDateTime CurrentDateTime = QDateTime::currentDateTime(); //get current DateTime when run programm.
    ui->txt_leave_datetime_box->setMinimumDateTime(CurrentDateTime); //set time to minimum Date and Time.
    ui->txt_leave_datetime_box->setDateTime(CurrentDateTime); //display date time for current
    ui->txt_return_back_date_time->setMinimumDateTime(CurrentDateTime.addSecs(3600)); //1 hour min leave time
    ui->txt_return_back_date_time->setMaximumDateTime(CurrentDateTime.addDays(1095)); //limit 3y
    ui->txt_return_back_date_time->setDateTime(CurrentDateTime.addSecs(3600)); //add 1 hour min leave time
}

void MainWindow::on_txt_leave_datetime_box_dateTimeChanged(const QDateTime &getLeaveOnDateTime)
{
    ui->txt_return_back_date_time->setDateTime(getLeaveOnDateTime.addSecs(3600));
    ui->txt_return_back_date_time->setMinimumDateTime(getLeaveOnDateTime.addSecs(3600));
    ui->txt_return_back_date_time->setMaximumDateTime(getLeaveOnDateTime.addDays(1095));
}

void MainWindow::on_txt_day_num_box_valueChanged(int days) //when day/s number_box change, it will sync change "back on" date time
{
    QDateTime leaveDateTime = ui->txt_leave_datetime_box->dateTime();
    int seconds=days*86400;
    ui->txt_return_back_date_time->setDateTime(leaveDateTime.addSecs(seconds));
    QDateTime backDateTime = ui->txt_return_back_date_time->dateTime();
    ui->lbl_total_count->setText(countDateTime(leaveDateTime,backDateTime));
}

void MainWindow::on_txt_return_back_date_time_dateTimeChanged(const QDateTime &backOnDateTime)
{
    QDateTime getDateOnLeave = ui->txt_leave_datetime_box->dateTime(), getBackOn = backOnDateTime;
    ui->txt_day_num_box->setValue(static_cast<int>(getDateOnLeave.daysTo(getBackOn)));
        //because the datetime.daysTo(<64bit int>) data type is <long long>(64bit int), spin box only <int>(32bit int).
        //So I convert it and set limit for datetime max days is 1095 days (3 years).
    QDateTime backDateTime = ui->txt_return_back_date_time->dateTime();
    ui->lbl_total_count->setText(countDateTime(getDateOnLeave,backDateTime));
}

void MainWindow::on_rb_days_clicked() //when day/s radio box clicked
{
    ui->txt_day_num_box->setEnabled(true);
    ui->txt_return_back_date_time->setEnabled(false);
}

void MainWindow::on_rb_backon_clicked() //when "back on" radio box clicked
{
    ui->txt_return_back_date_time->setEnabled(true);
    ui->txt_day_num_box->setEnabled(false);
}

QString countDateTime(QDateTime leave, QDateTime back)
{
    return secondToDHMString(leave.secsTo(back));
}

QString secondToDHMString(int64_t get_second) //calulate x Day x Hours x Minutes
{
    QString return_str="";
    int const s_hours=24, s_minutes=60, s_socond=60;
    get_second/=s_socond;
    int minutes= static_cast<int>(get_second) % s_minutes;
    get_second/=s_minutes;
    int hours= static_cast<int>(get_second) % s_hours;
    get_second/=s_hours;
    int days= static_cast<int>(get_second);
    return_str+=(days>=0&&days<=1)?QString::number(days)+" Day ":QString::number(days)+" Days ";
    return_str+=(hours>=0 && hours<=1)? QString::number(hours)+" Hour ":QString::number(hours)+" Hours ";
    return_str+=(minutes>=0 && minutes<=1)? QString::number(minutes)+" Minute ":QString::number(minutes)+" Minutes ";
    return return_str;
}

void MainWindow::on_menu_CreateNewEmployee_triggered()
{
    CreateNewEmployee createNewEmployeeProfile;
    createNewEmployeeProfile.setModal(true);
    createNewEmployeeProfile.exec();
}

void MainWindow::on_btn_check_clicked()
{
    bool isFound=false;
    QMessageBox msgBox;
    if(ui->txt_id->text()=="")
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Can't be empty.");
        msgBox.exec();
        on_btn_clear_clicked();

        //User friendly setting
        ui->txt_id->setFocusPolicy(Qt::StrongFocus);
        ui->txt_id->setFocus();
    }
    else
    {
        QFile file("Data/employee.txt");
        QFileInfo info(file);
        int row =0;

        try
        {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                while (!in.atEnd()) {
                    QString get_text = in.readLine();
                    row++;
                }
                file.close();
            }
            else
            {
                throw QString("Fail to Read");
            }

            qDebug()<<row;

            //create new 2d array
            QString **employee_detail=new QString*[row];
            for(int i=0; i<row;i++) employee_detail[i] = new QString[2];

            //store data into array
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                if(row>0)
                {
                    int count=0;
                    QTextStream in(&file);
                    while (!in.atEnd())
                    {
                        QString get_text = in.readLine();
                        QStringList getDetail=get_text.split("|");
                        employee_detail[count][0]= getDetail[0];
                        employee_detail[count][1]= getDetail[1];
                        qDebug()<<employee_detail[count][0]<<"\t"<<employee_detail[count][1];
                        count++;
                    }
                }
                file.close();
            }

            //find
            for(int count=0;count<row;count++)
            {
                if(employee_detail[count][0]==ui->txt_id->text())
                {
                    isFound=true;
                    ui->txt_id->setText(employee_detail[count][0]);
                    ui->txt_name->setText(employee_detail[count][1]);
                }
            }
            //free up memory
            delete [] employee_detail;

            if(!isFound)
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("ID not found");
                msgBox.setText("Sorry, ID: "+ui->txt_id->text()+" not in record.\nPlease create new ID.");
                msgBox.exec();
                on_btn_clear_clicked();

                //User friendly setting
                ui->txt_id->setFocusPolicy(Qt::StrongFocus);
                ui->txt_id->setFocus();
            }
            else
            {
                //User friendly setting
                setEnableFunc(true);
                ui->txt_id->setReadOnly(true);
                ui->txt_remark->setFocusPolicy(Qt::StrongFocus);
                ui->txt_remark->setFocus();
            }
        }
        catch (QString ex)
        {
            savetofile *error_log = new savetofile();
            error_log->saveTxtToFile("Data/log.txt","Error @ "+QDateTime::currentDateTime().toString()+" >> "+ex);
            qDebug()<<"Error: "; //Debug: get error infomation

            delete  error_log;
            msgBox.setWindowTitle("Error! Fail to read!");
            msgBox.setText("Sorry, fail to read.\nMaybe the file acess Premision error.\nPlease Check the file Properties at\n\""+info.absoluteFilePath()+"\"\nMake sure set the acess to Write and Read with normal user.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }
}

void MainWindow::on_txt_id_returnPressed()
{
    on_btn_check_clicked();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    QMessageBox msgBox;
    switch(index)
    {
        case 0:
            //User friendly setting
            ui->txt_id->setFocusPolicy(Qt::StrongFocus);
            ui->txt_id->setFocus();
            break;
        case 1:

            break;
        default:
            msgBox.setWindowTitle("Error!");
            msgBox.setText("Someting Wrong.\nError: tabBarClicked Error");
            msgBox.exec();
            break;
    }
}
