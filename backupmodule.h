#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <QObject>

class backupModule : public QObject
{
    Q_OBJECT
public:
    explicit backupModule(QObject *parent = 0);

signals:

public slots:
};

#endif // BACKUPMODULE_H