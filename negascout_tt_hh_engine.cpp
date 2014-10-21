#include "negascout_tt_hh_engine.h"
#include <string>

NegaScout_TT_HH_Engine::NegaScout_TT_HH_Engine()
{

}

NegaScout_TT_HH_Engine::~NegaScout_TT_HH_Engine()
{

}

//搜索最佳步法
void NegaScout_TT_HH_Engine::searchOneGoodMove(const int board[10][9])
{
    int score;
    memcpy(currentBoard,board,360);
    maxDepth = searchDepth;
    calculateBoardHashKey(currentBoard);
    resetHistoryTable();
    score = negaScout(maxDepth,-20000,20000);
    undoChessStep.lastStep = bestMove;
    undoChessStep.killedChessManId = stepMove(&bestMove);
}

//NegaScout搜索函数
int NegaScout_TT_HH_Engine::negaScout(int depth, int alpha, int beta)
{
    int count,i;
    int type;
    int a,b,t;
    int side;
    int score;

    i = isGameOver(currentBoard, depth);        //游戏是否结束

    if(i != 0)
        return i;

    side=(maxDepth - depth) % 2;                         //计算当前节点的类型，极大 0/极小 1
    score = lookUpHashTable(alpha,beta,depth,side);     //查询哈希表中是否存在
    if(score!=66666)
        return score;
    if(depth <= 0)                                                 //叶子节点取估值
    {
        score = chessEvaluation->Eveluate(currentBoard,side);
        enterHashTable(exact,score,depth,side);                    //将估值存入置换表
        return score;
    }

    count = chessMoveGenerator->createAllPossibleMove(currentBoard,depth,side);  //产生左右走法

    for(i = 0;i < count;i++){
        chessMoveGenerator->moveList[depth][i].score =
                getHistoryScore(&chessMoveGenerator->moveList[depth][i]);       //得到某一走法的历史得分
    }

    mergeSort(chessMoveGenerator->moveList[depth],count,0);         //走法的历史得分排序

    int bestmove = -1;

    a = alpha;
    b = beta;

    int eval_is_exact=0;

    for(i=0;i<count;i++)
    {
        HashStepMove(&chessMoveGenerator->moveList[depth][i],currentBoard);

        type = stepMove(&chessMoveGenerator->moveList[depth][i]);

        t = -negaScout(depth-1,-b,-a);          //迭代搜索

        if(t>a && t<beta && i>0)
        {
            //对于第一个后的节点，如果上面的搜索 failhigh
            a = -negaScout(depth - 1,-beta,-t);                                /* re-search */
            eval_is_exact = 1;                                              //设数据类型为精确值
            if(depth == maxDepth)
                bestMove = chessMoveGenerator->moveList[depth][i];
            bestmove = i;
        }
        undoHashStepMove(&chessMoveGenerator->moveList[depth][i],type,currentBoard); //撤销哈希表操作
        undoStepMove(&chessMoveGenerator->moveList[depth][i],type);
        if(a < t)
        {
            eval_is_exact = 1;
            a = t;
            if(depth == maxDepth)
                bestMove = chessMoveGenerator->moveList[depth][i];
        }
        if(a >= beta)
        {
            enterHashTable(lower_bound,a,depth,side);
            enterHistoryScore(&chessMoveGenerator->moveList[depth][i],depth);
            return a;
        }
        b = a+1;
    }
    if(bestmove != -1)
        enterHistoryScore(&chessMoveGenerator->moveList[depth][bestmove], depth);
    if (eval_is_exact)
        enterHashTable(exact,a,depth,side);
    else
        enterHashTable(upper_bound,a,depth,side);
    return a;
}
