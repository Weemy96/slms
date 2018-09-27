#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtDebug> //debug lib
#include "createnewemployee.h"
#include "savetofile.h"
#include "edit.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <exception>
#include <QKeyEvent>
#include <QModelIndexList>
#include <about.h>
#include <howtouse.h>

QString countDateTime(QDateTime leave, QDateTime back);
QString secondToDHMString(int64_t get_second);
bool isValidDateTimeInBetween(QString id, QString leave);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinTimeForLeaveBox();
    ui->txt_id->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]{0,10}")));
    savetofile *save_log = new savetofile();
    bool isSaved = save_log->saveTxtToFile("Data/log.txt","Software opened at "+QDateTime::currentDateTime().toString());
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
    ui->txt_leave_datetime_box->setMinimumDateTime(CurrentDateTime); //set time to minimum Date and Time for now.
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

void MainWindow::on_menu_CreateNewEmployee_triggered()//call Create Detail window
{
    CreateNewEmployee createNewEmployeeProfile;
    createNewEmployeeProfile.setModal(true);
    createNewEmployeeProfile.exec();
}

void MainWindow::on_btn_check_clicked() //checking id is exitst or not
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

void MainWindow::on_txt_id_returnPressed() //enter key in id call on_btn_check_clicked()
{
    on_btn_check_clicked();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)//tab bar is change command
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
            on_btn_refresh_clicked();
            break;
        default:
            msgBox.setWindowTitle("Error!");
            msgBox.setText("Someting Wrong.\nError: tabBarClicked Error");
            msgBox.exec();
            break;
    }
}

void MainWindow::on_btn_addrecord_clicked() //need add check record <date
{
    savetofile *save_leave_data_n_function = new savetofile();
    QMessageBox msg;
    if(save_leave_data_n_function->clean_space_front_n_back(ui->txt_remark->toPlainText().simplified()) != "")
    {
        QString id = ui->txt_id->text(),
                name = ui->txt_name->text(),
                leave_date = ui->txt_leave_datetime_box->text(),
                back_date = ui->txt_return_back_date_time->text(),
                days = ui->txt_day_num_box->text(),
                between_in = ui->lbl_total_count->text(),
                remark = save_leave_data_n_function->clean_space_front_n_back(ui->txt_remark->toPlainText());

        if(isValidDateTimeInBetween(id,ui->txt_leave_datetime_box->text()))
        {
            msg.setText("Can't select this leave date, because crash with your saved record.\nPlease select another date and try again.");
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("Warning!");
            msg.exec();
        }
        else
        {
            if(isValidDateTimeInBetween(id,ui->txt_return_back_date_time->text()))
            {
                msg.setText("Can't select this return date, because crash with your saved record.\nPlease select another date and try again.");
                msg.setIcon(QMessageBox::Warning);
                msg.setWindowTitle("Warning!");
                msg.exec();
            }
            else
            {
                //ID|Name|leave date|back date|days|beteen calculation (day/s hours minits)|remark
                save_leave_data_n_function->saveTxtToFile("./Data/record.txt",id+"|"+name+"|"+leave_date+"|"+back_date+"|"+days+"|"+between_in+"|"+remark);
                save_leave_data_n_function->saveTxtToFile("Data/log.txt","Record Added: ["+id+"] on "+QDateTime::currentDateTime().toString());
                msg.setText("Save Done!");
                msg.setIcon(QMessageBox::Information);
                msg.exec();
                on_btn_clear_clicked();
            }
        }

    }
    else
    {
        msg.setWindowTitle("Error!");
        msg.setText("Remark must filled.");
        msg.exec();
    }
}

