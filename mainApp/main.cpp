#include "mainForm/mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QStringList>
#include <QString>
#include <QFile>
QString configDir;
QString  g_OutputDebug;
int debugCount=0;
QDateTime createTime;
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
    QByteArray localMsg = msg.toLocal8Bit();
    QString text;
    switch(type) {
    case QtDebugMsg:
        text = QString("Debug: ");
        break;
    case QtWarningMsg:
        text = QString("Warning: ");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal: ");
        break;
    default:
        text = QString("Debug: ");
    }

    // 设置输出信息格式
    QString context_info = QString("F2:(%1) L:(%2)").arg(QString(context.file)).arg(context.line); // F文件L行数
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("%1 %2 \t%3 \t%4").arg(text).arg(context_info).arg(strDateTime).arg(msg);
    g_OutputDebug.append(strMessage).append("\r\n");
    debugCount++;
    if(debugCount>=256){
        QDateTime currentTime=QDateTime::currentDateTime();
        QString fileDir=configDir+"/"+QString(createTime.toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt"));
        /*不是一天需新建文件*/
        if(currentTime.date().day()!=createTime.date().day()){
           createTime=QDateTime::currentDateTime();
           fileDir=configDir+"/"+QString(createTime.toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt"));
        }
        /*一天中超过12小时,需新建文件*/
        else if((currentTime.time().hour()-createTime.time().hour())>=12){
            createTime=QDateTime::currentDateTime();
            fileDir=configDir+"/"+QString(createTime.toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt"));
        }
        QFile file(fileDir);
        file.open(QIODevice::ReadWrite | QIODevice::Append);
        QTextStream stream(&file);
        stream << g_OutputDebug;
        g_OutputDebug="";
        debugCount=0;
        file.flush();
        file.close();
    }
    // 解锁
    mutex.unlock();
}
int main(int argc, char *argv[])
{
    QDir path = QDir(qApp->applicationDirPath());
    configDir=path.path()+("/logs");
    if (!path.exists(configDir))
    {
     path.mkdir(configDir);
    }
    //注册MessageHandler
    qInstallMessageHandler(outputMessage);

    qRegisterMetaType<RequestMetaData>("RequestMetaData");
    qRegisterMetaType<ResponseMetaData>("ResponseMetaData");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.connect( &a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    return a.exec();
}

