#ifndef SAVETOTXT_H
#define SAVETOTXT_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>

namespace save
{
    class savetxt;
}

class savetxt{
public:
    static bool saveTxtToFile(QString FileLocation, QString text);
};

bool savetxt::saveTxtToFile(QString FileLoc, QString text)
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

#endif // SAVETOTXT_H
