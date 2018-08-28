#ifndef SAVETOFILE_H
#define SAVETOFILE_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>

class savetofile
{
public:
    static bool saveTxtToFile(QString FileLoc, QString text);
//    {
//            QFile file(FileLoc);
//            if(file.open(QIODevice::Text|QIODevice::Append))
//            {
//                QTextStream stream(&file);
//                stream << text <<endl;
//                file.close();
//                return true;
//            }
//            else
//            {
//                return false;
//            }
//    }
};

#endif // SAVETOFILE_H
