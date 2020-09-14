#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
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
        ui->tiptable->setItem(i,col++,new QTableWidgetItem(QString::number(0.0)));
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

    if(form.Get_PlayerNum()%2)
        leadvote = -0.5;
    else
        leadvote = 0.5;

    Speech();
}


void MainWindow::Speech()
{
    Timer_speech1.start(1000);
    time_left = 10;
    Timer_speech2.start(10000);

    static int count = 0;//计数器
    count = 0;

    if(leaderpos!=0)
    {
        ui->statuslabel3->setText("发言阶段");
        activeplayer = leaderpos;
        if(form.P[activeplayer-1].Get_Id()==0)
        {
            ui->picturelabel->setStyleSheet("border-image: url(:/image/red.png)");
        }else
        {
            ui->picturelabel->setStyleSheet("border-image: url(:/image/black.png)");
        }
        ui->playerlabel2->setText(QString::number(form.P[activeplayer-1].Get_Position()));
        QMessageBox::question(this,"选择发言方向","请选择发言方向");
    }else
    {
        connect(&Timer_speech1,&QTimer::timeout,ui->lcdNumber,[=](){
            qDebug() << QString::number(time_left);
            ui->lcdNumber->display(time_left--);
        });
        connect(&Timer_speech2,&QTimer::timeout,this,[=](){
            if(leaderpos==0)
            {
                Next_right();
                count++;
                if(count==form.Get_PlayerNum())
                {
                    Vote();
                }
            }else
            {
                //等会完善这个东西：第二轮及以后的发言阶段
                qDebug() << "success";
            }
        });
    }





}

void MainWindow::Vote()
{
    ui->statuslabel3->setText("投票阶段");
    Timer_speech2.stop();
    Timer_vote.start(10000);
    static VoteDialog *V = new VoteDialog(this);
    static int count2 = 0;
    count2 = 0;
    V->show();
    connect(&Timer_vote,&QTimer::timeout,[=](){
        Timer_vote.start(10000);
        count2++;
        V->show();
        Next_right();
        time_left = 10;
        if(count2==form.Get_IdNumber(0)+form.Get_IdNumber(1))//村长选举判断
        {
            V->close();
            int maximum = form.P[0].Get_voted();
            int maxpos = form.P[0].Get_Position();
            for(int i=1;i<form.Get_PlayerNum();i++)
            {
                if(form.P[i].Get_Death()==false&&form.P[i].Get_voted()>maximum)
                {
                    maximum = form.P[i].Get_voted();
                    maxpos = form.P[i].Get_Position();
                }
            }
            if(leaderpos==0)//村长当选路线
            {
                Timer_vote.stop();
                form.P[maxpos-1].Change_Leader();
                leaderpos = maxpos;
                ui->tiptable->setItem(maxpos-1,3,new QTableWidgetItem("是"));
                QMessageBox msgbox;
                msgbox.setText(QString::number(leaderpos)+"号玩家当选为村长");
                msgbox.setStandardButtons(QMessageBox::Ok);
                msgbox.setDefaultButton(QMessageBox::Ok);
                Timer_speech1.stop();
                msgbox.exec();
                time_left=10;
                Timer_speech1.start();
                for(int i=0;i<form.Get_PlayerNum();i++)
                {
                    form.P[i].Set_Voted(0.0);
                    ui->tiptable->setItem(i,4,new QTableWidgetItem(QString::number(0.0)));
                }
                Speech();//返回发言阶段
            }else//放逐路线  //等会完善这个东西记得链接只有一次
            {

            }
        }
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




