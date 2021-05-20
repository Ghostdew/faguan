#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent,int num) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1200,780);//固定窗口大小

    QApplication::setQuitOnLastWindowClosed(false);

    QLabel *helplabel = new QLabel(this);
    helplabel->setFixedSize(1314,673);
    helplabel->setStyleSheet("border-image: url(:/image/help.png);");
    helplabel->hide();//帮助页面的设置

    connect(ui->actionExit,&QAction::triggered,this,[=](){
        QApplication::setQuitOnLastWindowClosed(true);
        this->close();
    });//菜单栏退出按钮实现

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


    connect(ui->hideButton,&QPushButton::clicked,[=](){

        ui->tiptable->hide();
        ui->hideButton->hide();
        ui->showButton->show();
    });

    connect(ui->showButton,&QPushButton::clicked,[=](){

        ui->tiptable->show();
        ui->showButton->hide();
        ui->hideButton->show();
    });
    form.init(num);//玩家维护队列初始化
    Go();//开始游戏
}

void MainWindow::paintEvent(QPaintEvent *event)//绘制背景图
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mainbackground.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}


void MainWindow:: Go()
{
    ui->showButton->setVisible(false);
    ui->endTextLabel->setVisible(false);
    ui->endTitleLabel->setVisible(false);
    ui->exitToolButton->setVisible(false);
    ui->restartToolButton->setVisible(false);
    ui->closeButton->setVisible(false);
    ui->tiptable->setColumnCount(5);//表格初始化
    ui->tiptable->setHorizontalHeaderLabels(QStringList()<<"玩家名"<<"存活情况"<<"身份"<<"是否为村长"<<"所得票数");
    ui->statuslabel3->setText("村长选举发言阶段");
    ui->statuslabel3->setAlignment(Qt::AlignHCenter);//居中对齐
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

void MainWindow:: endGame(int type)
{
    ui->picturelabel->setVisible(false);//隐藏控件
    ui->tiptable->setVisible(false);
    ui->daylabel1->setVisible(false);
    ui->daylabel2->setVisible(false);
    ui->statuslabel1->setVisible(false);
    ui->statuslabel2->setVisible(false);
    ui->statuslabel3->setVisible(false);
    ui->playerlabel1->setVisible(false);
    ui->playerlabel2->setVisible(false);
    ui->Input_textEdit->setVisible(false);
    ui->Output_textEdit->setVisible(false);
    ui->speechbtn->setVisible(false);
    ui->TimeLabel->setVisible(false);
    ui->lcdNumber->setVisible(false);
    ui->showButton->setVisible(false);
    ui->hideButton->setVisible(false);
    ui->tableLabel->setVisible(false);
    ui->helpButton->setVisible(false);
    ui->closeButton->setVisible(false);
    ui->speechLabel->setVisible(false);


    ui->endTitleLabel->setVisible(true);//显示游戏结束控件
    if(type)
        ui->endTextLabel->setText("黑牌阵营获胜");
    else
        ui->endTextLabel->setText("红牌阵营获胜");
    ui->endTextLabel->setVisible(true);
    ui->exitToolButton->setVisible(true);
    ui->restartToolButton->setVisible(true);

    connect(ui->restartToolButton,&QToolButton::pressed,ui->restartToolButton,[=](){
        ui->restartToolButton->setStyleSheet("border-image: url(:/image/restartbutton2.png);");
    });//重玩按钮点击变化实现



    connect(ui->restartToolButton,&QToolButton::released,[=](){
        ui->restartToolButton->setStyleSheet("border-image: url(:/image/restartbutton.png);");
        this->hide();
        QApplication::setQuitOnLastWindowClosed(true);
        emit restart();
    });//重玩按钮点击变化实现

    connect(ui->exitToolButton,&QToolButton::pressed,ui->exitToolButton,[=](){
        ui->exitToolButton->setStyleSheet("border-image: url(:/image/exitbutton2.png);");
        QApplication::setQuitOnLastWindowClosed(true);
    });//退出按钮点击变化实现



    connect(ui->exitToolButton,&QToolButton::released,[=](){
        ui->exitToolButton->setStyleSheet("border-image: url(:/image/exitbutton.png);");
        this->close();
    });//退出按钮点击变化实现
}


void MainWindow::Speech()//发言函数
{
    ui->Output_textEdit->clear();//再次投票时清除输出文本框内容
    Timer_speech.start(speech_time*1000);
    Timer_Display.start(1000);
    time_left = speech_time;


    static int count = 0;//判断是否所有可发言玩家都发言完毕的计数器
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

        QMessageBox *choosebox = new QMessageBox(this);
        choosebox->setText("请选择发言方向");
        choosebox->setWindowTitle("选择发言方向");
        choosebox->setWindowIcon(QIcon(":/image/titleicon.jpg"));
        choosebox->setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        choosebox->button(QMessageBox::Yes)->setText("Left");
        choosebox->button(QMessageBox::No)->setText("Right");
        Timer_Display.stop();
        Timer_speech.stop();
        int ret = choosebox->exec();//弹出模态窗口
        time_left = speech_time;
        Timer_Display.start(1000);
        Timer_speech.start(1);//直接触发切换当前活跃玩家
        if(ret==QMessageBox::Yes)
            direction = 1;//左边开始发言
        else
            direction = 0;//右边开始发言
    }else//第一次发言阶段
    {
        connect(&Timer_Display,&QTimer::timeout,ui->lcdNumber,[=](){

            ui->lcdNumber->display(time_left--);
        });
        connect(&Timer_speech,&QTimer::timeout,this,[=](){
            Timer_speech.start(speech_time*1000);

            QString speech_text = ui->Input_textEdit->toPlainText();//读取文本框中文字
            ui->Input_textEdit->clear();//清楚输入文本框中文字


            if(leaderpos==0)//第一轮选举村长发言阶段
            {
                ui->Output_textEdit->insertPlainText(QString::number(activeplayer)+"号玩家："+speech_text+"\n");
                Next_right();//右置位发言
                time_left = speech_time;
                count++;



                if(count==form.Get_PlayerNum())
                {
                    Vote();//所有人都完成发言，进入投票阶段
                }
            }else
            {
                if(direction)//第二轮及以后的发言阶段,根据村长的选择由村长左置位或右置位开始发言
                {
                    if(count>=1)//因为第一个是村长直接切换，村长不发言
                    {
                        ui->Output_textEdit->insertPlainText(QString::number(activeplayer)+"号玩家："+speech_text+"\n");
                    }
                    Next_left();
                    time_left = speech_time;
                    count++;

                    if(count==form.Get_IdNumber(0)+form.Get_IdNumber(1)+1)//因为之前切换了一下，所以最后count要比真实发言人数多1
                    {
                        Vote();
                    }
                }else
                {
                    if(count>=1)//因为第一个是村长直接切换，村长不发言
                    {
                        ui->Output_textEdit->insertPlainText(QString::number(activeplayer)+"号玩家："+speech_text+"\n");
                    }
                    Next_right();
                    time_left = speech_time;
                    count++;

                    if(count==form.Get_IdNumber(0)+form.Get_IdNumber(1)+1)
                    {
                        Vote();
                    }
                }
            }
        });
        connect(ui->speechbtn,&QPushButton::clicked,[=](){
            if(Timer_speech.isActive())
                Timer_speech.start(1);
        });//连接发言按钮和定时器
    }

}

