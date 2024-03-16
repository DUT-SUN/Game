#include "tcp_server.h"
#include "ui_tcp_server.h"
#include<QDebug>
#include <QPlainTextEdit>
#include "GameModel.h"
#include <QMouseEvent>
#include<QPainter>
#include<math.h>
#include<QMessageBox>
#include <QTimer>
#include<QSound>
#include <qdebug.h>
// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/chessone.wav"
#define WIN_SOUND ":/res/win.wav"
#define LOSE_SOUND ":/res/lose.wav"
TCP_server::TCP_server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TCP_server)
{
    ui->setupUi(this);
    this->setWindowTitle("联机模式");
    initGame();
}

TCP_server::~TCP_server()
{
    delete ui;
}



//启动tcp服务器
void TCP_server::on_begin_button_clicked()
{

    //1.构造QtcpServer对象
    mServer=new QTcpServer(this);
    //2.连接客户端连接上来的信号
    QObject::connect(mServer,&QTcpServer::newConnection,this,[&](){
        mSocket=mServer->nextPendingConnection();
        QHostAddress c_addr=mSocket->peerAddress();
        flag=true;

        qDebug()<<c_addr.toString()<<"连接上来!";
        //使能发送输入框和按钮
        ui->lineEdit->setEnabled((true));
        ui->push_button->setEnabled((true));
        QObject::connect(mSocket,&QTcpSocket::readyRead,this,[&](){
//            int x,y;
//            QByteArray arry = tcpSock->readAll();
//            QString str = QString(arry);

//            QString chessback = str.section("#",0,0);
//            qDebug()<<"Value 0f chessback"<<chessback;
//            if(chessback == "chessback"){
//                gameBoard.backChess();
//                gameBoard.backChess();
//                moveX = -1;
//                moveY = -1;
//                update();
//                QMessageBox msgBox;
//                msgBox.setText("对方悔棋！");
//                msgBox.exec();
//            }else{
             int x,y;
             QByteArray arry =mSocket->readAll();
             QString str = QString(arry);
             QString flagx = str.section("#",0,0);

             if(flagx == "chessback"){
                ui->textBrowser->insertPlainText("黑方执行悔棋操作\n");
                 game->backChess();
             }
             else if(flagx=="touxiang"){
                 initGame();
                   ui->textBrowser->insertPlainText("对方投降\n");
             }
             else if(flagx=="qiuhe"){
                 initGame();
                 ui->textBrowser->insertPlainText("对方求和\n");
             }
             else if(flagx =="limitedtime"){
                  qDebug()<<"执行了1";
                 if(!daojishi_flag){
              ui->textBrowser->insertPlainText("对方申请开启了限时模式\n");
               daojishi();
               daojishi_flag=true;
               qDebug()<<daojishi_flag;
                 }else{
 qDebug()<<"执行了2";
                     ui->Black_timer->display("0");  // 初始化LCD显示
                     ui->White_timer->display("0");
                     disconnect(blackTimerId,0,0,0);
                     disconnect(whiteTimerId,0,0,0);
                     disconnect(game,0,0,0);
                     daojishi_flag=false;
                 }

             }
             else{
              x=str.section("#",0,0).toInt();
              y=str.section("#",1,1).toInt();
              if(x!=NULL&&y!=NULL){
                  game->actionByPerson(x,y);
                  qDebug()<<'('<<x<<','<<y<<')';
                   QSound::play(CHESS_ONE_SOUND);
                  update();
              }else{
                    ui->textBrowser->insertPlainText("黑方:"+str+"\n");
              }
             }



//           //接受数据
//            QByteArray arr=mSocket->readAll();
//            //转化成字符串
//            QString str(arr);

            //显示

        });
    });

    //3.监听
    mServer->listen(QHostAddress::Any,ui->spinBox->value());


    QObject::connect(ui->lineEdit,&QLineEdit::returnPressed,this,&TCP_server::on_push_button_clicked);
    //禁止端口号和启动按钮
    ui->spinBox->setEnabled((false));
    ui->begin_button->setEnabled((false));

}
void TCP_server::on_push_button_clicked()
{
//获取发送的数据
    QString str=ui->lineEdit->text();
    //转化成QByteArray
    QByteArray arr;
    arr.append(str);
    //发送
    mSocket->write(arr);
    //显示要发送的内容
    ui->textBrowser->insertPlainText("白方:"+str+"\n");
    //清空输入框
    ui->lineEdit->clear();
}

