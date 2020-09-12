#include "mainForm/mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <fstream>      // std::ofstream
std::ofstream g_OutputDebug;
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
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

    QString context_info = QString("F:(%1) L:(%2)").arg(QString(context.file)).arg(context.line); // F文件信息L行数
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    std::string message = qPrintable(QString("%1 %2 \t%3 \t%4").arg(text).arg(context_info).arg(current_date).arg(msg));
    g_OutputDebug << message << "\r\n"; // std::ofstream
    // 解锁
    mutex.unlock();
}
int main(int argc, char *argv[])
{

    //注册MessageHandler
//    qInstallMessageHandler(outputMessage); //注册MessageHandler
//    g_OutputDebug.open(qPrintable(QString(QString(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt")))), std::ios::out | std::ios::trunc);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

