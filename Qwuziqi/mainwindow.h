#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtNetwork>
#include <QWidget>
#include <QtNetwork>
#include "GameModel.h"
#include <QPainter>
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMainUi(QWidget *parent);
    QWidget *par;

private:
    Ui::MainWindow *ui;
    GameModel*game;//游戏指针
    GameType game_type;//储存游戏类型
    //点击的位置
    int clickPosRow,clickPosCol;
    //是否选中
    bool selectPos=false;
    //绘制
    void paintEvent(QPaintEvent *event);

    void initGame();
    void initAIGame();
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*ebent);
    void chessOneByPerson();
private slots:
    void chessOneByAI();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();
};
#endif // MAINWINDOW_H
