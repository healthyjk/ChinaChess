//类的公有变量和方法声明
#ifndef CHESSDEFINE_H
#define CHESSDEFINE_H

#include <QPoint>

//对战方定义
#define BLACKCHESS 0//黑方
#define REDCHESS   1//红方

//模式定义
#define MAN_PCCHESS   1//人机对弈
#define MAN_MANCHESS   2//人人对弈

//棋子定义
#define NOCHESS    0 //没有棋子
#define B_KING     1 //黑帅
#define B_CAR      2 //黑车
#define B_HORSE    3 //黑马
#define B_CANON    4 //黑炮
#define B_BISHOP   5 //黑士
#define B_ELEPHANT 6 //黑象
#define B_PAWN     7 //黑卒

#define R_KING     8 //红将
#define R_CAR      9 //红车
#define R_HORSE    10//红马
#define R_CANON    11//红炮
#define R_BISHOP   12//红士
#define R_ELEPHANT 13//红相
#define R_PAWN     14//红兵

#define R_KING_ID  27
#define B_KING_ID   4

#define CMAN_WIDTH 50       //棋子宽度
#define CMAN_HEIGHT 50      //棋子高度
#define BOARD_GAP   59.5       //棋盘间隔
#define BOARD_LEFT_MARGIN 263  //棋盘左边距
#define BOARD_TOP_MARGIN  70.7  //棋盘上边距

//数据类型定义
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;


//棋子判断
#define IsBlack(x) (x>=0 && x<=15)//判断某个棋子是不是黑色
#define IsRed(x)   (x>=16 && x<=31)//判断某个棋子是不是红色

//判断两个棋子是不是同色
#define IsSameColor(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))


//棋步
class ChessStep
{
public:
    int movedId;    //表明是什么棋子
    QPoint from;    //起始位置
    QPoint to;  //走到什么位置
    int score;  //走法的分数
};

class UnDoChessStep
{
public:
    ChessStep lastStep;
    int killedChessManId;
};

/********公用函数在此处声明***********/

//将棋子id转变为棋子类型
int idToType(int id);
//判断棋子能否移动
bool canMoveToDst(const int board[10][9],QPoint& from,QPoint& to);
//计算象眼的位置
int elephantEye(const int board[10][9],QPoint& from,QPoint& to);
//计算一条直线上棋子的数目
int countChessMan(const int board[10][9],QPoint& from,QPoint& to);

/*********************************/



#endif // CHESSDEFINE_H
