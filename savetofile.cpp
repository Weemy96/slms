#include "savetofile.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>

bool savetofile::saveTxtToFile(QString FileLoc, QString text)
{
    QFile file(FileLoc);
    QMessageBox msgBox;
    try
    {
        if(file.open(QIODevice::Text|QIODevice::Append))
        {
            QTextStream stream(&file);
            stream << text <<endl;
            file.close();
            return true;
        }
        else
        {
            QFileInfo info(FileLoc);
            msgBox.setWindowTitle("Error! Fail to save!");
            msgBox.setText("Sorry, fail to save.\nMaybe the file set as Read Only or need Permission to Write.\nPlease Check the file Properties at\n\""+info.absoluteFilePath()+"\"");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }
    }
    catch (std::exception & ex)
    {
        qDebug()<<"Error savetofile.cpp: "<<ex.what(); //Debug: get Infomation
        msgBox.setWindowTitle("Unknow Error!");
        msgBox.setIcon(QMessageBox::Critical);
        QString error="Error Code: savetofile_exception\n"+static_cast<QString>(ex.what());
        msgBox.setText(error);
        msgBox.exec();
        return false;
    }
}
