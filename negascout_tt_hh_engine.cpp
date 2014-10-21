#include "negascout_tt_hh_engine.h"
#include <string>

NegaScout_TT_HH_Engine::NegaScout_TT_HH_Engine()
{

}

NegaScout_TT_HH_Engine::~NegaScout_TT_HH_Engine()
{

}

//������Ѳ���
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

//NegaScout��������
int NegaScout_TT_HH_Engine::negaScout(int depth, int alpha, int beta)
{
    int count,i;
    int type;
    int a,b,t;
    int side;
    int score;

    i = isGameOver(currentBoard, depth);        //��Ϸ�Ƿ����

    if(i != 0)
        return i;

    side=(maxDepth - depth) % 2;                         //���㵱ǰ�ڵ�����ͣ����� 0/��С 1
    score = lookUpHashTable(alpha,beta,depth,side);     //��ѯ��ϣ�����Ƿ����
    if(score!=66666)
        return score;
    if(depth <= 0)                                                 //Ҷ�ӽڵ�ȡ��ֵ
    {
        score = chessEvaluation->Eveluate(currentBoard,side);
        enterHashTable(exact,score,depth,side);                    //����ֵ�����û���
        return score;
    }

    count = chessMoveGenerator->createAllPossibleMove(currentBoard,depth,side);  //���������߷�

    for(i = 0;i < count;i++){
        chessMoveGenerator->moveList[depth][i].score =
                getHistoryScore(&chessMoveGenerator->moveList[depth][i]);       //�õ�ĳһ�߷�����ʷ�÷�
    }

    mergeSort(chessMoveGenerator->moveList[depth],count,0);         //�߷�����ʷ�÷�����

    int bestmove = -1;

    a = alpha;
    b = beta;

    int eval_is_exact=0;

    for(i=0;i<count;i++)
    {
        HashStepMove(&chessMoveGenerator->moveList[depth][i],currentBoard);

        type = stepMove(&chessMoveGenerator->moveList[depth][i]);

        t = -negaScout(depth-1,-b,-a);          //��������

        if(t>a && t<beta && i>0)
        {
            //���ڵ�һ����Ľڵ㣬������������ failhigh
            a = -negaScout(depth - 1,-beta,-t);                                /* re-search */
            eval_is_exact = 1;                                              //����������Ϊ��ȷֵ
            if(depth == maxDepth)
                bestMove = chessMoveGenerator->moveList[depth][i];
            bestmove = i;
        }
        undoHashStepMove(&chessMoveGenerator->moveList[depth][i],type,currentBoard); //������ϣ�����
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