void TCP_server::initGame(){
    game=new GameModel;
    initMANGame();
}



void TCP_server::initMANGame(){
         daojishi_flag=false;
    game_type=MAN;
    game->gameStatus=PLAYING;
    //数据模型中进行初始化功能
    game->startGame(game_type);
    update();
}

void TCP_server::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing,true);

    for (int i=0;i<16;i++){
         //从上往下
        painter.drawLine(MARGIN +BLOCK_SIZE*(i+1),MARGIN+BLOCK_SIZE,MARGIN +BLOCK_SIZE*(i+1),size().height()-MARGIN-BLOCK_SIZE);
      //从左到右
        painter.drawLine(MARGIN+BLOCK_SIZE,MARGIN+BLOCK_SIZE*(i+1),size().width()-MARGIN-BLOCK_SIZE-241-76,MARGIN+BLOCK_SIZE*(i+1));
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
                   if(btnValue == QMessageBox::Ok){
                       game->startGame(game_type);
                       game->gameStatus = PLAYING;
                   }

               }
           }

}

void TCP_server::mouseMoveEvent(QMouseEvent*event){
    int x=event->x();
    int y=event->y();
    //棋盘边缘不可以落子
    if(x >= MARGIN + BLOCK_SIZE/2 &&
            x<size().width()-241-76-MARGIN-BLOCK_SIZE/2&&
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
void TCP_server::chessOneByPerson(){
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->gameMapVec[clickPosRow][clickPosCol]==0){
        game->actionByPerson(clickPosRow,clickPosCol);
        qDebug()<<'('<<clickPosRow<<','<<clickPosCol<<')';
         QSound::play(CHESS_ONE_SOUND);
        update();
    }
}
void TCP_server::mouseReleaseEvent(QMouseEvent*event){
//    if(selectPos==false){
//        return;
//    }else{
//        selectPos=false;
//    }
//    chessOneByPerson();
//    if(game_type==AI){
//         QTimer::singleShot(AI_THINK_TIME,this,SLOT(chessOneByAI()));

//    }

    //鼠标释放事件
    if(flag){

    if( game->playerFlag){//等待对方
        QMessageBox msgBox;
        msgBox.setText("请等待客户端连接网络或下棋！");
        msgBox.exec();
        return;
    }

    if( ! game->playerFlag)
    {
            if(selectPos==false){
                return;
            }else{
                selectPos=false;
            }
            chessOneByPerson();
            qDebug()<<clickPosRow<<clickPosCol;
                QString str = QString::number(clickPosRow) + "#" + QString::number(clickPosCol);
                QByteArray arry;
                arry.append(str);
                mSocket->write(arry);




        update();

}
}
}



void TCP_server::on_pushButton_3_clicked()
{

        if(flag){//已经连接以后
            if( !game->playerFlag){

           game->backChess();
           QString a = "chessback";
           QString str = a + "#";
            QByteArray arry;
            arry.append(str);
            mSocket->write(arry);
        }
            else{
                QMessageBox msgBox;
                msgBox.setText("当前是对方回合，无法悔棋！");
                msgBox.exec();
            }
       }

    }


void TCP_server::on_pushButton_4_clicked()
{
    if(flag){//已经连接以后
        initGame();
          ui->textBrowser->insertPlainText("您已申请投降\n");


       QString a = "touxiang";
       QString str = a + "#";
        QByteArray arry;
        arry.append(str);
        mSocket->write(arry);


   }
}

void TCP_server::on_pushButton_5_clicked()
{
    if(flag){//已经连接以后
        initGame();
          ui->textBrowser->insertPlainText("您已申请求和\n");


       QString a = "qiuhe";
       QString str = a + "#";
        QByteArray arry;
        arry.append(str);
        mSocket->write(arry);


   }
}

void TCP_server::BlackTimerUpdate()
{
    // 将时间 String 显示在 LCD_NUM 组件上，Timer 计时1秒后发送timeout信号，TimerUpdate接受到timeout信号后更新倒计时时间
    QString timeString = QString("%1:%2").arg(blackCountdown/60,2,10,QChar('0')).arg(blackCountdown%60,2,10,QChar('0'));
    ui->Black_timer->display(timeString);
    if(!blackCountdown)
    {
         blackCountdown = COUNTDOWN;    // 初始化倒计时时间
         whiteCountdown = COUNTDOWN;
          ui->textBrowser->insertPlainText("您输了！");
          initGame();
    }
    else
    {
         blackCountdown--;
    }
}

void TCP_server::WhiteTimerUpdate()
{
    // 将时间 String 显示在 LCD_NUM 组件上，Timer 计时1秒后发送timeout信号，TimerUpdate接受到timeout信号后更新倒计时时间
    QString timeString = QString("%1:%2").arg(whiteCountdown/60,2,10,QChar('0')).arg(whiteCountdown%60,2,10,QChar('0'));
    ui->White_timer->display(timeString);
    if(!whiteCountdown)
    {
         blackCountdown = COUNTDOWN;    // 初始化倒计时时间
         whiteCountdown = COUNTDOWN;

          ui->textBrowser->insertPlainText("白棋时间到，您赢了！");
          initGame();
    }
    else
    {
        whiteCountdown--;
    }
}


void TCP_server::on_pushButton_2_clicked()
{

    //发送消息开启计时
    if(flag){//已经连接以后
        //不是我的回合点击提醒
        //是我的回合去计时
        //不是我的回合对面发过来请求计时
        //发过来请求是我的回合什么都不能做
  if(!daojishi_flag){

        if(!game->playerFlag){//如果是你的回合，要去发送信号，告诉对方你开启的，一起计时，假如不是你的回合直接计时
          ui->textBrowser->insertPlainText("您已开启限时模式\n");
                           QString a = "limitedtime";
                           QString str = a + "#";
                            QByteArray arry;
                            arry.append(str);
                            mSocket->write(arry);
                            daojishi_flag=true;
                            daojishi();
        }else{
            QMessageBox::warning(0 , "限时模式" , "不是你的回合，不可以申请限时", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0 );
            return;

        }


    }else{
      QString a = "limitedtime";
      QString str = a + "#";
       QByteArray arry;
       arry.append(str);
       mSocket->write(arry);
        ui->Black_timer->display("0");  // 初始化LCD显示
        ui->White_timer->display("0");
        disconnect(blackTimerId,0,0,0);
        disconnect(whiteTimerId,0,0,0);
        disconnect(game,0,0,0);
         daojishi_flag=false;
    }
}
}

 void TCP_server::daojishi()
 {
     // 初始化控件
     blackTimerId = new QTimer(this);
     whiteTimerId = new QTimer(this);

     // 初始化信号与槽
     connect(blackTimerId,&QTimer::timeout,this,&TCP_server::BlackTimerUpdate,Qt::QueuedConnection);
     connect(whiteTimerId,&QTimer::timeout,this,&TCP_server::WhiteTimerUpdate,Qt::QueuedConnection);
      connect(game,&GameModel::TurnToNextPlayer,this,&TCP_server::SwitchTimer,Qt::QueuedConnection);
//   }
     // 初始化倒计时
     blackCountdown = COUNTDOWN;         // 初始化倒计时时间
     whiteCountdown = COUNTDOWN;

     ui->Black_timer->display("10:00");  // 初始化LCD显示
     ui->White_timer->display("10:00");
 }
void TCP_server::SwitchTimer()
{
    if(game->playerFlag)
    {
        whiteTimerId->stop();       // 白棋定时器停止
        blackTimerId->start(1000);  // 黑棋定时器启动
    }
    else
    {
        blackTimerId->stop();       // 黑棋定时器停止
        whiteTimerId->start(1000);  // 白棋定时器启动
    }
}
