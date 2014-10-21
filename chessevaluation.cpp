#include "chessevaluation.h"
#include "chessdefine.h"
#include <QString>
#include <QSound>
#include <QDebug>

//�������������������˱��ڲ�ͬλ�õĸ���ֵ
//�������ǹ���֮��Խ�����Ͻ���ֵԽ��
//����ĸ���ֵ����
const int R_PAWN_ADD[10][9]=
{
    {0,0,0,0,0,0,0,0,0},
    {120,120,140,150,150,150,140,120,120},
    {120,120,140,150,150,150,140,120,120},
    {100,120,140,140,140,140,140,120,100},
    {100,100,100,100,100,100,100,100,100},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
};

//����ĸ���ֵ����
const int B_PAWN_ADD[10][9]=
{
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {100,100,100,100,100,100,100,100,100},
    {100,120,140,140,140,140,140,120,100},
    {120,120,140,150,150,150,140,120,120},
    {120,120,140,150,150,150,140,120,120},
    {0,0,0,0,0,0,0,0,0},
};


ChessEvaluation::ChessEvaluation()
{
    //��ʼ����ֵ����
    this->baseValue[B_KING] = BASEVALUE_KING;
    this->baseValue[B_CAR] = BASEVALUE_CAR;
    this->baseValue[B_HORSE]= BASEVALUE_HORSE;
    this->baseValue[B_ELEPHANT] = BASEVALUE_ELEPHANT;
    this->baseValue[B_BISHOP] = BASEVALUE_BISHOP;
    this->baseValue[B_CANON] = BASEVALUE_CANON;
    this->baseValue[B_PAWN] = BASEVALUE_PAWN;
    this->baseValue[R_KING] = BASEVALUE_KING;
    this->baseValue[R_CAR] = BASEVALUE_CAR;
    this->baseValue[R_HORSE]= BASEVALUE_HORSE;
    this->baseValue[R_ELEPHANT] = BASEVALUE_ELEPHANT;
    this->baseValue[R_BISHOP] = BASEVALUE_BISHOP;
    this->baseValue[R_CANON] = BASEVALUE_CANON;
    this->baseValue[R_PAWN] = BASEVALUE_PAWN;

    //��ʼ�����ֵ������
    this->flexibilityValue[B_KING] = FLEXIBILITY_KING;
    this->flexibilityValue[B_CAR] = FLEXIBILITY_CAR;
    this->flexibilityValue[B_HORSE]= FLEXIBILITY_HORSE;
    this->flexibilityValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT;
    this->flexibilityValue[B_BISHOP] = FLEXIBILITY_BISHOP;
    this->flexibilityValue[B_CANON] = FLEXIBILITY_CANON;
    this->flexibilityValue[B_PAWN] = FLEXIBILITY_PAWN;
    this->flexibilityValue[R_KING] = FLEXIBILITY_KING;
    this->flexibilityValue[R_CAR] = FLEXIBILITY_CAR;
    this->flexibilityValue[R_HORSE]= FLEXIBILITY_HORSE;
    this->flexibilityValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT;
    this->flexibilityValue[R_BISHOP] = FLEXIBILITY_BISHOP;
    this->flexibilityValue[R_CANON] = FLEXIBILITY_CANON;
    this->flexibilityValue[R_PAWN] = FLEXIBILITY_PAWN;

    //��ʼ�����ʽڵ���
    this->accessCount = 0;
}

ChessEvaluation::~ChessEvaluation()
{

}

