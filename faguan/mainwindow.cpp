#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1200,780);//固定窗口大小

    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);//菜单栏退出按钮实现

    connect(ui->StartButton,&QToolButton::pressed,ui->StartButton,[=](){
        ui->StartButton->setStyleSheet("border-image: url(:/image/StartButton2.jpg);");
    });//开始按钮点击变化实现

    connect(ui->StartButton,&QToolButton::released,[=](){
        ui->StartButton->setStyleSheet("border-image: url(:/image/StartButton.jpg);");
        StartGame();
    });//点击按钮开始游戏

    connect(&Timer1,&QTimer::timeout,this,&MainWindow::Go);//判断游戏开始

    ui->titlelabel->setStyleSheet("color:red");//设置标题颜色

    ui->picturelabel->setVisible(false);//隐藏控件

    ui->tiptable->setVisible(false);
    ui->tiptable->setColumnCount(6);
    ui->tiptable->setHorizontalHeaderLabels(QStringList()<<"玩家名"<<"存活情况"<<"身份"<<"是否为村长"<<"所得票数"<<"本轮发言内容");
    ui->daylabel1->setVisible(false);
    ui->daylabel2->setVisible(false);
    ui->statuslabel1->setVisible(false);
    ui->statuslabel2->setVisible(false);
    ui->statuslabel3->setVisible(false);
    ui->statuslabel3->setAlignment(Qt::AlignHCenter);
    ui->playerlabel1->setVisible(false);
    ui->playerlabel2->setVisible(false);
    ui->textEdit->setVisible(false);
    ui->speechbtn->setVisible(false);







}

void MainWindow::paintEvent(QPaintEvent *event)//绘制背景图
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mainbackground.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

void MainWindow::StartGame()//游戏开始初始化
{
    QWidget *Choose_num = new QWidget;//创建选择人数窗口
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
        form.init(playernum);
        Choose_num->close();
        Timer1.setInterval(10);//发出游戏开始信号
        Timer1.start(1);
        //qDebug() << playernum;
    });
    connect(cancel,&QPushButton::clicked,Choose_num,[=](){
        Choose_num->close();
    });
    Choose_num->show();
}

void MainWindow:: Go()
{
    Timer1.stop();//停止触发器
    ui->StartButton->setVisible(false);//显示控件
    ui->titlelabel->setVisible(false);
    ui->picturelabel->setVisible(true);
    ui->tiptable->setVisible(true);

    ui->tiptable->setRowCount(form.Get_PlayerNum());
    for(int i=0;i<form.Get_PlayerNum();i++)//初始化表格
    {
        int col = 0;
        ui->tiptable->setItem(i,col++,new QTableWidgetItem("号玩家"));
        ui->tiptable->setItem(i,col++,new QTableWidgetItem("存活"));
        ui->tiptable->setItem(i,col++,new QTableWidgetItem("未知"));
        ui->tiptable->setItem(i,col++,new QTableWidgetItem("否"));
        ui->tiptable->setItem(i,col++,new QTableWidgetItem(QString::number(0)));
    }

    ui->daylabel1->setVisible(true);
    ui->daylabel2->setVisible(true);
    ui->statuslabel1->setVisible(true);
    ui->statuslabel2->setVisible(true);
    ui->statuslabel3->setVisible(true);
    ui->playerlabel1->setVisible(true);
    ui->playerlabel2->setVisible(true);
    ui->textEdit->setVisible(true);
    ui->speechbtn->setVisible(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}




