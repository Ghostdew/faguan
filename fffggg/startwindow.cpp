#include "startwindow.h"
#include "ui_startwindow.h"
#include <QTimer>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDialog>
#include <QDebug>


StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1200,780);//固定窗口大小



    QLabel *helplabel = new QLabel(this);
    helplabel->setFixedSize(1314,673);
    helplabel->setStyleSheet("border-image: url(:/image/help.png);");
    helplabel->hide();//帮助页面的设置

    ui->closeButton->setVisible(false);//隐藏关闭按钮
    ui->titlelabel->setStyleSheet("color:red");//设置标题颜色

    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);//菜单栏退出按钮实现

    connect(ui->StartButton,&QToolButton::pressed,ui->StartButton,[=](){
        ui->StartButton->setStyleSheet("border-image: url(:/image/StartButton2.jpg);");
    });//开始按钮点击变化实现



    connect(ui->StartButton,&QToolButton::released,[=](){
        ui->StartButton->setStyleSheet("border-image: url(:/image/StartButton.jpg);");
        StartGame();
    });//点击按钮开始游戏

    connect(ui->helpButton,&QToolButton::pressed,ui->helpButton,[=](){
        ui->helpButton->setStyleSheet("border-image: url(:/image/helpbutton2.png);");
    });//帮助按钮点击变化实现



    connect(ui->helpButton,&QToolButton::released,[=](){
        ui->helpButton->setStyleSheet("border-image: url(:/image/helpbutton.png);");
        QTimer::singleShot(100,this,[=](){
            ui->helpButton->setVisible(false);
            ui->closeButton->setVisible(true);
            helplabel->show();
        });
    });//点击按钮打开帮助界面

    connect(ui->closeButton,&QToolButton::pressed,ui->closeButton,[=](){
        ui->closeButton->setStyleSheet("border-image: url(:/image/closebutton2.png);");
    });//关闭按钮点击变化实现



    connect(ui->closeButton,&QToolButton::released,[=](){
        ui->closeButton->setStyleSheet("border-image: url(:/image/closebutton.png);");
        QTimer::singleShot(100,this,[=](){
            ui->helpButton->setVisible(true);
            ui->closeButton->setVisible(false);
            helplabel->hide();
        });
    });//点击按钮关闭帮助界面
}

void StartWindow::paintEvent(QPaintEvent *event)//绘制背景图
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mainbackground.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

void StartWindow::StartGame()//游戏开始初始化
{
    QDialog *Choose_num = new QDialog(this);//创建选择人数窗口
    Choose_num->setFixedSize(300,140);
    Choose_num->setWindowTitle("选择玩家人数");
    Choose_num->setWindowIcon(QIcon(":/image/titleicon.jpg"));
    QSpinBox *nbox = new QSpinBox(Choose_num);
    nbox->move(190,30);
    nbox->setMinimum(3);
    QLabel *nlabel = new QLabel("请选择玩家人数：",Choose_num);
    nlabel->move(50,35);
    QPushButton *ok =new QPushButton("确认",Choose_num);
    ok->move(140,100);
    QPushButton *cancel =new QPushButton("取消",Choose_num);
    cancel->move(220,100);
    connect(ok,&QPushButton::clicked,Choose_num,[=](){
        int playernum = nbox->value();
        scene = nullptr;
        scene = new MainWindow(this,playernum);
        qDebug() << "new mainwindow";
        connect(scene,&MainWindow::restart,this,&StartWindow::show);//监听返回
        Choose_num->close();
        this->close();
        scene->show();
    });
    connect(cancel,&QPushButton::clicked,Choose_num,[=](){
        Choose_num->close();
    });
    Choose_num->exec();
}

StartWindow::~StartWindow()
{
    delete ui;
}
