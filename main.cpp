#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>

void CreateUser();
void CreateRecord();
void CreateLog();
void CreateAll();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool isDataFolderExists = QDir("Data").exists();
    if (!isDataFolderExists) //check Data folder is exists. If NO, create it and create all file needed.
    {
        QDir().mkdir("Data");
        CreateAll();
    }
    else
    {
        if(QDir("Data").isEmpty()) CreateAll(); //check Data folder is empty. If TRUE, so Create All file need

//employee
        if(QFileInfo("Data/employee.txt").exists()) //check employee.txt is exists. If NO, create it.
        {
            if(QFileInfo("Data/employee.txt").isDir()) //check employee.txt is folder, If YES, remove it and create file.
            {
                QDir("Data/employee.txt").removeRecursively();
                CreateUser();
            }
        }
        else
        {
            CreateUser();
        }

//record
        if(QFileInfo("Data/record.txt").exists()) //check record.txt is exists. If NO, create it.
        {
            if(QFileInfo("Data/record.txt").isDir()) //check record.txt is folder. If YES, remove it and create file.
            {
                QDir("Data/record.txt").removeRecursively();
                CreateRecord();
            }
        }
        else
        {
            CreateRecord();
        }

//log
        if(QFileInfo("Data/log.txt").exists()) //check log.txt is exists. If NO create it.
        {
            if(QFileInfo("Data/log.txt").isDir()) //check log.txt is folder. If yes remove it and create file.
            {
                QDir("Data/log.txt").removeRecursively();
                CreateLog();
            }
        }
        else
        {
            CreateLog();
        }
    }

    MainWindow w;
    //set application run center screen
    QDesktopWidget *scrSize = QApplication::desktop();
    int W = scrSize->width()/2-w.width()/2;
    int H = scrSize->height()/2-w.height()/2;
    w.move(W,H);
    w.show();

    return a.exec();
}

void CreateUser()
{
    QFile createFile("Data/employee.txt");
    createFile.open(QIODevice::WriteOnly|QIODevice::Text);
    createFile.close();
}

void CreateRecord()
{
    QFile createFile("Data/record.txt");
    createFile.open(QIODevice::WriteOnly|QIODevice::Text);
    createFile.close();
}

void CreateLog()
{
    QFile createFile("Data/log.txt");
    createFile.open(QIODevice::WriteOnly|QIODevice::Text);
    createFile.close();
}

void CreateAll()
{
    CreateUser();
    CreateRecord();
    CreateLog();
}
