#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
