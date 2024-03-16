#include"GameModel.h"
#include <time.h>
#include <stdlib.h>
#include <QMessageBox>
#include<QDebug>
#include <QHttpMultiPart>
#include <QNetworkReply>


void GameModel::startGame(GameType type){

    GameType gameType = type;
    length = 0;
    //初始化棋盘
    gameMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        std::vector<int> lineBoard;
        for(int j=0;j<BOARD_GRAD_SIZE;j++){
            lineBoard.push_back(0);
        }
        gameMapVec.push_back(lineBoard);
    }

    //如果是AI模式,需要评分数组
    if(gameType == AI){
        scoreMapVec.clear();
        for(int i=0;i<BOARD_GRAD_SIZE;i++){
            std::vector<int> lineScores;
            for(int j=0;j<BOARD_GRAD_SIZE;j++){
                lineScores.push_back(0);
            }
            scoreMapVec.push_back(lineScores);
        }
    }
    playerFlag = true;
}
void GameModel::updateGameMap(int row,int col){
    emit(TurnToNextPlayer());
    chessList.push_back(QPoint(row,col));//记录下子的序列
    ++length;
    if(playerFlag)
        gameMapVec[row][col]=1;
    else
        gameMapVec[row][col]=-1;
    playerFlag=!playerFlag;
}
void GameModel::actionByPerson(int row,int col)
{
     QHttpMultiPart *mMultiPart= new QHttpMultiPart (QHttpMultiPart::FormDataType);
                       QHttpPart dataPart1;
                        dataPart1.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
                        dataPart1.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"color\""));
                        int color=playerFlag;
                        QString str;

                        str = tr("%1").arg(color);
                        QByteArray bytes = str.toUtf8();
                        dataPart1.setBody( bytes );

                        QHttpPart dataPart2;
                         dataPart2.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
                         dataPart2.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"rowx\""));
                         int rowx=row;
                         QString str1;

                         str1 = tr("%1").arg(rowx);
                         QByteArray bytes1 = str1.toUtf8();
                         dataPart2.setBody( bytes1 );

                         QHttpPart dataPart3;
                          dataPart3.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
                          dataPart3.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"colx\""));
                          int colx=col;
                          QString str2;

                          str2 = tr("%1").arg(colx);
                          QByteArray bytes2 = str2.toUtf8();
                          dataPart3.setBody( bytes2 );


