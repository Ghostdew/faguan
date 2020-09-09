#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    Platform Base(8);
//    for(int i=0;i<Base.P.size();i++)
//      qDebug()<< Base.P[i].Get_Id()<<' '<< Base.P[i].Get_Position() <<endl;
    return a.exec();
}
