#include "chessdefine.h"
#include <stdlib.h>

//����idתType����
int idToType(int id)
{
    switch(id)
    {
    case 0:
    case 8:return B_CAR;break;
    case 1:
    case 7:return B_HORSE;break;
    case 2:
    case 6:return B_ELEPHANT;break;
    case 3:
    case 5:return B_BISHOP;break;
    case 4:return B_KING;break;
    case 9:
    case 10:return B_CANON;break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:return B_PAWN;break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:return R_PAWN;break;
    case 21:
    case 22:return R_CANON;break;
    case 23:
    case 31:return R_CAR;break;
    case 24:
    case 30:return R_HORSE;break;
    case 25:
    case 29:return R_ELEPHANT;break;
    case 26:
    case 28:return R_BISHOP;break;
    case 27:return R_KING;break;
    default:break;
    }
    return NOCHESS;
}


//����һ��ֱ�������ӵ���Ŀ
int countChessMan(const int board[10][9],QPoint& from,QPoint& to)
{
    int num = 0;
    if(from.x() != to.x() && from.y() != to.y() )
        return -1;
    int small,large;
    if(from.x() == to.x())
    {
        small = from.y() < to.y()? from.y() : to.y();
        large = from.y() + to.y() - small;
        for(int i = small + 1;i < large;i++)
            if(board[i][to.x()] != -1)
                num++;
    }
    else
    {
        small = from.x() < to.x()? from.x() : to.x();
        large = from.x() + to.x() - small;
        for(int i = small + 1;i < large;i++)
            if(board[to.y()][i] != -1)
                num++;
    }
    return num;
}

//�������۵�λ��
int elephantEye(const int board[10][9],QPoint& from,QPoint& to)
{
    QPoint pt;
    pt.setX((from.x() + to.x())/2);
    pt.setY((from.y() + to.y())/2);
    return board[pt.y()][pt.x()];
}



//�ж������ܷ��ƶ�
bool canMoveToDst(const int board[10][9],QPoint& from,QPoint& to)
{
    if(from == to)
        return false;
    if(to.x() < 0 || to.x() > 8 || to.y() < 0 || to.y() > 9)
        return false;
    int fromId = board[from.y()][from.x()];
    int toId = board[to.y()][to.x()];
    if(fromId / 16 == toId / 16 && toId != -1)      //ͬɫ����
        return false;
    int count,elephantEyeId;
    QPoint horseFoot;                   //��ŵ�λ��

    count = abs(from.x() - to.x()) * 10
            + abs(from.y() - to.y());

    switch(idToType(fromId) % 7)
    {
    case 0:                 //��������
        if(count != 10 && count != 1)
            return false;
        if(IsRed(fromId))        //����Ǻ���
        {
            if(to.y() > from.y())  //���ܵ���
                return false;
            if(from.y() > 4 && from.y() == to.y()) //���ԼҲ��ܺ�����
                return false;
        }
        else                                       //����Ǻ���
        {
            if(to.y() < from.y())
                return false;
            if(from.y() < 5 && from.y() == to.y())
                return false;
        }
        return true;
        break;
    case 1:                 //������˧

        if(IsRed(fromId) && toId == B_KING_ID
                && (countChessMan(board,from,to) == 0))
            return true;

        if(IsBlack(fromId) && toId == R_KING_ID
                && (countChessMan(board,from,to) == 0))
            return true;

        if(count != 10 && count != 1)
            return false;

        if(to.x() < 3 || to.x() > 5)
            return false;

        if(to.y() > 2 && IsBlack(fromId))
            return false;

        if(to.y() < 7 && IsRed(fromId))
            return false;

        return true;
        break;
    case 2:                 //��
        if(countChessMan(board,from,to) == 0)
        {
            return true;
        }
        return false;
        break;
    case 3:                 //��
        if(count != 21 && count != 12)
            return false;
        if(count == 21)                     //���������ŵ�λ��
        {
            horseFoot.setX((from.x() + to.x()) / 2);
            horseFoot.setY(from.y());
        }
        else
        {
            horseFoot.setY((from.y() + to.y()) / 2);
            horseFoot.setX(from.x());
        }
        if(board[horseFoot.y()][horseFoot.x()] != -1)
            return false;
        return true;
        break;
    case 4:                 //��
        if(toId == -1)             //Ŀ�ĵ�Ϊ��
        {
            if(countChessMan(board,from,to) == 0)
            {
                return true;
            }
            return false;
        }
        else                        //Ŀ�ĵ�Ϊ����:���Ӵ���
        {
            if(countChessMan(board,from,to) == 1)
            {
                return true;
            }
            return false;
        }
        break;
    case 5:                 //ʿ������
        if(count != 11)
            return false;
        if(to.x() < 3 || to.x() > 5)
            return false;
        if(to.y() > 2 && IsBlack(fromId))
            return false;
        if(to.y() < 7 && IsRed(fromId))
            return false;
        return true;
        break;
    case 6:                 //�������
        if(count != 22)
            return false;
        elephantEyeId = elephantEye(board,from,to);
        if(elephantEyeId != -1)                                   //�������û����ס
            return false;
        if(to.y() > 4 && IsRed(fromId))        //���ܹ���
            return true;
        if(to.y() < 5 && IsBlack(fromId))
            return true;
        return false;
        break;
    default:
        return false;
    }
}
