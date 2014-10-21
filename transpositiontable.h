#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H
#include "chessdefine.h"

//������ö���͵��������ͣ���ȷ���±߽磬�ϱ߽�
enum ENTRY_TYPE{exact,lower_bound,upper_bound};

//��ϣ����Ԫ�صĽṹ����
typedef struct HASHITEM
{
    long long checksum;     //64λУ����
    ENTRY_TYPE entry_type;  //��������
    short depth;            //ȡ�ô�ֵʱ�Ĳ��
    short eval;             //�ڵ��ֵ
}HashItem;

//�û�����
class TranspositionTable
{
public:
    TranspositionTable();
    virtual ~TranspositionTable();

public:
    void enterHashTable(ENTRY_TYPE entry_type,short eval,short depth,int tableNum);
                                                                    //����ǰ�ڵ��ֵ�����ϣ��
    int lookUpHashTable(int alpha, int beta, int depth,int tableNum);//��ѯ��ϣ���е�ǰ�ڵ�����
    void undoHashStepMove(ChessStep* move,int chessManId,int currentBoard[10][9]);
                                                                    //���������߷��Ĺ�ϣֵ����ԭ���߹�֮ǰ��
    void HashStepMove(ChessStep* move,int currentBoard[10][9]);     //���������߷������������µĹ�ϣֵ
    void calculateBoardHashKey(int currentBoard[10][9]);            //����������̵Ĺ�ϣֵ
    void initHashKey();

protected:
    long long Rand64(); //����64λ�������
    long Rand32();      //����32λ�������

public:
    UINT randomHashKey32[15][10][9];         //32λ������飬��������32λ��ϣֵ
    ULONGLONG randomHashKey64[15][10][9];    //64λ������飬��������64λ��ϣֵ
    HashItem *point_TT[10];                  //�û���ͷָ��
    UINT hashKey32;                          //32λ��ϣֵ
    long long  hashKey64;                    //64λ��ϣֵ
};

#endif // TRANSPOSITIONTABLE_H
