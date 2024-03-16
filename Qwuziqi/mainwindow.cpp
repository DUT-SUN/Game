#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameModel.h"
#include <QMouseEvent>
#include<QPainter>
#include<math.h>
#include <QMessageBox>
#include <QTimer>
#include<QSound>
#include <qdebug.h>
// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/chessone.wav"
#define WIN_SOUND ":/res/win.wav"
#define LOSE_SOUND ":/res/lose.wav"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //设置窗口大小
    setFixedSize(MARGIN*2+BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2+BLOCK_SIZE*BOARD_GRAD_SIZE);

    initGame();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing,true);
    for (int i=0;i<16;i++){
         //从上往下
        painter.drawLine(MARGIN +BLOCK_SIZE*(i+1),MARGIN+BLOCK_SIZE,MARGIN +BLOCK_SIZE*(i+1),size().height()-MARGIN-BLOCK_SIZE);
      //从左到右
        painter.drawLine(MARGIN+BLOCK_SIZE,MARGIN+BLOCK_SIZE*(i+1),size().width()-MARGIN-BLOCK_SIZE,MARGIN+BLOCK_SIZE*(i+1));
    }
    //绘制选中点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    //绘制落点
    if(clickPosRow>0&&clickPosRow<BOARD_GRAD_SIZE&&
            clickPosCol>0&&clickPosCol<BOARD_GRAD_SIZE&&
            game->gameMapVec[clickPosRow][clickPosCol]==0){
        if(game->playerFlag)
            brush.setColor((Qt::black));
        else
            brush.setColor((Qt::white));
        painter.setBrush(brush);
        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE);
    }
    //绘制棋子
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
          for(int j=0;j<BOARD_GRAD_SIZE;j++){
              if(game->gameMapVec[i][j]==1){
                  brush.setColor(Qt::black);
                  painter.setBrush(brush);
                  painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);
              }else if(game->gameMapVec[i][j]==-1){
                  brush.setColor(Qt::white);
                  painter.setBrush(brush);
                  painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);
              }

          }
    }
    //判断输赢
        if(clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
              clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
              (game->gameMapVec[clickPosRow][clickPosCol]==1||game->gameMapVec[clickPosRow][clickPosCol]==-1)){
            //防止五个空白判别胜利
               if(game->isWin(clickPosRow,clickPosCol) && game->gameStatus == PLAYING){
                   game->gameStatus = WIN;
                         QSound::play(WIN_SOUND);

                   QString str;
                   str = game->gameMapVec[clickPosRow][clickPosCol]==1?"黑棋":"白棋";
                   QMessageBox::StandardButton btnValue = QMessageBox::information(this,"五子棋嬴家",str+"玩家赢得胜利！");
                       painter.end();
                   if(btnValue == QMessageBox::Ok){
                       game->startGame(game_type);
                       game->gameStatus = PLAYING;

                   }

               }
           }

}

void MainWindow::initGame(){
    game=new GameModel;
    initAIGame();
}

void MainWindow::initAIGame(){
    game_type=AI;
    game->gameStatus=PLAYING;
    //数据模型中进行初始化功能
    game->startGame(game_type);
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent*event){
    int x=event->x();
    int y=event->y();
    //棋盘边缘不可以落子
    if(x >= MARGIN + BLOCK_SIZE/2 &&
            x<size().width()-MARGIN-BLOCK_SIZE/2&&
            y>=MARGIN +BLOCK_SIZE/2&&
            y<size().height()-MARGIN-BLOCK_SIZE/2)
    {
        //获取最近的左上角点
        int row=(y-MARGIN)/BLOCK_SIZE;
        int col=(x-MARGIN)/BLOCK_SIZE;
        int leftTopPosX=MARGIN+BLOCK_SIZE*col;
        int leftTopPosY=MARGIN+BLOCK_SIZE*row;
        clickPosRow=-1;
        clickPosCol=-1;
        int len=0;
        selectPos=false;
        //左上角距离
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow=row;
            clickPosCol=col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        //右上角距离
        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow=row;
            clickPosCol=col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        //左下角距离
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow=row+1;
            clickPosCol=col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        //右下角距离
        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow=row+1;
            clickPosCol=col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }

    }
    update();
}
void MainWindow::chessOneByPerson(){
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->gameMapVec[clickPosRow][clickPosCol]==0){
        game->actionByPerson(clickPosRow,clickPosCol);
         QSound::play(CHESS_ONE_SOUND);
        update();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent*event){
    if(selectPos==false){
        return;
    }else{
        selectPos=false;
    }
    chessOneByPerson();
    if(game_type==AI){
         QTimer::singleShot(AI_THINK_TIME,this,SLOT(chessOneByAI()));

    }


}
void MainWindow::chessOneByAI(){
    game->actionByAI(clickPosRow,clickPosCol);
     QSound::play(CHESS_ONE_SOUND);
    update();
}

void MainWindow::setMainUi(QWidget *parent)
{
    par = parent;
}

void MainWindow::on_pushButton_clicked()
{
//    qDebug() << "on_pushButton_clicked";
    this->hide();
    par->show();
}


void MainWindow::on_pushButton_2_clicked()
{
     initGame();
}


void MainWindow::on_pushButton_3_clicked()
{
    game->backChess();
}
