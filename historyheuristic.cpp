#include "historyheuristic.h"
#include <string.h>

HistoryHeuristic::HistoryHeuristic()
{
}

HistoryHeuristic::~HistoryHeuristic()
{
   // delete this->historyTable;
   // delete this->targetBuff;
}

//对当前走法队列进行归并排序
void HistoryHeuristic::mergeSort(ChessStep* from, int n,bool direction)
{
    int s = 1;
    while(s < n)
    {
        mergePass(from,targetBuff,s,n,direction);
        s += s;
        mergePass(targetBuff,from,s,n,direction);
        s += s;
    }
}

//将某一最佳走法汇入历史记录表
void HistoryHeuristic::enterHistoryScore(ChessStep *move,int depth)
{
    int nFrom,nTo;
    nFrom = move->from.y() * 9 + move->from.x();//原始位置
    nTo = move->to.y() * 9 + move->to.x();      //目标位置
    historyTable[nFrom][nTo] += 2<<depth;//增量为2的depth 次方
}

//取某一走法的历史得分
int HistoryHeuristic::getHistoryScore(ChessStep *move)
{
    int nFrom,nTo;
    nFrom = move->from.y() * 9 + move->from.x();//原始位置
    nTo = move->to.y() * 9 + move->to.x();      //目标位置
    return historyTable[nFrom][nTo];            //返回历史得分
}

//将历史记录表清空
void HistoryHeuristic::resetHistoryTable()
{
     memset(historyTable,10,8100*4);
}

//从小到大排序，mergePass调用
void HistoryHeuristic::merge_A(ChessStep* source,ChessStep* target,int l,int m, int n)
{
    //从大到小排序
    int i = l;
    int j = m+1;
    int k = l;

    while(i <= m && j <= n)
        if(source[i].score >= source[j].score)
            target[k++] = source[i++];
        else
            target[k++] = source[j++];

    if(i > m)
        for(int q = j;q <= n;q++)
            target[k++] = source[q];
        else
            for(int q=i;q<=m;q++)
                target[k++] = source[q];
}

//用于合并排序好的相邻数据段，mergeSort调用
void HistoryHeuristic::mergePass(ChessStep* source, ChessStep* target,const int s, const int n, const bool direction)
{
    int i=0;

    while(i <= n - 2*s)
    {
        //合并大小为s的相邻二段子数组
        if(direction)
            merge(source,target,i,i+s-1,i+2*s-1);
        else
            merge_A(source,target,i,i+s-1,i+2*s-1);
        i=i+2*s;
    }

    if(i+s<n) //剩余的元素个数小於2s
    {
        if(direction)
            merge(source,target,i,i+s-1,n-1);
        else
            merge_A(source,target,i,i+s-1,n-1);
    }
    else
        for(int j=i;j<=n-1;j++)
            target[j]=source[j];
}

//从小到大排序，mergePass调用
void HistoryHeuristic::merge(ChessStep* source,ChessStep* target,int l,int m, int n)
{
    //从小到大排序
    int i = l;
    int j = m + 1;
    int k = l;

    while(i <= m && j <= n)
        if(source[i].score <= source[j].score)
            target[k++]=source[i++];
        else
            target[k++]=source[j++];

    if(i > m)
        for(int q=j; q<=n;q++)
            target[k++] = source[q];
        else
            for(int q=i; q<=m;q++)
                target[k++] = source[q];
}