void MainWindow::Vote()
{
    ui->statuslabel3->setText("投票阶段");
    Timer_speech.stop();
    Timer_Display.start(1000);//刷新计时器
    time_left = vote_time;
    Timer_vote.start(vote_time*1000);


    VoteDialog *V = new VoteDialog(this,form.Get_PlayerNum());//投票窗口实例创建
    static int count2 = 0;
    count2 = 0;
    V->show();
    if(leaderpos==0)//只做一次信号和槽连接，判断是否第一次进入发言函数
    {
        connect(&Timer_vote,&QTimer::timeout,[=](){
            Timer_vote.start(vote_time*1000);
            V->close();
            count2++;
            V->show();
            Next_right();
            time_left = vote_time;
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

                    Timer_Display.stop();
                    msgbox.exec();
                    time_left=10;
                    Timer_Display.start();


                    for(int i=0;i<form.Get_PlayerNum();i++)//玩家票数归零
                    {
                        form.P[i].Set_Voted(0.0);
                        ui->tiptable->setItem(i,4,new QTableWidgetItem(QString::number(0.0)));
                    }
                    Speech();//返回发言阶段
                }else//放逐路线
                {
                    Timer_vote.stop();//停止计时器
                    QMessageBox *exbox = new QMessageBox(this);//实现弹出对话框提示玩家被放逐人选
                    exbox->setStandardButtons(QMessageBox::Ok);
                    exbox->setDefaultButton(QMessageBox::Ok);
                    exbox->setWindowTitle("系统提示");
                    exbox->setWindowIcon(QIcon(":/image/titleicon.jpg"));

                    QString str_text;
                    QString str_red = "红牌";
                    QString str_black = "黑牌";
                    bool exleader = false;//村长被放逐标识
                    int deadnum = 0;//一次被放逐的人数
                    for(int i=0;i<form.Get_PlayerNum();i++)
                    {
                        if(form.P[i].Get_voted()==maximum&&form.P[i].Get_Death()==false&&maximum!=0)
                        {
                            deadnum++;
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
                    if(deadnum==0)
                        str_text = "无人被放逐";
                    exbox->setText(str_text);
                    time_left=vote_time;
                    Timer_Display.stop();
                    exbox->exec();



                    if(form.GameOver())//判断游戏是否结束
                    {
                        int type = form.GameOver();
                        if(type==1)
                        {
                            //qDebug() << "red win!";
                            endGame(0);
                        }else
                        {
                            endGame(1);
                            //qDebug() << "black win!";
                        }

                    }else
                    {
                        if(exleader)//村长被放逐，指定新任村长
                        {
                            QDialog *Choosewidgt = new QDialog(this);//创建选择新任村长窗口
                            Choosewidgt->setFixedSize(300,140);
                            Choosewidgt->setWindowTitle("系统提示");
                            Choosewidgt->setWindowIcon(QIcon(":/image/titleicon.jpg"));
                            QSpinBox *nbox = new QSpinBox(Choosewidgt);
                            nbox->move(190,30);
                            nbox->setMinimum(1);
                            nbox->setMaximum(form.Get_PlayerNum());
                            QLabel *nlabel = new QLabel("请指定新任村长：",Choosewidgt);
                            nlabel->move(50,35);
                            QPushButton *ok =new QPushButton("确认",Choosewidgt);
                            ok->move(140,100);
                            connect(ok,&QPushButton::clicked,Choosewidgt,[=](){
                                int pos = nbox->value();
                                if(form.P[pos-1].Get_Death())
                                {
                                    QMessageBox::warning(this,"系统提示","指定玩家已死亡，请重新指定！");
                                }else
                                {
                                    leaderpos = pos;
                                    form.P[leaderpos-1].Change_Leader();
                                    ui->tiptable->setItem(leaderpos-1,3,new QTableWidgetItem("是"));

                                    QMessageBox *msgbox = new QMessageBox(this);//实现弹出对话框提示玩家新村长人选
                                    msgbox->setText(QString::number(leaderpos)+"号玩家当选为村长");
                                    msgbox->setWindowTitle("系统提示");
                                    msgbox->setWindowIcon(QIcon(":/image/titleicon.jpg"));
                                    msgbox->setStandardButtons(QMessageBox::Ok);
                                    msgbox->setDefaultButton(QMessageBox::Ok);
                                    msgbox->exec();

                                    Choosewidgt->close();
                                }

                            });
                            Choosewidgt->exec();
                        }

                        for(int i=0;i<form.Get_PlayerNum();i++)//玩家票数归零
                        {
                            form.P[i].Set_Voted(0.0);
                            ui->tiptable->setItem(i,4,new QTableWidgetItem(QString::number(0.0)));
                        }
                        Night();
                    }
                }
            }
        });
    }
    connect(V,&VoteDialog::senddata,[=](int num){
        if(!form.P[num-1].Get_Death())
        {
            V->close();
            if(form.P[activeplayer-1].Get_Leader())
            {
                form.P[num-1].Set_Voted(1+leadvote+form.P[num-1].Get_voted());
            }else
            {
                form.P[num-1].Set_Voted(1+form.P[num-1].Get_voted());
            }
            ui->tiptable->setItem(num-1,4,new QTableWidgetItem(QString::number(form.P[num-1].Get_voted())));
            //qDebug() << "Timer triggered";
            Timer_vote.start(1);
        }else//投给已死之人可以重投
        {
            QMessageBox *errorbox = new QMessageBox(this);//实现弹出对话框提示玩家投票错误
            errorbox->setWindowTitle("系统提示");
            errorbox->setWindowIcon(QIcon(":/image/titleicon.jpg"));
            errorbox->setText(QString::number(num)+"号玩家已死亡，请重新投票！");
            errorbox->setStandardButtons(QMessageBox::Ok);
            errorbox->setDefaultButton(QMessageBox::Ok);
            Timer_vote.stop();
            Timer_Display.stop();
            errorbox->exec();
            Timer_vote.start(vote_time*1000);
            time_left = vote_time;
            Timer_Display.start(1000);
            V->exec();
        }

    });//弹出窗口所收集的值传递

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
    night_msg.exec();//夜晚提示窗口弹出

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

    time_left = vote_time;//重置显示器时间
    Timer_Display.start(1000);//分别启动计时器

    static int eyecount =0;//判断是否所有人都选择的计数器
    eyecount = 0;//计数器初始化
    if(day==1)//只做一次信号和槽的连接
    {
        connect(&Timer_night,&QTimer::timeout,[=](){
            Timer_night.start(vote_time*1000);
            if(QMessageBox::Yes == QMessageBox::question(this,"系统提示","今晚是否睁眼",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
            {
                //qDebug() << "enter eye";
                form.P[activeplayer-1].Change_Openeye();
                eyecount++;
                Next_right();
                time_left = vote_time;
            }else
            {
                //qDebug() << "enter ceye";
                eyecount++;
                Next_right();
                time_left = vote_time;
            }
            if(eyecount == form.Get_IdNumber(0)+form.Get_IdNumber(1))
            {
                time_left = vote_time;
                Timer_Display.stop();
                Timer_night.stop();
                if(form.Get_Night()==0&&form.Get_Openeyes(0)%2==0&&form.Get_Openeyes(0))
                {
                    leadvote = 0;
                }else
                {
                    if(form.Get_Night()==1&&form.Get_Openeyes(1)%2==0&&form.Get_Openeyes(0))
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
                static QMessageBox morning_msg;//创建弹出窗口告诉玩家夜晚发生了什么
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

void MainWindow::Next_left()//以环型向左切换下一个玩家
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
    if(form.P[activeplayer-1].Get_Death())//*************之后可再添加弹出窗口来完善，比如XX玩家已死亡之类的提示窗口
        Next_left();
}

void MainWindow::Next_right()//以环型向右切换下一个玩家
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
    if(form.P[activeplayer-1].Get_Death())//*****************之后可再添加弹出窗口来完善
        Next_right();
}

void MainWindow::closeEvent(QCloseEvent *event)//窗口关闭事件，弹出对话框
{
    if(QMessageBox::Yes==QMessageBox::warning(this,"系统提示","确定离开吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        QApplication::setQuitOnLastWindowClosed(true);
        event->accept();
    }else
    {
        event->ignore();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
