#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "platform.h"
#include "player.h"
#include "votedialog.h"




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



    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Platform form;//平台实例
    QTimer Timer_Gamestart;//计时器
    QTimer Timer_speech1,Timer_speech2;//发言计时器,***************发言计时器1最好改名为lcd计时器
    QTimer Timer_vote;//投票计时器
    int leaderpos =0;//当前村长位置，0为无村长
    int time_left;//剩余时间
    int direction = 0;//0为从村长右边玩家开始发言，1为从村长左边玩家开始发言
    int activeplayer = 1;//当前活跃玩家
    float leadvote = 0.5;//村长票数改变量



private slots:
    void Go();//游戏开始运行
    void Speech();//发言阶段
    void Vote();//投票阶段
    void Night();//夜晚阶段
    void Next_left();//向左变换当前玩家
    void Next_right();//向右变换当前玩家
   // void receivedata(int num);//接受vote对话框中的数据

};

#endif // MAINWINDOW_H
