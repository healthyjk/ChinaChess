#ifndef CHESSMOVEGENERATE_H
#define CHESSMOVEGENERATE_H

#include "chessdefine.h"
#include <QtGui>

//得到每种棋子的走法
class ChessMoveGenerator
{
public:
    ChessMoveGenerator();
    virtual ~ChessMoveGenerator();

public:
    int createAllPossibleMove(const int board[10][9],int nPly,int turn); //产生所有可能的走法
protected:
    void generatePawnMove(const int board[10][9],QPoint &from,int nPly);   //产生兵卒的走法
    void generateBishopMove(const int board[10][9],QPoint &from,int nPly); //产生士的走法
    void generateCarMove(const int board[10][9],QPoint &from,int nPly);    //产生车的走法
    void generateHorseMove(const int board[10][9], QPoint &from,int nPly);  //产生马的走法
    void generateElephantMove(const int board[10][9],QPoint &from,int nPly);//产生相/象的走法
    void generateKingMove(const int board[10][9],QPoint &from,int nPly);    //产生将/帅的走法
    void generateCanonMove(const int board[10][9],QPoint &from,int nPly);   //产生炮的走法
    int  AddMove(QPoint from,QPoint to,int chessManId,int nPly);
public:
    ChessStep moveList[8][80];      //存放CreatePossibleMove产生的所有走法的队列
    int moveListCount;              //记录moveList中走法的数量
private:
    QPoint to;
};
#endif // CHESSMOVEGENERATE_H
