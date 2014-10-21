#include "transpositiontable.h"
#include <stdlib.h>
#include <time.h>
#include <QDebug>

TranspositionTable::TranspositionTable()
{
    initHashKey();
}

TranspositionTable::~TranspositionTable()
{
    delete this->point_TT[0];
    delete this->point_TT[1];
}

//��ʼ��Hash Key
void TranspositionTable::initHashKey()
{
    int i,j,k;
    srand((unsigned)time(NULL));
    //����������
    for(i=0;i<15;i++)
        for(j=0;j<10;j++)
            for(k=0;k<9;k++)
            {
                randomHashKey32[i][j][k] = Rand32();
                randomHashKey64[i][j][k] = Rand64();
            }
    //�����û������ÿռ�,1M*2����Ŀ
    point_TT[0]=new HashItem[1024*1024];//���ڴ��ȡ����ֵ�Ľڵ�����
    point_TT[1]=new HashItem[1024*1024];//���ڴ��ȡ��Сֵ�Ľڵ�����
}

//����ǰ�ڵ��ֵ�����ϣ��
void TranspositionTable::enterHashTable(ENTRY_TYPE entry_type,short eval,short depth,int tableNum)
{
    int x;
    HashItem* pointHashTable;
    x = hashKey32 & 0xFFFFF;//�����ʮλ��ϣ��ַ
    pointHashTable = &point_TT[tableNum][x]; //ȡ������ı���ָ��
    //������д���ϣ��
    pointHashTable->checksum = hashKey64; //64λУ����
    pointHashTable->entry_type = entry_type;//��������
    pointHashTable->eval = eval;            //Ҫ�����ֵ
    pointHashTable->depth = depth;          //���
}

//��ѯ��ϣ���е�ǰ�ڵ�����
int TranspositionTable::lookUpHashTable(int alpha, int beta, int depth,int tableNum)
{
    int x;
    HashItem* pointHashTable;

    x = hashKey32 & 0xFFFFF;                         //�����ʮλ��ϣ��ַ
    pointHashTable = &point_TT[tableNum][x];        //ȡ������ı���ָ��

    if(pointHashTable->depth >= depth && pointHashTable->checksum == hashKey64)
    {
        switch(pointHashTable->entry_type)//�ж���������
        {
        case exact://ȷ��ֵ
            return pointHashTable->eval;
            break;

        case lower_bound://�±߽�
            if(pointHashTable->eval>=beta)
                return beta;
                //return pointHashTable->eval;
            break;

        case upper_bound://�ϱ߽�
            if (pointHashTable->eval<=alpha)
                return alpha;
              //  return pointHashTable->eval;
            break;
        }
    }
    return 66666;
}

//���������߷��Ĺ�ϣֵ����ԭ���߹�֮ǰ��
void TranspositionTable::undoHashStepMove(ChessStep* move,int chessManId,int currentBoard[10][9])
{
    int toId,fromId,fromType,toType,chessManType;
    fromId = currentBoard[move->from.y()][move->from.x()];
    toId = currentBoard[move->to.y()][move->to.x()];
    fromType = idToType(fromId);
    toType = idToType(toId);
    chessManType = idToType(chessManId);
    //���ƶ��������ƶ�ǰλ���ϵ����������
    hashKey32 = hashKey32 ^ randomHashKey32[toType][move->from.y()][move->from.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[toType][move->from.y()][move->from.x()];

    //���ƶ���������λ���ϵ�������ӹ�ϣֵ����ȥ��
    hashKey32 = hashKey32 ^ randomHashKey32[toType][move->to.y()][move->to.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[toType][move->to.y()][move->to.x()];

    if(chessManId != -1)
    {
        //�����Ե�����������Ӧ��������ָ�����ϣֵ
        hashKey32 = hashKey32 ^ randomHashKey32[chessManType][move->to.y()][move->to.x()];
        hashKey64 = hashKey64 ^ randomHashKey64[chessManType][move->to.y()][move->to.x()];
    }
}

//���������߷������������µĹ�ϣֵ
void TranspositionTable::HashStepMove(ChessStep* move,int currentBoard[10][9])
{
    int toId,fromId,fromType,toType;

    fromId = currentBoard[move->from.y()][move->from.x()];
    toId = currentBoard[move->to.y()][move->to.x()];
    fromType = idToType(fromId);
    toType = idToType(toId);

    //��Ҫ�ƶ������Ӵ���ȥ��
    hashKey32 = hashKey32 ^ randomHashKey32[fromType][move->from.y()][move->from.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[fromType][move->from.y()][move->from.x()];

    if(toId != -1)//Ŀ��λ�������ӣ�ҲҪ����ȥ��
    {
        hashKey32 = hashKey32 ^ randomHashKey32[toType][move->to.y()][move->to.x()];
        hashKey64 = hashKey64 ^ randomHashKey64[toType][move->to.y()][move->to.x()];
    }

    //���ƶ���������Ŀ��λ�õ����������
    hashKey32 = hashKey32 ^ randomHashKey32[fromType][move->to.y()][move->to.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[fromType][move->to.y()][move->to.x()];
}

//����������̵Ĺ�ϣֵ
void TranspositionTable::calculateBoardHashKey(int currentBoard[10][9])
{
    int j,k,chessManType,chessManId;
    hashKey32=0;
    hashKey64=0;
    //���������Ӷ�Ӧ�Ĺ�ϣ������
    for(j=0;j<10;j++)
        for(k=0;k<9;k++)
        {
            chessManId = currentBoard[j][k];
            chessManType = idToType(chessManId);
            if(chessManId != -1)
            {
                hashKey32=hashKey32 ^ randomHashKey32[chessManType][j][k];
                hashKey64=hashKey64 ^ randomHashKey64[chessManType][j][k];
            }
        }
}

//����64λ�������
long long TranspositionTable::Rand64()
{
    return rand()^((long long)rand()<<15)^((long long)rand()<<30)^
        ((long long)rand()<<45)^((long long)rand()<<60);
}
//����32λ�������
long TranspositionTable::Rand32()
{
    return rand()^((long)rand()<<15)^((long)rand()<<30);
}
