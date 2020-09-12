#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include "platform.h"
#include "player.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);//绘图事件，绘制背景图片
    void StartGame();//开始游戏初始化
    void Speech();//发言函数
    void Vote();//投票函数


    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Platform form;
    QTimer Timer1;
    QThread T;

private slots:
    void Go();//游戏开始运行

};

#endif // MAINWINDOW_H
