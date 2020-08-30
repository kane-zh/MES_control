#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QtDebug>
#include "pluginManage/pluginmanage.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    //将matlab窗口设置为随着窗口变化而变化
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void showPluginForm();        //显示插件的窗体
    void addPluginActions();      //添加插件action
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
