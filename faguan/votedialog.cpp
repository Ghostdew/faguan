#include "votedialog.h"
#include "ui_votedialog.h"

VoteDialog::VoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{
    ui->setupUi(this);
    setWindowTitle("投票");
    connect(ui->pushButton,&QPushButton::clicked,[=](){
       int num=ui->spinBox->value();
       emit senddata(num);
       this->close();
    });
}

VoteDialog::~VoteDialog()
{
    delete ui;
}
