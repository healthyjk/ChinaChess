#ifndef HISTORYHEURISTIC_H
#define HISTORYHEURISTIC_H
#include "chessdefine.h"

//��ʷ������
class HistoryHeuristic
{
public:
    HistoryHeuristic();
    virtual ~HistoryHeuristic();

    void mergeSort(ChessStep* from, int n,bool direction);  //�Ե�ǰ�߷����н��й鲢����
    void enterHistoryScore(ChessStep *move,int depth);      //��ĳһ����߷�������ʷ��¼��
    int getHistoryScore(ChessStep *move);                   //ȡĳһ�߷�����ʷ�÷�
    void resetHistoryTable();                               //����ʷ��¼�����

protected:
    void merge_A(ChessStep* source,ChessStep* target,int l,int m, int n);//��С��������MergePass����
    //���ںϲ�����õ��������ݶΣ�MergeSort����
    void mergePass(ChessStep* source, ChessStep* target,const int s, const int n, const bool direction);

    void merge(ChessStep* source,ChessStep* target,int l,int m, int n);  //��С��������MergePass����
protected:
    int historyTable[90][90]; //��ʷ�÷ֱ�
    ChessStep targetBuff[100];//�����õĻ������
};

#endif // HISTORYHEURISTIC_H