int ChessEvaluation::Eveluate(const int board[10][9],int turn)
{
    int fromType,toType,fromId,toId;
    int i,j,k;
    QPoint from;
    this->accessCount++;//ÿ����һ�ξ�����һ��
    //��ʼ��
    memset(this->chessValuePosition,0,360);
    memset(this->threadPosition,0,360);
    memset(this->guardPosition,0,360);
    memset(this->flexibilityPosition,0,360);

    //ɨ�����̣��ҳ�ÿһ�����ӣ�������в/���������ӣ������������
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromId = board[i][j];
                fromType = idToType(fromId);                  //ȡ��������

                from.setX(j);
                from.setY(i);
                getRelatePiece(board,from);                   //�ҳ��������������λ��

                for(k = 0;k < this->relatedPosCount;k++)      //��ÿһĿ��λ��
                {
                    toId = board[relatePos[k].y()][relatePos[k].x()];  //ȡĿ����������
                    toType = idToType(toId);
                    if(toType==NOCHESS)                         //����ǿհף����������
                        flexibilityPosition[i][j]++;
                    else                        //������
                    {
                        if(IsSameColor(fromType,toType))
                        {
                            //�������ӣ�Ŀ���ܱ���
                            guardPosition[relatePos[k].y()][relatePos[k].x()]++;
                        }
                        else
                        {
                            //�з����ӣ�Ŀ������в
                            threadPosition[relatePos[k].y()][relatePos[k].x()]++;
                            flexibilityPosition[i][j]++;           //���������
                            switch(toType)
                            {
                            case R_KING://��˧
                                if(!turn)  {                //�ֵ�������
                                    qDebug() << "hongshuai";
                                    QSound::play("./sounds/ILLEGAL.wav");
                                    return 18888;          //����ʧ�ܼ�ֵ
                                }

                                break;

                            case B_KING:                //�ڽ�
                                if(turn)     {           //�ֵ�������
                                    qDebug() << "heishuai";
                                    QSound::play("./sounds/ILLEGAL.wav");
                                    return 18888;       //����ʧ�ܼ�ֵ
                                }
                                break;

                            default:                  //������в�����Ӽ�����в��ֵ
                                threadPosition[relatePos[k].y()][relatePos[k].x()] +=
                                        (30 +(baseValue[toType]- baseValue[fromType]) / 10) / 10;
                                break;
                            }
                        }
                    }
                }
            }
        }

    //�����ѭ��ͳ��ɨ�赽������
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromType = idToType(board[i][j]);           //ȡ��������
                from.setX(j);
                from.setY(i);
                chessValuePosition[i][j]++;

                //������Ӵ������ֵ��Ϊ0����ÿһ���ӵ�����Լ�ֵ�ӽ����Ӽ�ֵ

                chessValuePosition[i][j] += flexibilityValue[fromType]
                                            * flexibilityPosition[i][j];
                //���ϱ��ĸ���ֵ
                chessValuePosition[i][j] += getPAWNValue(board,from);
            }
        }

    //�����ѭ������ͳ��ɨ�赽������,����ÿ�����ӵ��ܼ�ֵ
    int halfValue;
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromId = board[i][j];
                fromType = idToType(fromId);            //ȡ��������
                halfValue = baseValue[fromType] / 16;        //���ӻ�����ֵ��1/16��Ϊ��в/��������
                chessValuePosition[i][j] += baseValue[fromType]; //ÿ�����ӵĻ�����ֵ�������ܼ�ֵ
                if(IsRed(fromId))//����
                {
                    if(threadPosition[i][j])//��ǰ�����������в
                    {
                        if(turn)//�ֵ�������
                        {
                            if(fromType == R_KING)                   //����Ǻ콫
                                chessValuePosition[i][j] -= 20;       //��ֵ����20
                            else
                            {
                                //��ֵ��ȥ2��halfValue
                                chessValuePosition[i][j] -= halfValue * 2;
                                if(guardPosition[i][j])                        //�Ƿ񱻼������ӱ���
                                    chessValuePosition[i][j] += halfValue;         //�������ټ���nHalfvalue
                            }
                        }
                        else        //��ǰ���屻��в���ֵ�������
                        {
                            if(fromType == R_KING)          //�Ƿ��Ǻ�˧
                                return 18888;           //����ʧ�ܼ�ֵ
                            chessValuePosition[i][j] -= halfValue * 10;         //��ȥ10����nHalfvalue,��ʾ��в�̶ȸ�
                            if(guardPosition[i][j])         //���������
                                chessValuePosition[i][j] += halfValue * 9;      //�������ټ���9����nHalfvalue
                        }
                        //����в�����Ӽ�����в���ֹһ������вһ���������ĳ�������ֵ����û�з�ӳ��������
                        chessValuePosition[i][j] -= threadPosition[i][j];
                    }
                    else
                    {
                        //û����в
                        if(threadPosition[i][j])
                            chessValuePosition[i][j] += 5;            //�ܱ�������һ���
                    }
                }
                else
                {
                    //����Ǻ���
                    if(threadPosition[i][j])
                    {
                        //����в
                        if(!turn)
                        {
                            //�ֵ�������
                            if(fromType == B_KING)                      //����Ǻڽ�
                                chessValuePosition[i][j]-=20;                 //���Ӽ�ֵ����20
                            else
                            {
                                //���Ӽ�ֵ����2��halfvalue
                                chessValuePosition[i][j] -= halfValue * 2;
                                if(guardPosition[i][j])//����ܱ���
                                    chessValuePosition[i][j]  += halfValue;       //���Ӽ�ֵ����nHalfvalue
                            }
                        }
                        else
                        {
                            //�ֵ�������
                            if(fromType == B_KING)      //�Ǻڽ�
                                return 18888;       //����ʧ�ܼ�ֵ
                            chessValuePosition[i][j] -= halfValue * 10;         //���Ӽ�ֵ����10��nHalfvalue
                            if(guardPosition[i][j])//�ܱ���
                                chessValuePosition[i][j] += halfValue*9;        //�������ټ���9��nHalfvalue
                        }
                        //����в�������ټ�����в��,��ֹһ������вһ���������ĳ�,����ֵ����û�з�ӳ���������
                        chessValuePosition[i][j] -= threadPosition[i][j];
                    }
                    else
                    {
                        //������в
                        if(guardPosition[i][j])
                            chessValuePosition[i][j] += 5;              //�ܱ�������һ���
                    }
                }
            }
        }

    int redValue = 0;
    int blackValue = 0;
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            fromId = board[i][j];
            fromType = idToType(fromId);
            if(fromId != -1)
            {
                if(IsRed(fromId))
                    redValue += chessValuePosition[i][j];  //�Ѻ����ֵ����
                else
                    blackValue += chessValuePosition[i][j];//�Ѻ����ֵ����
            }
        }

    if(turn)
        return redValue-blackValue;         //����ֵ������߷��ع�ֵ
    else
        return blackValue-redValue;             //����ֵ������߷��ظ���ֵ



}


