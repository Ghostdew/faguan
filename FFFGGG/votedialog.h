#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>

namespace Ui {
class VoteDialog;
}

class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(QWidget *parent,int playernum);
    ~VoteDialog();

private:
    Ui::VoteDialog *ui;

signals:
    void senddata(int data);

};

#endif // VOTEDIALOG_H
