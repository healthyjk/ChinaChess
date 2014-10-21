#ifndef CHESSMOVEGENERATE_H
#define CHESSMOVEGENERATE_H

#include "chessdefine.h"
#include <QtGui>

//�õ�ÿ�����ӵ��߷�
class ChessMoveGenerator
{
public:
    ChessMoveGenerator();
    virtual ~ChessMoveGenerator();

public:
    int createAllPossibleMove(const int board[10][9],int nPly,int turn); //�������п��ܵ��߷�
protected:
    void generatePawnMove(const int board[10][9],QPoint &from,int nPly);   //����������߷�
    void generateBishopMove(const int board[10][9],QPoint &from,int nPly); //����ʿ���߷�
    void generateCarMove(const int board[10][9],QPoint &from,int nPly);    //���������߷�
    void generateHorseMove(const int board[10][9], QPoint &from,int nPly);  //��������߷�
    void generateElephantMove(const int board[10][9],QPoint &from,int nPly);//������/����߷�
    void generateKingMove(const int board[10][9],QPoint &from,int nPly);    //������/˧���߷�
    void generateCanonMove(const int board[10][9],QPoint &from,int nPly);   //�����ڵ��߷�
    int  AddMove(QPoint from,QPoint to,int chessManId,int nPly);
public:
    ChessStep moveList[8][80];      //���CreatePossibleMove�����������߷��Ķ���
    int moveListCount;              //��¼moveList���߷�������
private:
    QPoint to;
};
#endif // CHESSMOVEGENERATE_H