//                        //上面这段代码就是添加你需要携带的参数，比如用户名，或者token之类的，每一个参数对应一个QHttpPart对象
                        mMultiPart->append(dataPart1);
                          mMultiPart->append(dataPart2);
                            mMultiPart->append(dataPart3);

                        QUrl url("http://127.0.0.1:8080/putdown");
                        QNetworkRequest request(url);
                        QNetworkReply* reply = accessManager1.post(request, mMultiPart);
    updateGameMap(row,col);
}
bool GameModel::isWin(int row,int col){
    for(int i=0;i<5;i++){
        //水平方向判断
        if(row>0 && row<BOARD_GRAD_SIZE &&
           col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+1] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+2] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+3] &&
           gameMapVec[row][col-i] == gameMapVec[row][col-i+4]){
            return true;
        }
        //垂直方向判断
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col>0 && col<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col] == gameMapVec[row-i+1][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+2][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+3][col] &&
           gameMapVec[row-i][col] == gameMapVec[row-i+4][col]){
            return true;
        }
        //左下->右上
        if(
           row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col+i-4>0 && col+i<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+1][col+i-1] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+2][col+i-2] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+3][col+i-3] &&
           gameMapVec[row-i][col+i] == gameMapVec[row-i+4][col+i-4]){
            return true;
        }
        //右下->左上
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
           col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+1][col-i+1] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+2][col-i+2] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+3][col-i+3] &&
           gameMapVec[row-i][col-i] == gameMapVec[row-i+4][col-i+4]){
            return true;
        }

    }
    return false;
}
//计分功能
void GameModel::calculateScore(){

    int personNum = 0; //玩家连子个数
    int botNum = 0;   //AI生成子个数
    int emptyNum = 0;   //各方向空白位的个数

    //清空评分数組
    scoreMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++){
        std::vector<int> lineScores;
        for(int j=0;j<BOARD_GRAD_SIZE;j++){
            lineScores.push_back(0);
        }
        scoreMapVec.push_back(lineScores);
    }
    //计分
    /*计分理解：
     * 遍历每一个棋子，判断哪些是空白的点(即为0的点)，以该点为中心，判断周围8个方向点向外延伸的四格里，
     * 有多少个是黑子、白子、空白，以此作为依据来评分。下方算法是以守为主，所以守的分数>攻的分数
     */
    for(int row=0;row<BOARD_GRAD_SIZE;row++){
        for(int col=0;col<BOARD_GRAD_SIZE;col++){
            //空白点才算
            if(row>0 && col>0 && gameMapVec[row][col]==0){
                //遍历周围8个方向
                for(int y=-1;y<=1;y++){
                    for(int x=-1;x<=1;x++){
                        //重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //原坐标不算
                        if(!(y==0 && x==0)){
                            //每个方向延伸4个子

                            //对玩家黑子评分(正反两个方向)
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==1){ //真人玩家的子
                                    personNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出边界，或有白子
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==1){ //真人玩家的子
                                    personNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出边界，或有白子
                                    break;
                                }
                            }
                            if(personNum == 1){                 //杀2
                                scoreMapVec[row][col]+=10;
                            }else if(personNum == 2){           //杀3
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=30;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=40;
                            }else if(personNum == 3){           //杀4
                                //量变空位不一样，优先級不一样
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=60;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=110;
                            }else if(personNum == 4){           //杀5
                                scoreMapVec[row][col]+=10100;
                            }

                            //进行一次清空
                            emptyNum = 0;

                            //对AI白子评分
                            for(int i=1;i<=4;i++){
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                   col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row+i*y][col+i*x]==-1){ //AI的子
                                    botNum++;
                                }else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                         col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row+i*y][col+i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出边界
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++){
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                   col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                   gameMapVec[row-i*y][col-i*x]==-1){ //AI的子
                                    botNum++;
                                }else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                         col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                         gameMapVec[row-i*y][col-i*x]==0){ //空白位
                                    emptyNum++;
                                    break;
                                }else{ //出边界
                                    break;
                                }
                            }
                            if(botNum == 0){
                                scoreMapVec[row][col]+=5;  //活1
                            }else if(botNum == 1){
                                scoreMapVec[row][col]+=10; //活2
                            }else if(botNum == 2){         //活3
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=25;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                            }else if(botNum == 3){         //活4
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=55;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=100;
                            }else if(botNum >= 4){         //活5
                                scoreMapVec[row][col]+=20000;
                            }


                        }
                    }
                }
            }
        }
    }

}
//AI执行下棋
void GameModel::actionByAI(int &clickRow,int &clickCol){
    //计算评分
    calculateScore();

    //从评分表中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int,int>> maxPoints;
    for(int row = 1;row<BOARD_GRAD_SIZE;row++){
        for(int col = 1;col<BOARD_GRAD_SIZE;col++){
            //前提是這個坐标是空的
            if(gameMapVec[row][col] == 0){
                if(scoreMapVec[row][col]>maxScore){     //找最大数和坐标
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row,col));
                }else if(scoreMapVec[row][col] == maxScore){   //如果有多个最大值就將他们存起來，在后面的代码随机抽1个
                    maxPoints.push_back(std::make_pair(row,col));
                }
            }
        }
    }
    //随机落子，如果有多个点的话
    srand((unsigned)time(NULL));
    int index = rand()%maxPoints.size();
    std::pair<int,int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first;
    clickCol = pointPair.second;
    updateGameMap(clickRow,clickCol);
}


void GameModel::backChess()//悔棋
{

      if(chessList.size()==0){

QMessageBox::warning(0 , "悔棋" , "当前无棋可悔，请先下棋", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0 );
       return;

      } else if(chessList.size()>=2){
      int x,y;
                         x=chessList.last().x();
                  y=chessList.last().y();
                     chessList.pop_back();
                       gameMapVec[x][y]=0;
                     length--;
                     x=chessList.last().x();
                     y=chessList.last().y();
                        chessList.pop_back();
                          gameMapVec[x][y]=0;
                        length--;
}
}
