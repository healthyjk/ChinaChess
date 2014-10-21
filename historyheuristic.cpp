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

//�Ե�ǰ�߷����н��й鲢����
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

//��ĳһ����߷�������ʷ��¼��
void HistoryHeuristic::enterHistoryScore(ChessStep *move,int depth)
{
    int nFrom,nTo;
    nFrom = move->from.y() * 9 + move->from.x();//ԭʼλ��
    nTo = move->to.y() * 9 + move->to.x();      //Ŀ��λ��
    historyTable[nFrom][nTo] += 2<<depth;//����Ϊ2��depth �η�
}

//ȡĳһ�߷�����ʷ�÷�
int HistoryHeuristic::getHistoryScore(ChessStep *move)
{
    int nFrom,nTo;
    nFrom = move->from.y() * 9 + move->from.x();//ԭʼλ��
    nTo = move->to.y() * 9 + move->to.x();      //Ŀ��λ��
    return historyTable[nFrom][nTo];            //������ʷ�÷�
}

//����ʷ��¼�����
void HistoryHeuristic::resetHistoryTable()
{
     memset(historyTable,10,8100*4);
}

//��С��������mergePass����
void HistoryHeuristic::merge_A(ChessStep* source,ChessStep* target,int l,int m, int n)
{
    //�Ӵ�С����
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

//���ںϲ�����õ��������ݶΣ�mergeSort����
void HistoryHeuristic::mergePass(ChessStep* source, ChessStep* target,const int s, const int n, const bool direction)
{
    int i=0;

    while(i <= n - 2*s)
    {
        //�ϲ���СΪs�����ڶ���������
        if(direction)
            merge(source,target,i,i+s-1,i+2*s-1);
        else
            merge_A(source,target,i,i+s-1,i+2*s-1);
        i=i+2*s;
    }

    if(i+s<n) //ʣ���Ԫ�ظ���С�2s
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

//��С��������mergePass����
void HistoryHeuristic::merge(ChessStep* source,ChessStep* target,int l,int m, int n)
{
    //��С��������
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
