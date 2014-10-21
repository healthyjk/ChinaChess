#include "chessmovegenerator.h"

ChessMoveGenerator::ChessMoveGenerator()
{
}

ChessMoveGenerator::~ChessMoveGenerator()
{
}

//得到所有可能的下一步
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
                    continue;//如要产生黑棋走法，跳过红棋

                if(turn && IsBlack(chessManId))
                    continue;//如要产生红棋走法，跳过黑棋
                from.setX(j);
                from.setY(i);
                switch(idToType(chessManId))
                {
                case R_KING://红帅
                case B_KING://黑将
                    generateKingMove(board,from,nPly);
                    break;
                case R_BISHOP://红士
                case B_BISHOP://黑士
                    generateBishopMove(board,from,nPly);
                    break;
                case R_ELEPHANT://红相
                case B_ELEPHANT://黑象
                    generateElephantMove(board,from,nPly);
                    break;
                case R_HORSE://红马
                case B_HORSE://黑马
                    generateHorseMove(board,from,nPly);
                    break;
                case R_CAR://红车
                case B_CAR://黑车
                    generateCarMove(board,from,nPly);
                    break;
                case R_PAWN://红兵
                case B_PAWN://黑卒
                    generatePawnMove(board,from,nPly);
                    break;
                case B_CANON://黑炮
                case R_CANON://红炮
                    generateCanonMove(board,from,nPly);
                    break;
                default:
                    break;
                }
            }
        }
    return this->moveListCount;
}

//搜索下一步将帅的所有棋步
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
                //这里要避免电脑主动走出将军会面的棋，因此将军会面的棋步不予考虑
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

//产生兵卒的走法
void ChessMoveGenerator::generatePawnMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    int chessManId=board[from.y()][from.x()];
    if(IsBlack(chessManId))         //如果是黑卒
    {
        to.ry()=i+1;//向前1步
        to.rx()=j;
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);//前方无阻碍

        if(i>4)//是否已过河
        {
            to.ry()=i;
            to.rx()=j+1;//右边
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
            to.rx()=j-1;//左边
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
        }
    }
    else                //红兵
    {
        to.ry()=i-1;      //向前
        to.rx()=j;
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);//前方无阻碍
        if(i<5)//是否已过河
        {
            to.ry()=i;
            to.rx()=j+1;//右边
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
            to.rx()=j-1;//左边
            if(canMoveToDst(board,from,this->to))
                AddMove(from,this->to,board[from.y()][from.x()],nPly);
        }
    }
}

//产生士的走法
void ChessMoveGenerator::generateBishopMove(const int board[10][9],QPoint &from,int nPly)
{
    int chessManId=board[from.y()][from.x()];
    if(IsBlack(chessManId))         //如果是黑卒
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

//产生车的走法
void ChessMoveGenerator::generateCarMove(const int board[10][9], QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //插入向右的有效的走法
    to.rx()=j+1;
    to.ry()=i;
    while(to.rx()<9)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()++;
    }
    //插入向左的有效的走法
    to.rx()=j-1;
    to.ry()=i;
    while(to.rx()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()--;
    }
    //插入向下的有效的走法
    to.rx()=j;
    to.ry()=i+1;
    while(to.ry()<10)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()++;
    }
    //插入向上的有效的走法
    to.rx()=j;
    to.ry()=i-1;
    while(to.ry()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()--;
    }
}

//产生马的走法
void ChessMoveGenerator::generateHorseMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //插入右下方的有效走法
    to.rx()=j+2;//右2
    to.ry()=i+1;//下1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入右上方的有效走法
    to.rx()=j+2;//右2
    to.ry()=i-1;//上1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左下方的有效走法
    to.rx()=j-2;//左2
    to.ry()=i+1;//下1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左上方的有效走法
    to.rx()=j-2;//左2
    to.ry()=i-1;//上1
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入右下方的有效走法
    to.rx()=j+1;//右1
    to.ry()=i+2;//下2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左下方的有效走法
    to.rx()=j-1;//左1
    to.ry()=i+2;//下2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入右上方的有效走法
    to.rx()=j+1;//右1
    to.ry()=i-2;//上2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左上方的有效走法
    to.rx()=j-1;//左1
    to.ry()=i-2;//上2
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);
}

//产生相/象的走法
void ChessMoveGenerator::generateElephantMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    //插入右下方的有效走法
    to.rx()=j+2;
    to.ry()=i+2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入右上方的有效走法
    to.rx()=j+2;
    to.ry()=i-2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左下方的有效走法
    to.rx()=j-2;
    to.ry()=i+2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);

    //插入左上方的有效走法
    to.rx()=j-2;
    to.ry()=i-2;
    if(canMoveToDst(board,from,this->to))
        AddMove(from,this->to,board[from.y()][from.x()],nPly);
}

//产生炮的走法
void ChessMoveGenerator::generateCanonMove(const int board[10][9],QPoint &from,int nPly)
{
    int i = from.y();
    int j = from.x();
    int chessManId;
    chessManId=board[from.y()][from.x()];
    //插入向右的有效的走法
    to.rx()=j+1;
    to.ry()=i;
    while(to.rx()<9)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()++;//继续下一个位置
    }

    //插入向左的有效的走法
    to.rx()=j-1;
    to.ry()=i;
    while(to.rx()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.rx()--; //继续下一个位置
    }

    //插入向下的有效的走法
    to.rx()=j;
    to.ry()=i+1;
    while(to.ry()<10)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()++;//继续下一个位置
    }

    //插入向上的有效的走法
    to.rx()=j;
    to.ry()=i-1;
    while(to.ry()>=0)
    {
        if(canMoveToDst(board,from,this->to))
            AddMove(from,this->to,board[from.y()][from.x()],nPly);
        to.ry()--;//继续下一个位置
    }
}

//记录可能步法
int  ChessMoveGenerator::AddMove(QPoint from,QPoint to,int chessManId,int nPly)
{
    moveList[nPly][moveListCount].from = from;
    moveList[nPly][moveListCount].to = to;
    moveList[nPly][moveListCount].movedId = chessManId;
    moveListCount++;
    return moveListCount;
}

