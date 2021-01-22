#include "backupmodule.h"
#include <QFile>
#include <QString>
#include <QDateTime>

#define SOURFILE1 "/log/messages"
#define SOURFILE2 "/log/Xorg.0.log"

backupModule::backupModule()
{

}

void backupModule::bakFile()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    int timeT = currentTime.toTime_t();
    QString strMsg = QString("copeMsg%1").arg(timeT);
    QString strXor = QString("copeXor%1").arg(timeT);
    QFile fileSourMsg(SOURFILE1);
    QFile fileSourXor(SOURFILE2);

    fileSourMsg.rename(strMsg);
    fileSourXor.rename(strXor);
}