int ChessEvaluation::getRelatePiece(const int board[10][9],QPoint& from)
{
    this->relatedPosCount=0;
    int fromType;
    QPoint to;
    int i = from.y();
    int j = from.x();
    fromType = idToType(board[i][j]);           //ȡ��������

    switch(fromType)
    {
    case R_KING://��˧
    case B_KING://�ڽ�
        //ѭ�����Ź�֮����Щλ�ÿɵ���/������������˧�Ƿ���������
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++)
                if(canMoveToDst(board,from,to))         //�ܷ񵽴�
                        AddPoint(to.rx(),to.ry());      //�ɴﵽ/������λ�ü�������

        for(to.ry() = 7;to.ry() < 10;to.ry()++)
            for(to.rx() = 3;to.rx() < 6;to.rx()++)
                if(canMoveToDst(board,from,to))         //�ܷ񵽴�
                        AddPoint(to.rx(),to.ry());      //�ɴﵽ/������λ�ü�������

        break;

    case R_BISHOP://��ʿ
        //ѭ�����Ź�֮����Щλ�ÿɵ���/����
        for(to.ry() = 7;to.ry() < 10;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++)
                if(canMoveToDst(board,from,to))         //�ܷ񵽴�
                        AddPoint(to.rx(),to.ry());      //�ɴﵽ/������λ�ü�������

        break;

    case B_BISHOP://��ʿ
        //ѭ�����Ź�֮����Щλ�ÿɵ���/����
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx() = 3;to.rx() < 6;to.rx()++)
                if(canMoveToDst(board,from,to))         //�ܷ񵽴�
                        AddPoint(to.rx(),to.ry());      //�ɴﵽ/������λ�ü�������
        break;

    case R_ELEPHANT://����
    case B_ELEPHANT://����
        //����
        to.rx() = j+2;
        to.ry() = i+2;
        if(canMoveToDst(board,from,to))         //�ܷ񵽴�
                AddPoint(to.rx(),to.ry());      //�ɴﵽ/������λ�ü�������

        //����
        to.rx()=j+2;
        to.ry()=i-2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        //����
        to.rx()=j-2;
        to.ry()=i+2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        //����
        to.rx()=j-2;
        to.ry()=i-2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        break;

        case R_HORSE://����
        case B_HORSE://����
            //������·��ܷ񵽴�/����
            to.rx()=j+2;
            to.ry()=i+1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������Ϸ��ܷ񵽴�/����
            to.rx()=j+2;
            to.ry()=i-1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������·��ܷ񵽴�/����
            to.rx()=j-2;
            to.ry()=i+1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������Ϸ��ܷ񵽴�/����
            to.rx()=j-2;
            to.ry()=i-1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������·��ܷ񵽴�/����
            to.rx()=j+1;
            to.ry()=i+2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������Ϸ��ܷ񵽴�/����
            to.rx()=j+1;
            to.ry()=i-2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������·��ܷ񵽴�/����
            to.rx()=j-1;
            to.ry()=i+2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //������Ϸ��ܷ񵽴�/����
            to.rx()=j-1;
            to.ry()=i-2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            break;

        case R_CAR://�쳵
        case B_CAR://�ڳ�
            //��������ܷ񵽴�/����
            to.rx()=j+1;
            to.ry()=i;
            while(to.rx()<9)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()++;
            }

            //��������ܷ񵽴�/����
            to.rx()=j-1;
            to.ry()=i;
            while(to.rx()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()--;
            }

            //��������ܷ񵽴�/����
            to.rx()=j;
            to.ry()=i+1;
            while(to.ry()<10)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()++;
            }

            //��������ܷ񵽴�/����
            to.rx()=j;
            to.ry()=i-1;
            while(to.ry()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()--;
            }

            break;

        case R_PAWN://���
            //�ۿ���ǰ�Ƿ񵽵�
            to.ry()=i-1;
            to.rx()=j;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());
            if(i<5)
            {
                //���ѹ���
                to.ry()=i;
                to.rx()=j+1;//����
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()=j-1;//����
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
            }

            break;

        case B_PAWN://����
            //�ۿ���ǰ�Ƿ񵽵�
            to.ry()=i+1;
            to.rx()=j;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());
            if(i>4)
            {
                //���ѹ���
                to.ry()=i;
                to.rx()=j+1;//����
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()=j-1;//����
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
            }

            break;

        case B_CANON://����
        case R_CANON://����
            //��������ܷ񵽴�/������λ��
            to.rx()=j+1;
            to.ry()=i;
            while(to.rx()<9)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()++;//��������
            }

            //��������ܷ񵽴�/������λ��
            to.rx()=j-1;
            to.ry()=i;
            while(to.rx()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()--;//��������
            }

            //��������ܷ񵽴�/������λ��
            to.rx()=j;
            to.ry()=i+1;
            while(to.ry()<10)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()++;//��������
            }

            //��������ܷ񵽴�/������λ��
            to.rx()=j;
            to.ry()=i-1;
            while(to.ry()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()--;//��������
            }

            break;

        default:
            break;
        }
    return relatedPosCount;
}

int ChessEvaluation::getPAWNValue(const int board[10][9],QPoint& from)
{
    if(idToType(board[from.y()][from.x()]) == R_PAWN)
        return R_PAWN_ADD[from.y()][from.x()];
    else if(idToType(board[from.y()][from.x()]) == B_PAWN)
        return B_PAWN_ADD[from.y()][from.x()];
    return 0;
}

void ChessEvaluation::AddPoint(int x,int y)
{
    relatePos[relatedPosCount].setX(x);
    relatePos[relatedPosCount].setY(y);
    relatedPosCount++;
}


