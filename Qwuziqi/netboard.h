#ifndef NETBOARD_H
#define NETBOARD_H

#include <QApplication>
#include <QWidget>
#include <QtNetwork>
#include "GameModel.h"
#include <QPainter>
#define COUNTDOWN 599                   // 倒计时时间
namespace Ui {
class netBoard;
}

class netBoard : public QWidget
{
    Q_OBJECT

public:
    explicit netBoard(QWidget *parent = nullptr);
    ~netBoard();
    void setMainUi(QWidget *parent);
    QWidget *par;
      QTcpSocket *mSocket;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_send_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_6_clicked();


    void  replyFinished(QNetworkReply *reply);

public slots:
    void BlackTimerUpdate();            // 更新黑方定时器
    void WhiteTimerUpdate();            // 更新白方定时器

private:
    Ui::netBoard *ui;
    //TCP客户端
 QNetworkAccessManager  accessManager;
     QNetworkAccessManager  manager3;
    GameModel*game;//游戏指针
    GameType game_type;//储存游戏类型
    //点击的位置
    int clickPosRow,clickPosCol;
    //是否选中
    bool selectPos=false;
    bool flag=false;
    bool deleteflag=true;
    bool daojishi_flag=false;
    void initGame();
    void initMANGame();
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*ebent);
    void chessOneByPerson();
    void SwitchTimer();       // 切换选手倒计时
    int blackCountdown;                 // 黑方初始倒计时时间
    int whiteCountdown;                 // 白方初始倒计时时间


public:

    QTimer* blackTimerId;               // 黑方定时器
    QTimer* whiteTimerId;               // 白方定时器
    void daojishi();
};

#endif // NETBOARD_H
