#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QPoint>
#include <QVector>
#include<vector>
#include <QApplication>
//游戏类型 联机orAI对战
enum GameType{
    MAN,
    AI
};
//游戏状态
enum GameStatus{
    PLAYING,
    WIN,
    DEAD
};
//棋盘尺寸
const int BOARD_GRAD_SIZE=17;//人机
//const int PERSON_BOARD_GRAD_SIZE=15;//人人
const int MARGIN=30;//棋盘边缘空隙
const int CHESS_RADIUS=15;//棋子的半径
const int MARK_SIZE=6;//落子的时候标记的大小
const int BLOCK_SIZE=40;//格子大小
const int POS_OFFSET=BLOCK_SIZE*0.4;//棋盘的模糊距离
const int AI_THINK_TIME=700;//AI下棋的思考时间
class GameModel :public QObject{
      Q_OBJECT
public:
    GameModel(){

    };
public:
    //存储当前游戏棋盘和棋子的情况，空白为0，黑子1，白子-1
    std::vector<std::vector<int>>gameMapVec;
    //存储各个点位的评分情况，作为AI下棋依据
    std::vector<std::vector<int>>scoreMapVec;
    //标示下棋方，true:黑棋方 false：AI (白方)
    bool playerFlag;
    GameType gameFlag;//游戏模式是人机还是联机
    GameStatus gameStatus;//游戏状态
    void startGame(GameType type);//开始游戏
    void actionByPerson(int row,int col);
    void updateGameMap(int row,int col);
    bool isWin(int row,int col);
    void calculateScore(); //計算每格分數函數
    QVector<QPoint> chessList;//下棋序列数组用于悔棋
    int length;//下棋记录长度
    void backChess();//悔棋
signals:
    void TurnToNextPlayer();      // 切换棋手的信号
};
#endif // GAMEMODEL_H
