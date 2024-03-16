#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include"mainwindow.h"
#include"set.h"
#include"netboard.h"
namespace Ui {
class welcome;
}

class welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = nullptr);
    ~welcome();

public slots:
    void OnAiGameBtnClick();            // 人机对战槽函数
    void on_set_btn_clicked();          //设置按钮

private slots:
    void on_NET_btn_clicked();

private:
    Ui::welcome *ui;
set s;
MainWindow w;
netBoard n;

};

#endif // WELCOME_H
