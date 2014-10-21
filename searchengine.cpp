#include "searchengine.h"

SearchEngine::SearchEngine()
{
}

SearchEngine::~SearchEngine()
{
    delete chessMoveGenerator;
    delete chessEvaluation;
}

//void setThinkProgress(CProgress* p){progress = p;}

//判断是否已分胜负
int SearchEngine::isGameOver(const int board[10][9],int depth)
{
    int i,j,chessManType;
    bool redAlive = false;
    bool blackAlive = false;

    //检查红方九宫是否有帅
    for(i=7;i<10;i++)
        for(j=3;j<6;j++)
        {
            chessManType = idToType(board[i][j]);
            if(chessManType == B_KING)
                blackAlive = true;
            if(chessManType == R_KING)
                redAlive = true;
        }

    //检查黑方九宫是否有将
    for(i=0;i<3;i++)
        for(j=3;j<6;j++)
        {
            chessManType = idToType(board[i][j]);
            if(chessManType == B_KING)
                blackAlive = true;
            if(chessManType == R_KING)
                redAlive = true;
        }
    QPoint R_KING_POS,B_KING_POS;
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


    i = (this->maxDepth - depth + 1) % 2;//取当前奇偶标志,奇数层为电脑方,偶数层为用户方
    //红方不在

    if(!redAlive){
        if(i)
            return 19990 + depth; //奇数层返回极大值
        else
            return -19990 - depth;//偶数层返回极小值
    }

    //黑方不在
    if(!blackAlive){
        if(i)
            return -19990 - depth;//奇数层返回极小值
        else
            return 19990 + depth; //偶数层返回极大值
    }

    return 0;//将帅都在，返回0

}

 //根据某一走法产生走了之后的棋盘
int SearchEngine::stepMove(ChessStep* move)
{
    int chessManId;
    chessManId = currentBoard[move->to.y()][move->to.x()];          //取目标位置棋子
    //把棋子移动到目标位置
    currentBoard[move->to.y()][move->to.x()] = currentBoard[move->from.y()][move->from.x()];
    currentBoard[move->from.y()][move->from.x()] = -1;//将原位置清空
    return chessManId;                  //返回被吃掉的棋子
}

 //恢复为走过之前的棋盘
void SearchEngine::undoStepMove(ChessStep* move,int chessManId)
{
    currentBoard[move->from.y()][move->from.x()] = currentBoard[move->to.y()][move->to.x()];
    currentBoard[move->to.y()][move->to.x()] = chessManId;//将原位置清空
}



