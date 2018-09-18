#ifndef SAVETOFILE_H
#define SAVETOFILE_H
#include <QString>

class savetofile
{
public:
    savetofile(){}
    bool saveTxtToFile(QString FileLoc, QString text);
    QString clean_space_front_n_back(QString str);
};

#endif // SAVETOFILE_H
