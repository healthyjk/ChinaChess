#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H
#include "chessdefine.h"

//定义了枚举型的数据类型，精确，下边界，上边界
enum ENTRY_TYPE{exact,lower_bound,upper_bound};

//哈希表中元素的结构定义
typedef struct HASHITEM
{
    long long checksum;     //64位校验码
    ENTRY_TYPE entry_type;  //数据类型
    short depth;            //取得此值时的层次
    short eval;             //节点的值
}HashItem;

//置换表类
class TranspositionTable
{
public:
    TranspositionTable();
    virtual ~TranspositionTable();

public:
    void enterHashTable(ENTRY_TYPE entry_type,short eval,short depth,int tableNum);
                                                                    //将当前节点的值存入哈希表
    int lookUpHashTable(int alpha, int beta, int depth,int tableNum);//查询哈希表中当前节点数据
    void undoHashStepMove(ChessStep* move,int chessManId,int currentBoard[10][9]);
                                                                    //撤销所给走法的哈希值，还原成走过之前的
    void HashStepMove(ChessStep* move,int currentBoard[10][9]);     //根据所给走法，增量生成新的哈希值
    void calculateBoardHashKey(int currentBoard[10][9]);            //计算给定棋盘的哈希值
    void initHashKey();

protected:
    long long Rand64(); //产生64位的随机数
    long Rand32();      //产生32位的随机数

public:
    UINT randomHashKey32[15][10][9];         //32位随机数组，用以生成32位哈希值
    ULONGLONG randomHashKey64[15][10][9];    //64位随机数组，用以生成64位哈希值
    HashItem *point_TT[10];                  //置换表头指针
    UINT hashKey32;                          //32位哈希值
    long long  hashKey64;                    //64位哈希值
};

#endif // TRANSPOSITIONTABLE_H
