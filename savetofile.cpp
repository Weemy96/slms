#include "savetofile.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>

bool savetofile::saveTxtToFile(QString FileLoc, QString text)
{
    QFile file(FileLoc);
    if(file.open(QIODevice::Text|QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << text <<endl;
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
