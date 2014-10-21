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

//初始化Hash Key
void TranspositionTable::initHashKey()
{
    int i,j,k;
    srand((unsigned)time(NULL));
    //填充随机数组
    for(i=0;i<15;i++)
        for(j=0;j<10;j++)
            for(k=0;k<9;k++)
            {
                randomHashKey32[i][j][k] = Rand32();
                randomHashKey64[i][j][k] = Rand64();
            }
    //申请置换表所用空间,1M*2个条目
    point_TT[0]=new HashItem[1024*1024];//用于存放取极大值的节点数据
    point_TT[1]=new HashItem[1024*1024];//用于存放取极小值的节点数据
}

//将当前节点的值存入哈希表
void TranspositionTable::enterHashTable(ENTRY_TYPE entry_type,short eval,short depth,int tableNum)
{
    int x;
    HashItem* pointHashTable;
    x = hashKey32 & 0xFFFFF;//计算二十位哈希地址
    pointHashTable = &point_TT[tableNum][x]; //取到具体的表项指针
    //将数据写入哈希表
    pointHashTable->checksum = hashKey64; //64位校验码
    pointHashTable->entry_type = entry_type;//表项类型
    pointHashTable->eval = eval;            //要保存的值
    pointHashTable->depth = depth;          //层次
}

//查询哈希表中当前节点数据
int TranspositionTable::lookUpHashTable(int alpha, int beta, int depth,int tableNum)
{
    int x;
    HashItem* pointHashTable;

    x = hashKey32 & 0xFFFFF;                         //计算二十位哈希地址
    pointHashTable = &point_TT[tableNum][x];        //取到具体的表项指针

    if(pointHashTable->depth >= depth && pointHashTable->checksum == hashKey64)
    {
        switch(pointHashTable->entry_type)//判断数据类型
        {
        case exact://确切值
            return pointHashTable->eval;
            break;

        case lower_bound://下边界
            if(pointHashTable->eval>=beta)
                return beta;
                //return pointHashTable->eval;
            break;

        case upper_bound://上边界
            if (pointHashTable->eval<=alpha)
                return alpha;
              //  return pointHashTable->eval;
            break;
        }
    }
    return 66666;
}

//撤销所给走法的哈希值，还原成走过之前的
void TranspositionTable::undoHashStepMove(ChessStep* move,int chessManId,int currentBoard[10][9])
{
    int toId,fromId,fromType,toType,chessManType;
    fromId = currentBoard[move->from.y()][move->from.x()];
    toId = currentBoard[move->to.y()][move->to.x()];
    fromType = idToType(fromId);
    toType = idToType(toId);
    chessManType = idToType(chessManId);
    //将移动棋子在移动前位置上的随机数添入
    hashKey32 = hashKey32 ^ randomHashKey32[toType][move->from.y()][move->from.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[toType][move->from.y()][move->from.x()];

    //将移动棋子在现位置上的随机数从哈希值当中去除
    hashKey32 = hashKey32 ^ randomHashKey32[toType][move->to.y()][move->to.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[toType][move->to.y()][move->to.x()];

    if(chessManId != -1)
    {
        //将被吃掉的棋子所对应的随机数恢复进哈希值
        hashKey32 = hashKey32 ^ randomHashKey32[chessManType][move->to.y()][move->to.x()];
        hashKey64 = hashKey64 ^ randomHashKey64[chessManType][move->to.y()][move->to.x()];
    }
}

//根据所给走法，增量生成新的哈希值
void TranspositionTable::HashStepMove(ChessStep* move,int currentBoard[10][9])
{
    int toId,fromId,fromType,toType;

    fromId = currentBoard[move->from.y()][move->from.x()];
    toId = currentBoard[move->to.y()][move->to.x()];
    fromType = idToType(fromId);
    toType = idToType(toId);

    //将要移动的棋子从中去除
    hashKey32 = hashKey32 ^ randomHashKey32[fromType][move->from.y()][move->from.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[fromType][move->from.y()][move->from.x()];

    if(toId != -1)//目标位置有棋子，也要从中去除
    {
        hashKey32 = hashKey32 ^ randomHashKey32[toType][move->to.y()][move->to.x()];
        hashKey64 = hashKey64 ^ randomHashKey64[toType][move->to.y()][move->to.x()];
    }

    //将移动的棋子在目标位置的随机数添入
    hashKey32 = hashKey32 ^ randomHashKey32[fromType][move->to.y()][move->to.x()];
    hashKey64 = hashKey64 ^ randomHashKey64[fromType][move->to.y()][move->to.x()];
}

//计算给定棋盘的哈希值
void TranspositionTable::calculateBoardHashKey(int currentBoard[10][9])
{
    int j,k,chessManType,chessManId;
    hashKey32=0;
    hashKey64=0;
    //将所有棋子对应的哈希数加总
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

//产生64位的随机数
long long TranspositionTable::Rand64()
{
    return rand()^((long long)rand()<<15)^((long long)rand()<<30)^
        ((long long)rand()<<45)^((long long)rand()<<60);
}
//产生32位的随机数
long TranspositionTable::Rand32()
{
    return rand()^((long)rand()<<15)^((long)rand()<<30);
}
