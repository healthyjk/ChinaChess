#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <chessdefine.h>
#include <chessevaluation.h>
#include <chessmovegenerator.h>
#include <string.h>

//搜索引擎的基类
class SearchEngine
{
public:
    SearchEngine();
    virtual ~SearchEngine();

public:
    virtual void searchOneGoodMove(const int board[10][9]) = 0;     //搜索最佳棋步
    int isGameOver(const int board[10][9],int depth);
    //根据某一走法产生走了之后的棋盘
    int stepMove(ChessStep* move);
    //恢复为走过之前的棋盘
    void undoStepMove(ChessStep* move,int chessManId);
    ChessStep getBestMove(){return bestMove;}          //得到最佳走法
    void setSearchDepth(int depth){this->searchDepth = depth;}//设定最大搜索深度
    void setEvaluator(ChessEvaluation* chessEvaluation){this->chessEvaluation=chessEvaluation;}  //设定估值引擎
    void setMoveGenerator(ChessMoveGenerator* chessMoveGenerator){this->chessMoveGenerator =chessMoveGenerator;}//设定走法产生器
    ChessMoveGenerator* chessMoveGenerator;          //走法产生器

protected:
    int currentBoard[10][9];                 //搜索时用于记录当前节点棋盘状态的数组
    ChessStep bestMove;                      //记录最佳走法
    UnDoChessStep undoChessStep;
    ChessEvaluation* chessEvaluation;        //估值核心
    int searchDepth;                         //最大搜索深度
    int maxDepth;                            //当前搜索的最大搜索深度
};

#endif
