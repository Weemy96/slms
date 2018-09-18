#include "createnewemployee.h"
#include "ui_createnewemployee.h"
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include "savetofile.h"

CreateNewEmployee::CreateNewEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewEmployee)
{
    ui->setupUi(this);
    ui->txt_createNewID->setPlaceholderText("Only accept 10 character of number and alphabet");
    ui->txt_createNewID->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]{0,10}")));
    ui->txt_createNewID->setFocusPolicy(Qt::StrongFocus);
    ui->txt_createNewID->setFocus();
}

CreateNewEmployee::~CreateNewEmployee()
{
    delete ui;
}

void CreateNewEmployee::on_btn_cancel_clicked()
{
    this->close();
}

void CreateNewEmployee::on_btn_Create_clicked()
{
    QMessageBox msgBox;
    if(checkInput())
    {
        QFile file("Data/employee.txt");
        QFileInfo info(file);
        int row =0; //get row of data
        bool isExists=false;

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

            //create new 2d array
            QString **employee_detail=new QString*[row];
            for(int i=0; i<row;i++) employee_detail[i] = new QString[2];

            //store data into array
            //[0] = ID
            //[1] = Name
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
                        //qDebug()<<employee_detail[count][0]<<"\t"<<employee_detail[count][1];
                        count++;
                    }
                }
                file.close();
            }

            //find id
            for(int count=0;count<row;count++)
            {
                if(employee_detail[count][0]==ui->txt_createNewID->text())
                {
                    isExists=true;
                }
            }
            //free up memory
            delete [] employee_detail;

            //if ID is Exists popup error
            if(!isExists)
            {
                bool isDone=false;
                savetofile *save_new_employee = new savetofile();
                QString name_employee = save_new_employee->clean_space_front_n_back(ui->txt_CreateNewName->text()); //clean front and back text with space
                //qDebug()<<name_employee;
                isDone = save_new_employee->saveTxtToFile("Data/employee.txt",ui->txt_createNewID->text()+"|"+ name_employee);
                if(isDone)
                {
                    msgBox.setText("Create Done.");
                    msgBox.exec();
                    this->close();
                }
                delete save_new_employee;
            }
            else
            {
                msgBox.setWindowTitle("Error!");
                msgBox.setText("ID "+ ui->txt_createNewID->text() +" is exsites.\nPlease register by other ID.");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
        }
        catch(QString e)
        {
            savetofile *error_log = new savetofile();
            error_log->saveTxtToFile("Data/log.txt","Error @ "+QDateTime::currentDateTime().toString()+" >> "+e);
            qDebug()<<"Error: "; //Debug: get error infomation

            delete  error_log;
            msgBox.setWindowTitle("Error! Fail to read!");
            msgBox.setText("Sorry, fail to read.\nMaybe the file acess Premision error.\nPlease Check the file Properties at\n\""+info.absoluteFilePath()+"\"\nMake sure set the acess to Write and Read with normal user.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setText("Pls fill all text box.");
        msgBox.setWindowTitle("Warning");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

}

bool CreateNewEmployee::checkInput()
{
    if(ui->txt_createNewID->text()==""||ui->txt_CreateNewName->text()=="")
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CreateNewEmployee::on_txt_CreateNewName_textChanged(const QString &arg1)
{
    if(arg1==" ")
    {
        ui->txt_CreateNewName->backspace();
    }
}