void MainWindow::on_btn_refresh_clicked() //read record, same as refresh record
{
    //clean all table and refresh
    ui->table_data_view->disconnect();
    ui->table_data_view->clearContents();
    ui->table_data_view->setRowCount(0);


    ui->table_data_view->setColumnCount(7);
    QStringList column_name;
    column_name<<"ID"<<"Name"<<"Leave Date"<<"Back Date"<<"Days"<<"Between"<<"Remark";
    ui->table_data_view->setHorizontalHeaderLabels(column_name);

    QFile record_file("Data/record.txt");
    QFileInfo info(record_file);
    int row =0, insertrow=0;

    try
    {
        if (record_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&record_file);

            //counting line
            while (!in.atEnd()) {
                QString get_text = in.readLine();
                row++;
            }
            record_file.close();
        }
        else
        {
            throw QString("Fail to Read");
        }

        //qDebug()<<row;
        ui->lbl_total_record->setText("Total Record: "+QString::number(row));
        //ID|Name|leave date|back date|days|beteen calculation (day/s hours minits)|remark
        //[0][1]    [2]            [3]  [4]                 [5]                        [6]
        //create new 2d array
        QString **get_record_detail=new QString*[row];
        for(int i=0; i<row;i++) get_record_detail[i] = new QString[8]; //[7]is row

        //store data into array
        if (record_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if(row>0)
            {
                int count=0;
                QTextStream in(&record_file);
                while (!in.atEnd())
                {
                    QString get_text = in.readLine();
                    QStringList getDetail=get_text.split("|");
                    for(int i=0;i<8;i++)
                    {
                        if(i!=7)
                        {
                            get_record_detail[count][i]= getDetail[i];
                        }
                        else
                        {
                            get_record_detail[count][i]=QString::number(count);
                        }
                    }
                    ui->table_data_view->insertRow(ui->table_data_view->rowCount());
                    insertrow=ui->table_data_view->rowCount()-1;
//                    ui->table_data_view->setItem(insertrow,0,new QTableWidgetItem(count));
                    ui->table_data_view->setItem(insertrow,0,new QTableWidgetItem(get_record_detail[count][0]));
                    ui->table_data_view->setItem(insertrow,1,new QTableWidgetItem(get_record_detail[count][1]));
                    ui->table_data_view->setItem(insertrow,2,new QTableWidgetItem(get_record_detail[count][2]));
                    ui->table_data_view->setItem(insertrow,3,new QTableWidgetItem(get_record_detail[count][3]));
                    ui->table_data_view->setItem(insertrow,4,new QTableWidgetItem(get_record_detail[count][4]));
                    ui->table_data_view->setItem(insertrow,5,new QTableWidgetItem(get_record_detail[count][5]));
                    ui->table_data_view->setItem(insertrow,6,new QTableWidgetItem(get_record_detail[count][6]));

                    //qDebug()<<get_record_detail[count][0]<<"\t"<<get_record_detail[count][1]<<"\t"<<get_record_detail[count][2]<<"\t"<<get_record_detail[count][3]<<"\t"<<get_record_detail[count][4]<<"\t"<<get_record_detail[count][5]<<"\t"<<get_record_detail[count][6]<<"\t"<<get_record_detail[count][7];
                    ui->btn_view->setEnabled(true);
                    ui->btn_edit->setEnabled(true);
                    ui->btn_del->setEnabled(true);

                    count++;
                }
            }
            else
            {
                ui->btn_view->setEnabled(false);
                ui->btn_edit->setEnabled(false);
                ui->btn_del->setEnabled(false);
            }
            record_file.close();
        }

        //free up memory
        delete [] get_record_detail;
        ui->table_data_view->selectRow(0);

    }
    catch(QString ex)
    {

    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F5)
    {
        on_btn_refresh_clicked();
    }
    else if(ui->tabWidget->currentIndex()==0 && ui->txt_id->isReadOnly() && event->key() == Qt::Key_F3)
    {
        on_btn_addrecord_clicked();
    }
    else if(ui->tabWidget->currentIndex()==1 && ((event->key() == Qt::Key_F) && (event->modifiers()& Qt::ControlModifier)))//Crtl+F
    {
        ui->txt_find_id->setFocusPolicy(Qt::StrongFocus);
        ui->txt_find_id->setFocus();
    }
    else if(ui->tabWidget->currentIndex()==1 && event->key() == Qt::Key_Delete)
    {
        on_btn_del_clicked();
    }
}

QStringList MainWindow::get_record_data_passing;
bool MainWindow::isedit_saved = false;
bool MainWindow::isedit_req;

void MainWindow::on_btn_view_clicked()
{
    call_edit_n_view_subwindow(false);
}

void MainWindow::on_btn_edit_clicked()
{
    call_edit_n_view_subwindow(true);
}

void MainWindow::call_edit_n_view_subwindow(bool isedit_requ)
{
    QModelIndexList selected_row=ui->table_data_view->selectionModel()->selectedRows();
    QModelIndex index=selected_row.at(0);
    int row_selected_index = index.row();

    for(int i=0;i<ui->table_data_view->columnCount();i++)
    {
        get_record_data_passing<<ui->table_data_view->item(row_selected_index,i)->text();
    }
    get_record_data_passing<<QString::number(row_selected_index);
    isedit_req=isedit_requ;
    qDebug()<<"at mainmenu.cpp: "<<get_record_data_passing;
    edit *callview = new edit;
    callview->setModal(true);
    callview->exec();
    qDebug()<<"callview is closed";
    delete callview;
    get_record_data_passing.clear(); //reset string
    if(MainWindow::isedit_saved)
    {
        on_btn_refresh_clicked();
        MainWindow::isedit_saved=false;
    }
}

void MainWindow::on_btn_del_clicked()
{
    QModelIndexList selected_row=ui->table_data_view->selectionModel()->selectedRows();
    QModelIndex index=selected_row.at(0);
    int row_selected_index = index.row();
    del_Line_in_file(row_selected_index, QString("Data/record.txt"));
    on_btn_refresh_clicked();
}

