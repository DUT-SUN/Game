#include "netboard.h"
#include "ui_netboard.h"
#include "GameModel.h"
#include <QMouseEvent>
#include <QTimer>
#include<QSound>
#include <QPainter>
#include <math.h>
#include <QMessageBox>

#include <QFileDialog>
#include <QDebug>

// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/chessone.wav"
#define WIN_SOUND ":/res/win.wav"
#define LOSE_SOUND ":/res/lose.wav"

netBoard::netBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::netBoard)
{

    ui->setupUi(this);


    this->setWindowTitle("联机模式");


    connect(&accessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));


    initGame();
}
void netBoard::initGame(){
    game=new GameModel;

    initMANGame();
}


void netBoard::initMANGame(){
     daojishi_flag=false;
    game_type=MAN;//
    game->gameStatus=PLAYING;
    //数据模型中进行初始化功能
    game->startGame(game_type);
    update();
}
netBoard::~netBoard()
{
    delete ui;
}


void netBoard::on_pushButton_clicked()
{

    this->hide();
    par->show();
}
void netBoard::setMainUi(QWidget *parent)
{
    par = parent;
}
void netBoard::on_pushButton_start_clicked()
{

    //连接服务器

    //1.创建QTcpSocket对象
    mSocket=new QTcpSocket(this);
    //2.连接成功的信号和槽
    QObject::connect(mSocket,&QTcpSocket::connected,this,[&]{
        //使能发送和输入
        ui->lineEdit_send->setEnabled(true);
         ui->pushButton_send->setEnabled(true);
         //禁止ip端口和连接
         ui->label_ip->setEnabled(false);
         ui->spinBox->setEnabled(false);
         ui->pushButton_start->setEnabled(false);
         flag=true;

             QObject::connect(mSocket,&QTcpSocket::readyRead,this,[&](){


                 int x,y;
                 QByteArray arry =mSocket->readAll();
                 QString str = QString(arry);
                 QString flagx = str.section("#",0,0);

                 if(flagx == "chessback"){
                    ui->textBrowser->insertPlainText("白方执行悔棋操作\n");
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
                        ui->textBrowser->insertPlainText("白方:"+str+"\n");
                  }
                 }
             });
         });
    //3.连接服务器
    QString ip=ui->label_ip->text();
    quint16 port =ui->spinBox->value();
    mSocket->connectToHost(ip,port);

    //连接回车发送的信号和槽
     QObject::connect(ui->lineEdit_send,&QLineEdit::returnPressed,this,&netBoard::on_pushButton_send_clicked);
}


//发送消息

void netBoard::on_pushButton_send_clicked()
{
    //获取发送的数据
        QString str=ui->lineEdit_send->text();
        //转化成QByteArray
        QByteArray arr;
        arr.append(str);
        //发送
        mSocket->write(arr);
        //显示要发送的内容
        ui->textBrowser->insertPlainText("黑方:"+str+"\n");
        //清空输入框
        ui->lineEdit_send->clear();
}


void netBoard::paintEvent(QPaintEvent *event)
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

void netBoard::mouseMoveEvent(QMouseEvent*event){
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
void netBoard::chessOneByPerson(){
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->gameMapVec[clickPosRow][clickPosCol]==0){
        game->actionByPerson(clickPosRow,clickPosCol);
        qDebug()<<'('<<clickPosRow<<','<<clickPosCol<<')';
         QSound::play(CHESS_ONE_SOUND);
        update();
    }
}
void netBoard::mouseReleaseEvent(QMouseEvent*event){
//    if(selectPos==false){
//        return;
//    }else{
//        selectPos=false;
//    }
//    chessOneByPerson();

    //鼠标释放事件
    if(flag){

    if(!  game->playerFlag){//等待对方
        QMessageBox msgBox;
        msgBox.setText("请等待服务端连接网络或下棋！！");
        msgBox.exec();
        return;
    }

    if(  game->playerFlag)
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

void netBoard::on_pushButton_3_clicked()
{
    if(flag){//已经连接以后
        if( game->playerFlag){
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

void netBoard::on_pushButton_4_clicked()
{
    if(flag){//已经连接以后
        ui->textBrowser->insertPlainText("您已申请投降\n");
        initGame();


       QString a = "touxiang";
       QString str = a + "#";
        QByteArray arry;
        arry.append(str);
        mSocket->write(arry);


   }
}

void netBoard::on_pushButton_5_clicked()
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

void netBoard::on_pushButton_2_clicked()
{
    //发送消息开启计时
    if(flag){//已经连接以后
        if(!daojishi_flag){
        if(game->playerFlag){//如果是你的回合，要去发送信号，告诉对方你开启的，一起计时，假如不是你的回合直接计时
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
void netBoard::daojishi()
{
    // 初始化控件
    blackTimerId = new QTimer(this);
    whiteTimerId = new QTimer(this);

    // 初始化信号与槽
    connect(blackTimerId,&QTimer::timeout,this,&netBoard::BlackTimerUpdate,Qt::QueuedConnection);
    connect(whiteTimerId,&QTimer::timeout,this,&netBoard::WhiteTimerUpdate,Qt::QueuedConnection);
     connect(game,&GameModel::TurnToNextPlayer,this,&netBoard::SwitchTimer,Qt::QueuedConnection);
//   }
    // 初始化倒计时
    blackCountdown = COUNTDOWN;         // 初始化倒计时时间
    whiteCountdown = COUNTDOWN;

    ui->Black_timer->display("10:00");  // 初始化LCD显示
    ui->White_timer->display("10:00");
}
void netBoard::BlackTimerUpdate()
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

void netBoard::WhiteTimerUpdate()
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
void netBoard::SwitchTimer()
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

void netBoard::on_pushButton_6_clicked()
{
    deleteflag=false;
  QPixmap pixmap=  this->grab(QRect(50,50,650,650));

  QString strFile = QCoreApplication::applicationDirPath()+"avater.png";
    pixmap.save(strFile);

                   QFile* file = new QFile(strFile);
                    file->open(QIODevice::ReadOnly);

                    qDebug()<<file->size()<<"   "<<file->fileName();

                      QHttpMultiPart *mMultiPart= new QHttpMultiPart (QHttpMultiPart::FormDataType);

                      QHttpPart filePart;
                      filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));//这里我们上传的是图片
                      filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"avater\""));//这里的name默认情况下file，如果服务器那边有特别指定就需要改成服务器指定的名字
                      filePart.setBodyDevice(file);//这里的file对象就是我们需要提交的文件，如果需要上传多个文件，我们就需要创建多个QHttpPart对象

                      QHttpPart dataPart;
                      dataPart.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
                      dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"playerflag\""));
                      int flag=game->playerFlag;
                      QString str;

                      str = tr("%1").arg(flag);
                      QByteArray bytes = str.toUtf8();
                      dataPart.setBody( bytes );

                      //上面这段代码就是添加你需要携带的参数，比如用户名，或者token之类的，每一个参数对应一个QHttpPart对象
                      mMultiPart->append(filePart);
                      mMultiPart->append(dataPart);

                      QUrl url("http://127.0.0.1:8080/uploadchess");
                      QNetworkRequest request(url);
                      QNetworkReply* reply = accessManager.post(request, mMultiPart);



}

void netBoard::replyFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    QVariant nCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

     qDebug()<<nCode.toInt() << "   "<<reply->errorString();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        int s = bytes.size();
        qDebug()<<bytes.data();

    }
    else
    {
        // 错误处理-显示错误信息，或显示上一次缓存的图片或叉图。
    }

}
