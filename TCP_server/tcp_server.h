#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QWidget>
#include <QtNetwork>
#include "GameModel.h"
#include <QPainter>
#include <QMainWindow>
#include <QMessageBox>
#define COUNTDOWN 599                   // 倒计时时间
QT_BEGIN_NAMESPACE
namespace Ui { class TCP_server; }
QT_END_NAMESPACE

class TCP_server : public QWidget
{
    Q_OBJECT

public:
    TCP_server(QWidget *parent = nullptr);
    ~TCP_server();

private slots:
    void on_push_button_clicked();

    void on_begin_button_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TCP_server *ui;
    //TCP服务器
    QTcpServer *mServer;
    QTcpSocket*mSocket;
    GameModel*game;//游戏指针
    GameType game_type;//储存游戏类型
    bool flag=false;//判断是否连接
    //点击的位置
    int clickPosRow,clickPosCol;
    //是否选中
    bool selectPos=false;
    void initGame();
    void initMANGame();
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*ebent);
    void chessOneByPerson();
    void daojishi();
     bool daojishi_flag=false;
public slots:
    void BlackTimerUpdate();            // 更新黑方定时器
    void WhiteTimerUpdate();            // 更新白方定时器



public:
    void SwitchTimer();       // 切换选手倒计时
    int blackCountdown;                 // 黑方初始倒计时时间
    int whiteCountdown;                 // 白方初始倒计时时间
    QTimer* blackTimerId;               // 黑方定时器
    QTimer* whiteTimerId;               // 白方定时器
};
#endif // TCP_SERVER_H
