#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <QObject>

class backupModule : public QObject
{
    Q_OBJECT
public:
   backupModule();

signals:

public :
   static void bakFile();

};

#endif // BACKUPMODULE_H
