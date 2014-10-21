//��Ĺ��б����ͷ�������
#ifndef CHESSDEFINE_H
#define CHESSDEFINE_H

#include <QPoint>

//��ս������
#define BLACKCHESS 0//�ڷ�
#define REDCHESS   1//�췽

//ģʽ����
#define MAN_PCCHESS   1//�˻�����
#define MAN_MANCHESS   2//���˶���

//���Ӷ���
#define NOCHESS    0 //û������
#define B_KING     1 //��˧
#define B_CAR      2 //�ڳ�
#define B_HORSE    3 //����
#define B_CANON    4 //����
#define B_BISHOP   5 //��ʿ
#define B_ELEPHANT 6 //����
#define B_PAWN     7 //����

#define R_KING     8 //�콫
#define R_CAR      9 //�쳵
#define R_HORSE    10//����
#define R_CANON    11//����
#define R_BISHOP   12//��ʿ
#define R_ELEPHANT 13//����
#define R_PAWN     14//���

#define R_KING_ID  27
#define B_KING_ID   4

#define CMAN_WIDTH 50       //���ӿ��
#define CMAN_HEIGHT 50      //���Ӹ߶�
#define BOARD_GAP   59.5       //���̼��
#define BOARD_LEFT_MARGIN 263  //������߾�
#define BOARD_TOP_MARGIN  70.7  //�����ϱ߾�

//�������Ͷ���
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;


//�����ж�
#define IsBlack(x) (x>=0 && x<=15)//�ж�ĳ�������ǲ��Ǻ�ɫ
#define IsRed(x)   (x>=16 && x<=31)//�ж�ĳ�������ǲ��Ǻ�ɫ

//�ж����������ǲ���ͬɫ
#define IsSameColor(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))


//�岽
class ChessStep
{
public:
    int movedId;    //������ʲô����
    QPoint from;    //��ʼλ��
    QPoint to;  //�ߵ�ʲôλ��
    int score;  //�߷��ķ���
};

class UnDoChessStep
{
public:
    ChessStep lastStep;
    int killedChessManId;
};

/********���ú����ڴ˴�����***********/

//������idת��Ϊ��������
int idToType(int id);
//�ж������ܷ��ƶ�
bool canMoveToDst(const int board[10][9],QPoint& from,QPoint& to);
//�������۵�λ��
int elephantEye(const int board[10][9],QPoint& from,QPoint& to);
//����һ��ֱ�������ӵ���Ŀ
int countChessMan(const int board[10][9],QPoint& from,QPoint& to);

/*********************************/



#endif // CHESSDEFINE_H
