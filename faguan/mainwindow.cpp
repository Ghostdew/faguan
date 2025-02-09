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


void MainWindow::Speech()//***********************************文本框输入还没做！！
{
    Timer_speech2.start(10000);
    Timer_speech1.start(1000);
    time_left = 10;


    static int count = 0;//计数器
    count = 0;

    if(leaderpos!=0)//村长选举完后发言阶段
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

        QMessageBox choosebox;
        choosebox.setText("请选择发言方向");
        choosebox.setWindowTitle("选择发言方向");
        choosebox.setWindowIcon(QIcon(":/image/titleicon.jpg"));
        choosebox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        choosebox.button(QMessageBox::Yes)->setText("Left");
        choosebox.button(QMessageBox::No)->setText("Right");
        Timer_speech1.stop();
        Timer_speech2.stop();
        int ret = choosebox.exec();//弹出模态窗口
        time_left = 10;
        Timer_speech1.start(1000);
        Timer_speech2.start(1);//直接触发切换当前活跃玩家
        if(ret==QMessageBox::Yes)
            direction = 1;//左边开始发言
        else
            direction = 0;//右边开始发言
    }else//第一次发言阶段
    {
        connect(&Timer_speech1,&QTimer::timeout,ui->lcdNumber,[=](){
            qDebug() << QString::number(time_left);
            ui->lcdNumber->display(time_left--);
        });
        connect(&Timer_speech2,&QTimer::timeout,this,[=](){
            Timer_speech2.start(10000);
            if(leaderpos==0)//第一轮选举村长发言阶段
            {
                Next_right();//右置位发言
                count++;
                if(count==form.Get_PlayerNum())
                {
                    Vote();//所有人都完成发言，进入投票阶段
                }
            }else
            {
                if(direction)//第二轮及以后的发言阶段,根据村长的选择由村长左置位或右置位开始发言
                {
                    Next_left();
                    count++;
                    if(count==form.Get_IdNumber(0)+form.Get_IdNumber(1)+1)//因为之前切换了一下，所以最后count要比真实发言人数多1
                    {
                        Vote();
                    }
                }else
                {
                    Next_right();
                    count++;
                    if(count==form.Get_IdNumber(0)+form.Get_IdNumber(1)+1)
                    {
                        Vote();
                    }
                }


            }
        });
        connect(ui->speechbtn,&QPushButton::clicked,[=](){
            if(Timer_speech2.isActive())
                Timer_speech2.start(1);
        });
    }





}