void MainWindow::del_one_line(int line_number, QString &strall)
{
    int nLine=0;
    int Index=0;

    //calcu nLine number of line
    while(Index!=-1)
    {
        Index=strall.indexOf('\n',Index+1);
        nLine++;
    }

    //if index localtion starting with 0 [first char] find and delete \n
    if(line_number==0)
    {
        int nIndex=strall.indexOf('\n');
        strall.remove(0,nIndex+1);
    }
    else
    {
        int nTemp=line_number;
        int nIndex=0,nIndex2=0;
        while(nTemp--)
        {
            nIndex=strall.indexOf('\n',nIndex+1);//renew nIndex
            if(nIndex!=-1) //checking nIndex is valid or not
            {
                nIndex2=strall.indexOf('\n',nIndex+1);
            }
        }

        //if delete not is last line (form nIndex+1 start to nIndex2-nIndex char will remove)
        if(line_number<nLine-1)
        {
            strall.remove(nIndex+1, nIndex2-nIndex);
        }

        //delete last line (starting form nIndex, len-nIndex char by string will remove)
        else if(line_number==nLine-1)
        {
            int len=strall.length();
            strall.remove(nIndex,len-nIndex);
        }
    }
}

void MainWindow::del_Line_in_file(int line_number_to_delete, QString filename)
{
    QString strall;
    QFile readfile(filename);
    if(readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        strall=stream.readAll();
    }
    readfile.close();
    del_one_line(line_number_to_delete, strall);

    QFile writefile(filename);
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream<<strall;
    }
    writefile.close();


}

void MainWindow::on_actionAbout_F12_triggered()
{
    about about_windows;
    about_windows.setModal(true);
    about_windows.exec();
}

bool isValidDateTimeInBetween(QString id, QString leave)
{
    QFile record_file("Data/record.txt");
    QFileInfo info(record_file);
    int row =0;
    bool isInTheDateTime=false;
    uint time_leave_record,time_back_record,get_leave_time;
    QDateTime time_leave_from_record, time_back_leave_form_record, time_from_leave;

    if (record_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&record_file);
        //counting line
        while (!in.atEnd()) {
            QString get_text = in.readLine();
            row++;
        }
        record_file.close();
    }
    else
    {
        throw QString("Fail to Read");
    }

    //ID|Name|leave date|back date|days|beteen calculation (day/s hours minits)|remark
    //[0][1]    [2]            [3]  [4]                 [5]                        [6]
    //create new 2d array
    QString **get_record_detail=new QString*[row];
    for(int i=0; i<row;i++) get_record_detail[i] = new QString[2];

    //store data into array
    if (record_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(row>0)
        {
            int count=0;
            QTextStream in(&record_file);
            while (!in.atEnd())
            {
                QString get_text = in.readLine();
                QStringList getDetail=get_text.split("|");
                if(getDetail[0]==id)
                {
                    get_record_detail[count][0] = getDetail[2];
                    get_record_detail[count][1] = getDetail[3];
                    count++;
                }
            }

            for(int i=0; i<count;i++)
            {
                time_leave_from_record = QDateTime::fromString(get_record_detail[i][0],"dd/MM/yyyy h:m AP");
                time_back_leave_form_record= QDateTime::fromString(get_record_detail[i][1],"dd/MM/yyyy h:m AP");
                time_from_leave= QDateTime::fromString(leave,"dd/MM/yyyy h:m AP");

                time_leave_record= time_leave_from_record.toTime_t();
                time_back_record= time_back_leave_form_record.toTime_t();
                get_leave_time= time_from_leave.toTime_t();

                if(get_leave_time >= time_leave_record && get_leave_time <= time_back_record)
                {
                    isInTheDateTime=true;
                }
            }
        }
        record_file.close();
    }

    //free up memory
    delete [] get_record_detail;
    return  isInTheDateTime;
}

void MainWindow::on_actionHow_to_use_triggered()
{
    howtouse menual;
    menual.setModal(true);
    menual.exec();
}

void MainWindow::on_btn_find_clicked()
{
    if(!ui->txt_find_id->text().simplified().isEmpty())
    {
        QString filter = ui->txt_find_id->text();
        for( int i = 0; i < ui->table_data_view->rowCount(); ++i )
        {
            bool match = false;
            for( int j = 0; j < ui->table_data_view->columnCount(); ++j )
            {
                QTableWidgetItem *item = ui->table_data_view->item( i, j );
                if( item->text().contains(filter) )
                {
                    match = true;
                    break;
                }
            }
            ui->table_data_view->setRowHidden( i, !match );
        }
    }
}

void MainWindow::on_txt_find_id_returnPressed()
{
    on_btn_find_clicked();
}
