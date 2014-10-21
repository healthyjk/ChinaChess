#include "chessmovegenerator.h"

ChessMoveGenerator::ChessMoveGenerator()
{
}

ChessMoveGenerator::~ChessMoveGenerator()
{
}

//�õ����п��ܵ���һ��
int ChessMoveGenerator::createAllPossibleMove(const int board[10][9],int nPly,int turn)
{
    int chessManId;
    int i,j;

    this->moveListCount=0;
    QPoint from;

    for(j=0;j<9;j++)
        for(i=0;i<10;i++)
        {
            if(board[i][j]!= -1)
            {
                chessManId=board[i][j];
                if(!turn && IsRed(chessManId))
                    continue;//��Ҫ���������߷�����������

                if(turn && IsBlack(chessManId))
                    continue;//��Ҫ���������߷�����������
                from.setX(j);
                from.setY(i);
                switch(idToType(chessManId))
                {
                case R_KING://��˧
                case B_KING://�ڽ�
                    generateKingMove(board,from,nPly);
                    break;
                case R_BISHOP://��ʿ
                case B_BISHOP://��ʿ
                    generateBishopMove(board,from,nPly);
                    break;
                case R_ELEPHANT://����
                case B_ELEPHANT://����
                    generateElephantMove(board,from,nPly);
                    break;
                case R_HORSE://����
                case B_HORSE://����
                    generateHorseMove(board,from,nPly);
                    break;
                case R_CAR://�쳵
                case B_CAR://�ڳ�
                    generateCarMove(board,from,nPly);
                    break;
                case R_PAWN://���
                case B_PAWN://����
                    generatePawnMove(board,from,nPly);
                    break;
                case B_CANON://����
                case R_CANON://����
                    generateCanonMove(board,from,nPly);
                    break;
                default:
                    break;
                }
            }
        }
    return this->moveListCount;
}

//������һ����˧�������岽
void ChessMoveGenerator::generateKingMove(const int board[10][9],QPoint& from,int nPly)
{
    QPoint R_KING_POS,B_KING_POS;
    int chessManId = board[from.y()][from.x()];
    for(int i = 0;i < 10;i++)
        for(int j = 3;j < 6;j++)
            if(board[i][j] == B_KING_ID){
                B_KING_POS.setX(j);
                B_KING_POS.setY(i);
            }
            else if(board[i][j] == R_KING_ID){
                R_KING_POS.setX(j);
                R_KING_POS.setY(i);
            }
    if(IsBlack(chessManId)){
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++){
                //����Ҫ������������߳�����������壬��˽���������岽���迼��
                if(canMoveToDst(board,from,this->to) && (R_KING_POS.x() != B_KING_POS.x() ||
                                                         countChessMan(board,R_KING_POS,B_KING_POS) != 0))
                    AddMove(from,this->to,board[from.y()][from.x()],nPly);
            }
    }
    else{
        for(to.ry()=7;to.ry()<10;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++){
                if(canMoveToDst(board,from,this->to))
                    AddMove(from,this->to,board[from.y()][from.x()],nPly);
            }
    }
}

//����������߷�
void ChessMoveGenerator::generatePawnMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    int chessManId=board[from.y()][from.x()];
    if(IsBlack(chessManId))         //����Ǻ���
    {
        to.ry()=i+1;//��ǰ1��
        to.rx()=j;
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);//ǰ�����谭

        if(i>4)//�Ƿ��ѹ���
        {
            to.ry()=i;
            to.rx()=j+1;//�ұ�
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
            to.rx()=j-1;//���
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
        }
    }
    else                //���
    {
        to.ry()=i-1;      //��ǰ
        to.rx()=j;
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);//ǰ�����谭
        if(i<5)//�Ƿ��ѹ���
        {
            to.ry()=i;
            to.rx()=j+1;//�ұ�
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
            to.rx()=j-1;//���
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
        }
    }
}

//����ʿ���߷�
void ChessMoveGenerator::generateBishopMove(const int board[10][9],QPoint &from,int nPly)
{
    int chessManId=board[from.y()][from.x()];
    if(IsBlack(chessManId))         //����Ǻ���
    {
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++){
                if(canMoveToDst(board,from,this->to))
                    AddMove(from,this->to,board[from.y()][from.x()],nPly);
            }
    }
    else
    {
        for(to.ry()=7;to.ry()<10;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++){
                if(canMoveToDst(board,from,this->to))
                    AddMove(from,this->to,board[from.y()][from.x()],nPly);
            }
    }
}

//���������߷�
void ChessMoveGenerator::generateCarMove(const int board[10][9], QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //�������ҵ���Ч���߷�
    to.rx()=j+1;
    to.ry()=i;
    while(to.rx()<9)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()++;
    }
    //�����������Ч���߷�
    to.rx()=j-1;
    to.ry()=i;
    while(to.rx()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()--;
    }
    //�������µ���Ч���߷�
    to.rx()=j;
    to.ry()=i+1;
    while(to.ry()<10)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()++;
    }
    //�������ϵ���Ч���߷�
    to.rx()=j;
    to.ry()=i-1;
    while(to.ry()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()--;
    }
}

//��������߷�
void ChessMoveGenerator::generateHorseMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //�������·�����Ч�߷�
    to.rx()=j+2;//��2
    to.ry()=i+1;//��1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j+2;//��2
    to.ry()=i-1;//��1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������·�����Ч�߷�
    to.rx()=j-2;//��2
    to.ry()=i+1;//��1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j-2;//��2
    to.ry()=i-1;//��1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������·�����Ч�߷�
    to.rx()=j+1;//��1
    to.ry()=i+2;//��2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������·�����Ч�߷�
    to.rx()=j-1;//��1
    to.ry()=i+2;//��2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j+1;//��1
    to.ry()=i-2;//��2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j-1;//��1
    to.ry()=i-2;//��2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);
}

//������/����߷�
void ChessMoveGenerator::generateElephantMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //�������·�����Ч�߷�
    to.rx()=j+2;
    to.ry()=i+2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j+2;
    to.ry()=i-2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������·�����Ч�߷�
    to.rx()=j-2;
    to.ry()=i+2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //�������Ϸ�����Ч�߷�
    to.rx()=j-2;
    to.ry()=i-2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);
}

//�����ڵ��߷�
void ChessMoveGenerator::generateCanonMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    int chessManId;
    chessManId=board[from.y()][from.x()];
    //�������ҵ���Ч���߷�
    to.rx()=j+1;
    to.ry()=i;
    while(to.rx()<9)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()++;//������һ��λ��
    }

    //�����������Ч���߷�
    to.rx()=j-1;
    to.ry()=i;
    while(to.rx()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()--; //������һ��λ��
    }

    //�������µ���Ч���߷�
    to.rx()=j;
    to.ry()=i+1;
    while(to.ry()<10)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()++;//������һ��λ��
    }

    //�������ϵ���Ч���߷�
    to.rx()=j;
    to.ry()=i-1;
    while(to.ry()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()--;//������һ��λ��
    }
}

//��¼���ܲ���
int  ChessMoveGenerator::AddMove(QPoint from,QPoint to,int chessManId,int nPly)
{
    moveList[nPly][moveListCount].from = from;
    moveList[nPly][moveListCount].to = to;
    moveList[nPly][moveListCount].movedId = chessManId;
    moveListCount++;
    return moveListCount;
}

