#ifndef NEGASCOUT_TT_HH_ENGINE_H
#define NEGASCOUT_TT_HH_ENGINE_H

#include "historyheuristic.h"
#include "transpositiontable.h"
#include "searchengine.h"
#include "chessdefine.h"

//�����û������ʷ�����Ŀմ�̽��������
class NegaScout_TT_HH_Engine : public TranspositionTable, public HistoryHeuristic, public SearchEngine,public QThread
{
public:
    NegaScout_TT_HH_Engine();
    virtual ~NegaScout_TT_HH_Engine();

public:
    virtual void searchOneGoodMove(const int board[10][9]);

protected:
    int negaScout(int depth, int alpha, int beta);      //NegaScout��������

};

#endif // NEGASCOUT_TT_HH_ENGINE_H
