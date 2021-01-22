#include <QApplication>
#include <QFile>
#include <QIODevice>
#include <QTime>
#include "qdebug.h"
#include <QStringList>

#include "udpclient.h"
#include "backupmodule.h"

#define PREWDNUM 1000
#define LINEWDLEN 500

int dursec = 0;


QString getDuration(QTime startTime, QTime endTime);
void storeTimeInfo(QString timeInfo);
void monitorBootupLog();



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monitorBootupLog();
    return a.exec();
}


void monitorBootupLog()
{
    QFile file("/log/messages");


    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"open log file failed.";
    }else{
        qDebug()<<"open log file succeeded.";
    }

    float ext_time = 0.0;
    //move to the end of the file
    quint64 startingPos;
    startingPos = file.size();
    file.seek(startingPos-PREWDNUM);
    QString timeInfo;
    bool isrestart = false;

    udpClient* client = udpClient::getSingleton();
    QTime startTime = QTime::currentTime();
    qDebug()<<"start monitoring the target app("<<startTime.toString()<<").."<<endl;
    while(true)
    {
        QByteArray lineRead = file.readLine(LINEWDLEN);

        if(lineRead.contains("EXT4-fs (sda4)"))
        {
            QString strLine = QString(lineRead);
            QStringList strlst1 = strLine.split('[');
            if(strlst1.size() > 1)
            {
                QStringList strlst2 = (strlst1[1]).split(']');
                if(strlst2.size()>1)
                {
                    QString extstr = strlst2[0];
                    ext_time = extstr.toFloat();
                }
            }
        }
        if(lineRead.contains("NetX is restart"))
        {
            isrestart = true;
        }

        if(lineRead.contains("netx_version"))
        {
            QTime endTime = QTime::currentTime();
            qDebug()<<"the monitored app is now up and running("<<endTime.toString()<<").."<<endl;
            qDebug()<<"duration:"<<getDuration(startTime, endTime)<<endl;

            //store bootup time info
            if(ext_time > 0)
            {
                timeInfo.append(QString("ext_time %1\n").arg(ext_time));
            }
            QString netxtimestr = "start time "+startTime.toString()+
                    "\nend time   "+endTime.toString()+
                    "\nduration   "+getDuration(startTime, endTime)+"\n";
            timeInfo.append(netxtimestr);
            if(isrestart)
            {
                timeInfo.append("NetX is restart\n\n");
            }

            QByteArray msg = "Net is started";
            msg.append(QString(" ext_time: %1;").arg(ext_time));
            msg.append(QString(" dur_time:%1").arg(dursec));

            if(ext_time+dursec > 21)
            {
                backupModule::bakFile();
            }

            client->setMessage(msg);
            storeTimeInfo(timeInfo);
            break;
		//system("reboot");
        }

        //keep the reading buffer size reasonable
//        quint64 currentSize = file.size();
//        if(currentSize - startingPos > 1024 * 64)
//        {
//            //move to the next reading position
//            startingPos = currentSize;
//            file.seek(startingPos);
//        }

    }
}

QString getDuration(QTime startTime, QTime endTime)
{
    int hourMargin = endTime.hour() - startTime.hour();
    int minuteMargin = endTime.minute() - startTime.minute();
    int secondMargin = endTime.second() - startTime.second();

    if(hourMargin < 0)
    {
        return "different days, checkout the start time and end time";
    }

    if(minuteMargin < 0)
    {
        minuteMargin = 60 - startTime.minute() + endTime.minute();
        hourMargin -= 1;
    }

    if(secondMargin < 0)
    {
        secondMargin = 60 - startTime.second() + endTime.second();
        minuteMargin -= 1;
    }

    QString duration;
    duration.sprintf("%02d:%02d:%02d", hourMargin, minuteMargin, secondMargin);
    dursec = hourMargin*3600 + minuteMargin*60 + secondMargin;
    return duration;
}

void storeTimeInfo(QString timeInfo)
{
    QFile file("/home/mingxing/timespent_tobootup.log");

    //open file
    if(!file.open(QIODevice::Append)){
        qDebug()<<"open file failed.";
    }else{
        //qDebug()<<"open file succeeded.";
    }
    int writtenLength = file.write(timeInfo.toLatin1(),timeInfo.length());
    if(writtenLength == -1)
    {
        qDebug()<<"written time info failed"<<endl;
    }
    file.close();
}
