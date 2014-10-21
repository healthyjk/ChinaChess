#ifndef CHESSEVALUATION_H
#define CHESSEVALUATION_H
#include <QPoint>
#include "chessdefine.h"

//定义每种棋子价值
//兵100，士250，象250，车500，马350，炮350，王无穷大
#define BASEVALUE_PAWN       100
#define BASEVALUE_BISHOP     250
#define BASEVALUE_ELEPHANT   250
#define BASEVALUE_HORSE      700
#define BASEVALUE_CANON      700
#define BASEVALUE_CAR        1400
#define BASEVALUE_KING       10000

//定义棋子灵活性，也就是每多一个可走位置应加的分值
//兵15，士1，象1，车6，马12，炮6，王0
#define FLEXIBILITY_PAWN     15
#define FLEXIBILITY_BISHOP   1
#define FLEXIBILITY_ELEPHANT 1
#define FLEXIBILITY_CAR      6
#define FLEXIBILITY_HORSE    12
#define FLEXIBILITY_CANON    6
#define FLEXIBILITY_KING     0

class ChessEvaluation
{
public:
    ChessEvaluation();
    virtual ~ChessEvaluation();

    virtual int Eveluate(const int board[10][9],int turn);//估值函数，对传入的棋盘打分，turn表示轮到谁走棋(0黑1红)
    int getAccessCount(){return accessCount;}//得到访问节点数
    void clearAccessCount(){accessCount=0;}  //重置访问节点数为0
    int getRelatePiece(const int board[10][9],QPoint& from);               //找出该棋子所有相关位置
    int getPAWNValue(const int board[10][9],QPoint& from);                  //得到兵卒的附加分数
    void AddPoint(int x,int y);                                     //添加棋子相关位置信息

protected:
    int baseValue[15];                 //分别存放每一颗棋子基本价值
    int flexibilityValue[15];          //分别存放每一颗棋子灵活性分值
    int threadPosition[10][9];         //存放每一位置被威胁的信息
    int guardPosition[10][9];          //存放每一位置被保护的信息
    int flexibilityPosition[10][9];    //存放每一位置上棋子的灵活性分值
    int chessValuePosition[10][9];     //存放每一位置上棋子的总价值
    int relatedPosCount;               //记录一棋子的相关位置个数
    QPoint relatePos[20];              //记录一棋子的相关位置
    long accessCount;                  //存放访问节点数
};

#endif // CHESSEVALUATION_H
