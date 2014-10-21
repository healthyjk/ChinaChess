#ifndef NEGASCOUT_TT_HH_ENGINE_H
#define NEGASCOUT_TT_HH_ENGINE_H

#include "historyheuristic.h"
#include "transpositiontable.h"
#include "searchengine.h"
#include "chessdefine.h"

//基于置换表和历史启发的空窗探测引擎类
class NegaScout_TT_HH_Engine : public TranspositionTable, public HistoryHeuristic, public SearchEngine,public QThread
{
public:
    NegaScout_TT_HH_Engine();
    virtual ~NegaScout_TT_HH_Engine();

public:
    virtual void searchOneGoodMove(const int board[10][9]);

protected:
    int negaScout(int depth, int alpha, int beta);      //NegaScout搜索函数

};

#endif // NEGASCOUT_TT_HH_ENGINE_H
