#ifndef CHESSEVALUATION_H
#define CHESSEVALUATION_H
#include <QPoint>
#include "chessdefine.h"

//����ÿ�����Ӽ�ֵ
//��100��ʿ250����250����500����350����350���������
#define BASEVALUE_PAWN       100
#define BASEVALUE_BISHOP     250
#define BASEVALUE_ELEPHANT   250
#define BASEVALUE_HORSE      700
#define BASEVALUE_CANON      700
#define BASEVALUE_CAR        1400
#define BASEVALUE_KING       10000

//������������ԣ�Ҳ����ÿ��һ������λ��Ӧ�ӵķ�ֵ
//��15��ʿ1����1����6����12����6����0
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

    virtual int Eveluate(const int board[10][9],int turn);//��ֵ�������Դ�������̴�֣�turn��ʾ�ֵ�˭����(0��1��)
    int getAccessCount(){return accessCount;}//�õ����ʽڵ���
    void clearAccessCount(){accessCount=0;}  //���÷��ʽڵ���Ϊ0
    int getRelatePiece(const int board[10][9],QPoint& from);               //�ҳ��������������λ��
    int getPAWNValue(const int board[10][9],QPoint& from);                  //�õ�����ĸ��ӷ���
    void AddPoint(int x,int y);                                     //����������λ����Ϣ

protected:
    int baseValue[15];                 //�ֱ���ÿһ�����ӻ�����ֵ
    int flexibilityValue[15];          //�ֱ���ÿһ����������Է�ֵ
    int threadPosition[10][9];         //���ÿһλ�ñ���в����Ϣ
    int guardPosition[10][9];          //���ÿһλ�ñ���������Ϣ
    int flexibilityPosition[10][9];    //���ÿһλ�������ӵ�����Է�ֵ
    int chessValuePosition[10][9];     //���ÿһλ�������ӵ��ܼ�ֵ
    int relatedPosCount;               //��¼һ���ӵ����λ�ø���
    QPoint relatePos[20];              //��¼һ���ӵ����λ��
    long accessCount;                  //��ŷ��ʽڵ���
};

#endif // CHESSEVALUATION_H