void MainWindow::Vote()
{
    ui->statuslabel3->setText("投票阶段");
    Timer_speech2.stop();
    Timer_speech1.start(1000);//刷新计时器
    time_left = 10;
    Timer_vote.start(10000);



    static VoteDialog *V = new VoteDialog(this);
    static int count2 = 0;
    count2 = 0;
    V->show();
    if(leaderpos==0)//只做一次信号和槽连接
    {
        connect(&Timer_vote,&QTimer::timeout,[=](){
            Timer_vote.start(10000);
            count2++;
            V->show();
            Next_right();
            time_left = 10;
            if(count2==form.Get_IdNumber(0)+form.Get_IdNumber(1))//选举结束判断
            {
                V->close();
                float maximum = form.P[0].Get_voted();
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

                    QMessageBox msgbox;//实现弹出对话框提示玩家村长人选
                    msgbox.setWindowTitle("系统提示");
                    msgbox.setWindowIcon(QIcon(":/image/titleicon.jpg"));
                    msgbox.setText(QString::number(leaderpos)+"号玩家当选为村长");
                    msgbox.setStandardButtons(QMessageBox::Ok);
                    msgbox.setDefaultButton(QMessageBox::Ok);

                    Timer_speech1.stop();
                    msgbox.exec();
                    time_left=10;
                    Timer_speech1.start();


                    for(int i=0;i<form.Get_PlayerNum();i++)//玩家票数归零
                    {
                        form.P[i].Set_Voted(0.0);
                        ui->tiptable->setItem(i,4,new QTableWidgetItem(QString::number(0.0)));
                    }
                    Speech();//返回发言阶段
                }else//放逐路线  //************************等会完善这个东西记得链接只有一次
                {
                    Timer_vote.stop();//停止计时器
                    QMessageBox exbox;//实现弹出对话框提示玩家被放逐人选
                    exbox.setStandardButtons(QMessageBox::Ok);
                    exbox.setDefaultButton(QMessageBox::Ok);
                    exbox.setWindowTitle("系统提示");
                    exbox.setWindowIcon(QIcon(":/image/titleicon.jpg"));

                    QString str_text;
                    QString str_red = "红牌";
                    QString str_black = "黑牌";
                    bool exleader = false;//村长被放逐标识

                    for(int i=0;i<form.Get_PlayerNum();i++)
                    {
                        if(form.P[i].Get_voted()==maximum&&form.P[i].Get_Death()==false)
                        {
                            str_text += QString::number(i+1)+"号玩家被放逐\n";
                            if(form.P[i].Get_Id())
                            {
                                str_text +="身份为："+str_black+"\n";
                                ui->tiptable->setItem(i,2,new QTableWidgetItem(str_black));
                            }
                            else
                            {
                                str_text +="身份为："+str_red+"\n";
                                ui->tiptable->setItem(i,2,new QTableWidgetItem(str_red));
                            }
                            if(form.P[i].Get_Leader())
                            {
                                exleader = true;
                                form.P[i].Change_Leader();
                                ui->tiptable->setItem(i,3,new QTableWidgetItem("否"));
                            }
                            form.P[i].Change_Death();
                            ui->tiptable->setItem(i,1,new QTableWidgetItem("死亡"));
                        }
                    }

                    exbox.setText(str_text);
                    time_left=10;
                    Timer_speech1.stop();
                    exbox.exec();

                    if(exleader)//村长被放逐，指定新任村长 ***************************今天写完可以吗？？很简单的，记得弹个对话框....简单个锤子，比night函数还难。。。。明天再写了，我感觉还得自己建一个dialog用exec（）否则不好写
                    {
                        QDialog *Choosewidgt = new QDialog(this);//创建选择新任村长窗口
                        Choosewidgt->setFixedSize(300,140);
                        Choosewidgt->setWindowTitle("系统提示");
                        Choosewidgt->setWindowIcon(QIcon(":/image/titleicon.jpg"));
                        QSpinBox *nbox = new QSpinBox(Choosewidgt);
                        nbox->move(190,30);
                        nbox->setMinimum(1);
                        QLabel *nlabel = new QLabel("请指定新任村长：",Choosewidgt);
                        nlabel->move(50,35);
                        QPushButton *ok =new QPushButton("确认",Choosewidgt);
                        ok->move(140,100);
                        connect(ok,&QPushButton::clicked,Choosewidgt,[=](){
                            int pos = nbox->value();
                            leaderpos = pos;
                            form.P[leaderpos-1].Change_Leader();
                            ui->tiptable->setItem(leaderpos-1,3,new QTableWidgetItem("是"));

                            QMessageBox msgbox;//实现弹出对话框提示玩家新村长人选
                            msgbox.setText(QString::number(leaderpos)+"号玩家当选为村长");
                            msgbox.setWindowTitle("系统提示");
                            msgbox.setWindowIcon(QIcon(":/image/titleicon.jpg"));
                            msgbox.setStandardButtons(QMessageBox::Ok);
                            msgbox.setDefaultButton(QMessageBox::Ok);
                            msgbox.exec();


                            Choosewidgt->close();
                        });
                        Choosewidgt->exec();
                        //delete Choosewidgt;
                    }

                    for(int i=0;i<form.Get_PlayerNum();i++)//玩家票数归零
                    {
                        form.P[i].Set_Voted(0.0);
                        ui->tiptable->setItem(i,4,new QTableWidgetItem(QString::number(0.0)));
                    }

                    if(form.GameOver())//判断游戏是否结束**************最后写
                    {
                        qDebug() << "GameOver";
                    }else
                    {
                        Night();
                    }

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
        });//弹出窗口所收集的值传递


    }

}

void MainWindow::Night()//夜晚睁眼阶段
{
    ui->statuslabel3->setText("夜晚阶段");
    form.Change_Night();
    int night = form.Get_Night();//由随机数判断是什么夜晚
    QString str_text = "今晚为：";
    QMessageBox night_msg;//创建弹出窗口告诉玩家是什么夜晚，并在左上角显示
    night_msg.setWindowTitle("系统提示");
    night_msg.setWindowIcon(QIcon(":/image/titleicon.jpg"));
    night_msg.setStandardButtons(QMessageBox::Ok);
    night_msg.setDefaultButton(QMessageBox::Ok);

    if(night)
    {
        ui->statuslabel2->setText("黑夜");
        str_text +="黑夜";
    }else
    {
        ui->statuslabel2->setText("红夜");
        str_text +="红夜";
    }
    night_msg.setText(str_text);
    night_msg.exec();
    //*******************************记得开计时器，之后睁眼要用
    for(int i=0;i<form.Get_PlayerNum();i++)//当前活跃玩家变为第一个没有死亡的玩家
    {
        if(!form.P[i].Get_Death())
        {
            activeplayer = i+1;
            if(form.P[i].Get_Id()==0)
            {
                ui->picturelabel->setStyleSheet("border-image: url(:/image/red.png)");
            }else
            {
                ui->picturelabel->setStyleSheet("border-image: url(:/image/black.png)");
            }
            ui->playerlabel2->setText(QString::number(form.P[activeplayer-1].Get_Position()));
            break;
        }
    }

    time_left = 10;
    Timer_speech1.start(1000);//分别启动计时器

    static int eyecount =0;
    eyecount = 0;
    if(day==1)//只做一次信号和槽的连接
    {
        connect(&Timer_night,&QTimer::timeout,[=](){
            Timer_night.start(10000);
            if(QMessageBox::Yes == QMessageBox::question(this,"系统提示","今晚是否睁眼",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
            {
                form.P[activeplayer-1].Change_Openeye();
                eyecount++;
                Next_right();
            }else
            {
                eyecount++;
                Next_right();
            }
            if(eyecount == form.Get_IdNumber(0)+form.Get_IdNumber(1))
            {
                time_left = 10;
                Timer_speech1.stop();
                Timer_night.stop();
                if(form.Get_Night() == 0&& form.Get_Openeyes(0)%2==0)
                {
                    leadvote = 0;
                }else
                {
                    if(form.Get_Night() == 1 && form.Get_Openeyes(1)%2==0)
                    {
                        leadvote = 0;
                    }else
                    {
                        leadvote = 0.5;
                    }
                }
                for(int i=0;i<form.Get_PlayerNum();i++)//睁眼人数归零
                {
                    if(form.P[i].Get_Openeye())
                        form.P[i].Change_Openeye();
                }
                static QMessageBox morning_msg;//创建弹出窗口告诉玩家是什么夜晚，并在左上角显示
                morning_msg.setWindowTitle("系统提示");
                morning_msg.setWindowIcon(QIcon(":/image/titleicon.jpg"));
                morning_msg.setStandardButtons(QMessageBox::Ok);
                morning_msg.setDefaultButton(QMessageBox::Ok);
                if(form.Get_PlayerNum()%2)
                    morning_msg.setText("天亮了，村长拥有"+QString::number(1-leadvote)+"票");
                else
                    morning_msg.setText("天亮了，村长拥有"+QString::number(1+leadvote)+"票");
                day++;
                ui->daylabel2->setText(QString::number(day));
                ui->statuslabel2->setText("白天");
                morning_msg.exec();
                Speech();
            }else
            {
                Timer_night.start(1);
            }
        });
    }

    Timer_night.start(1);

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
    if(form.P[activeplayer-1].Get_Death())//*************之后可再添加弹出窗口来完善，比如XX玩家已死亡之类的提示窗口
        Next_left();
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
    if(form.P[activeplayer-1].Get_Death())//*****************之后可再添加弹出窗口来完善
        Next_right();
}



MainWindow::~MainWindow()
{
    delete ui;
}




