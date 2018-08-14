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
void CreateAll();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool isDataFolderExists = QDir("Data").exists();
    if (!isDataFolderExists)
    {
        QDir().mkdir("Data");
        CreateAll();
    }
    else
    {
        if(QDir("Data").isEmpty()) CreateAll();
        if(QFileInfo("Data/user.txt").exists())
        {
            if(QFileInfo("Data/user.txt").isDir())
            {
                QDir("Data/user.txt").removeRecursively();
                CreateUser();
            }
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
    QFile createFile("Data/user.txt");
    createFile.open(QIODevice::WriteOnly|QIODevice::Text);
    createFile.close();
}
void CreateRecord()
{
    QFile createFile("Data/record.txt");
    createFile.open(QIODevice::WriteOnly|QIODevice::Text);
    createFile.close();
}
void CreateAll()
{
    CreateUser();
    CreateRecord();
}
