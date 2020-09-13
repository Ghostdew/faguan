#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QPushButton>
#include <cmath>
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

    connect(&Timer_Gamestart,&QTimer::timeout,this,&MainWindow::Go);//判断游戏开始

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
        Timer_Gamestart.start(1);//发出游戏开始信号
        //qDebug() << playernum;
    });
    connect(cancel,&QPushButton::clicked,Choose_num,[=](){
        Choose_num->close();
    });
    Choose_num->show();
}

void MainWindow:: Go()
{
    Timer_Gamestart.stop();//停止触发器
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
    if(form.P[0].Get_Id()==0)//设置玩家头像
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/red.png)");
    }else
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/black.png)");
    }

    Speech();
}


void MainWindow::Speech()
{
    Timer_speech1.setInterval(1000);
    Timer_speech1.start();
    time_left = 10;
    Timer_speech2.setInterval(10000);
    Timer_speech2.start();

    connect(&Timer_speech1,&QTimer::timeout,ui->lcdNumber,[=](){
        ui->lcdNumber->display(time_left--);
    });
    static int count = 0;//计数器
    connect(&Timer_speech2,&QTimer::timeout,this,[=](){
        if(leaderpos==0)
        {
            Next_right();
            count++;
            if(count==form.Get_PlayerNum())
            {
                Vote();
            }
        }
    });

}

void MainWindow::Vote()
{
    ui->statuslabel3->setText("投票阶段");
    Timer_speech2.stop();
    Timer_vote.start(10000);
    static VoteDialog *V = new VoteDialog(this);
    V->show();
    connect(&Timer_vote,&QTimer::timeout,[=](){
        Timer_vote.start(10000);
        V->show();
        Next_right();
        time_left = 10;
    });
    connect(V,&VoteDialog::senddata,[=](int num){
        if(form.P[activeplayer-1].Get_Leader())
        {
            form.P[num-1].Set_Voted(1+leadvote+form.P[num-1].Get_voted());
        }else
        {
            form.P[num-1].Set_Voted(1+form.P[num-1].Get_voted());
        }
        ui->tiptable->setItem(num-1,4,new QTableWidgetItem(QString::number(form.P[num-1].Get_voted())));
        Timer_vote.start(1);
    });

    Night();
}

void MainWindow::Night()
{

}

void MainWindow::Next_left()
{
    if(activeplayer==1)
    {
        activeplayer = form.Get_PlayerNum();
    }else
    {
        activeplayer--;
    }

    if(form.P[activeplayer-1].Get_Id()==0)
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/red.png)");
    }else
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/black.png)");
    }
    ui->playerlabel2->setText(QString::number(form.P[activeplayer-1].Get_Position()));
    time_left = 10;
}

void MainWindow::Next_right()
{
    if(activeplayer==form.Get_PlayerNum())
    {
        activeplayer = 1;
    }else
    {
        activeplayer++;
    }

    if(form.P[activeplayer-1].Get_Id()==0)
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/red.png)");
    }else
    {
        ui->picturelabel->setStyleSheet("border-image: url(:/image/black.png)");
    }
    ui->playerlabel2->setText(QString::number(form.P[activeplayer-1].Get_Position()));
    time_left = 10;
}



MainWindow::~MainWindow()
{
    delete ui;
}




