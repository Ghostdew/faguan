#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "mainwindow.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);//绘图事件，绘制背景图片
    ~StartWindow();

private:
    Ui::StartWindow *ui;
    MainWindow *scene = nullptr;

public slots:
    void StartGame();//游戏开始初始化
};



#endif // STARTWINDOW_H
