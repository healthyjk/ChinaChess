#ifndef HISTORYHEURISTIC_H
#define HISTORYHEURISTIC_H
#include "chessdefine.h"

//历史启发类
class HistoryHeuristic
{
public:
    HistoryHeuristic();
    virtual ~HistoryHeuristic();

    void mergeSort(ChessStep* from, int n,bool direction);  //对当前走法队列进行归并排序
    void enterHistoryScore(ChessStep *move,int depth);      //将某一最佳走法汇入历史记录表
    int getHistoryScore(ChessStep *move);                   //取某一走法的历史得分
    void resetHistoryTable();                               //将历史记录表清空

protected:
    void merge_A(ChessStep* source,ChessStep* target,int l,int m, int n);//从小到大排序，MergePass调用
    //用于合并排序好的相邻数据段，MergeSort调用
    void mergePass(ChessStep* source, ChessStep* target,const int s, const int n, const bool direction);

    void merge(ChessStep* source,ChessStep* target,int l,int m, int n);  //从小到大排序，MergePass调用
protected:
    int historyTable[90][90]; //历史得分表
    ChessStep targetBuff[100];//排序用的缓冲队列
};

#endif // HISTORYHEURISTIC_H
