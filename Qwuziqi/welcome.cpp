#include "welcome.h"
#include "ui_welcome.h"

welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{

    ui->setupUi(this);
    w.setMainUi(this);
    s.setMainUi(this);
    n.setMainUi(this);
    connect(ui->AI_btn,&QPushButton::clicked,this,&welcome::OnAiGameBtnClick);         // 绑定AI对战按钮

}
void welcome::OnAiGameBtnClick()
{
    this->hide();
    w.show();
}
welcome::~welcome()
{
    delete ui;
}




void welcome::on_set_btn_clicked()
{
    this->hide();
    s.show();
}

void welcome::on_NET_btn_clicked()
{
    this->hide();
    n.show();
}
