#include "chessevaluation.h"
#include "chessdefine.h"
#include <QString>
#include <QSound>
#include <QDebug>

//下面两个常量数组存放了兵在不同位置的附加值
//基本上是过河之后越靠近老将分值越高
//红兵的附加值矩阵
const int R_PAWN_ADD[10][9]=
{
    {0,0,0,0,0,0,0,0,0},
    {120,120,140,150,150,150,140,120,120},
    {120,120,140,150,150,150,140,120,120},
    {100,120,140,140,140,140,140,120,100},
    {100,100,100,100,100,100,100,100,100},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
};

//黑卒的附加值矩阵
const int B_PAWN_ADD[10][9]=
{
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {100,100,100,100,100,100,100,100,100},
    {100,120,140,140,140,140,140,120,100},
    {120,120,140,150,150,150,140,120,120},
    {120,120,140,150,150,150,140,120,120},
    {0,0,0,0,0,0,0,0,0},
};


ChessEvaluation::ChessEvaluation()
{
    //初始化价值数组
    this->baseValue[B_KING] = BASEVALUE_KING;
    this->baseValue[B_CAR] = BASEVALUE_CAR;
    this->baseValue[B_HORSE]= BASEVALUE_HORSE;
    this->baseValue[B_ELEPHANT] = BASEVALUE_ELEPHANT;
    this->baseValue[B_BISHOP] = BASEVALUE_BISHOP;
    this->baseValue[B_CANON] = BASEVALUE_CANON;
    this->baseValue[B_PAWN] = BASEVALUE_PAWN;
    this->baseValue[R_KING] = BASEVALUE_KING;
    this->baseValue[R_CAR] = BASEVALUE_CAR;
    this->baseValue[R_HORSE]= BASEVALUE_HORSE;
    this->baseValue[R_ELEPHANT] = BASEVALUE_ELEPHANT;
    this->baseValue[R_BISHOP] = BASEVALUE_BISHOP;
    this->baseValue[R_CANON] = BASEVALUE_CANON;
    this->baseValue[R_PAWN] = BASEVALUE_PAWN;

    //初始化灵活值分数组
    this->flexibilityValue[B_KING] = FLEXIBILITY_KING;
    this->flexibilityValue[B_CAR] = FLEXIBILITY_CAR;
    this->flexibilityValue[B_HORSE]= FLEXIBILITY_HORSE;
    this->flexibilityValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT;
    this->flexibilityValue[B_BISHOP] = FLEXIBILITY_BISHOP;
    this->flexibilityValue[B_CANON] = FLEXIBILITY_CANON;
    this->flexibilityValue[B_PAWN] = FLEXIBILITY_PAWN;
    this->flexibilityValue[R_KING] = FLEXIBILITY_KING;
    this->flexibilityValue[R_CAR] = FLEXIBILITY_CAR;
    this->flexibilityValue[R_HORSE]= FLEXIBILITY_HORSE;
    this->flexibilityValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT;
    this->flexibilityValue[R_BISHOP] = FLEXIBILITY_BISHOP;
    this->flexibilityValue[R_CANON] = FLEXIBILITY_CANON;
    this->flexibilityValue[R_PAWN] = FLEXIBILITY_PAWN;

    //初始化访问节点数
    this->accessCount = 0;
}

ChessEvaluation::~ChessEvaluation()
{

}

int ChessEvaluation::Eveluate(const int board[10][9],int turn)
{
    int fromType,toType,fromId,toId;
    int i,j,k;
    QPoint from;
    this->accessCount++;//每调用一次就增加一次
    //初始化
    memset(this->chessValuePosition,0,360);
    memset(this->threadPosition,0,360);
    memset(this->guardPosition,0,360);
    memset(this->flexibilityPosition,0,360);

    //扫描棋盘，找出每一个棋子，及其威胁/保护的棋子，还有其灵活性
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromId = board[i][j];
                fromType = idToType(fromId);                  //取棋子类型

                from.setX(j);
                from.setY(i);
                getRelatePiece(board,from);                   //找出该棋子所有相关位置

                for(k = 0;k < this->relatedPosCount;k++)      //对每一目标位置
                {
                    toId = board[relatePos[k].y()][relatePos[k].x()];  //取目标棋子类型
                    toType = idToType(toId);
                    if(toType==NOCHESS)                         //如果是空白，灵活性增加
                        flexibilityPosition[i][j]++;
                    else                        //有棋子
                    {
                        if(IsSameColor(fromType,toType))
                        {
                            //己方棋子，目标受保护
                            guardPosition[relatePos[k].y()][relatePos[k].x()]++;
                        }
                        else
                        {
                            //敌方棋子，目标受威胁
                            threadPosition[relatePos[k].y()][relatePos[k].x()]++;
                            flexibilityPosition[i][j]++;           //灵活性增加
                            switch(toType)
                            {
                            case R_KING://红帅
                                if(!turn)  {                //轮到黑棋走
                                    qDebug() << "hongshuai";
                                    QSound::play("./sounds/ILLEGAL.wav");
                                    return 18888;          //返回失败极值
                                }

                                break;

                            case B_KING:                //黑将
                                if(turn)     {           //轮到红棋走
                                    qDebug() << "heishuai";
                                    QSound::play("./sounds/ILLEGAL.wav");
                                    return 18888;       //返回失败极值
                                }
                                break;

                            default:                  //根据威胁的棋子加上威胁分值
                                threadPosition[relatePos[k].y()][relatePos[k].x()] +=
                                        (30 +(baseValue[toType]- baseValue[fromType]) / 10) / 10;
                                break;
                            }
                        }
                    }
                }
            }
        }

    //下面的循环统计扫描到的数据
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromType = idToType(board[i][j]);           //取棋子类型
                from.setX(j);
                from.setY(i);
                chessValuePosition[i][j]++;

                //如果棋子存在其价值不为0，把每一棋子的灵活性价值加进棋子价值

                chessValuePosition[i][j] += flexibilityValue[fromType]
                                            * flexibilityPosition[i][j];
                //加上兵的附加值
                chessValuePosition[i][j] += getPAWNValue(board,from);
            }
        }

    //下面的循环继续统计扫描到的数据,计算每个棋子的总价值
    int halfValue;
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            if(board[i][j] != -1)
            {
                fromId = board[i][j];
                fromType = idToType(fromId);            //取棋子类型
                halfValue = baseValue[fromType] / 16;        //棋子基本价值的1/16作为威胁/保护增量
                chessValuePosition[i][j] += baseValue[fromType]; //每个棋子的基本价值加入其总价值
                if(IsRed(fromId))//红棋
                {
                    if(threadPosition[i][j])//当前红棋如果被威胁
                    {
                        if(turn)//轮到红棋走
                        {
                            if(fromType == R_KING)                   //如果是红将
                                chessValuePosition[i][j] -= 20;       //价值降低20
                            else
                            {
                                //价值减去2倍halfValue
                                chessValuePosition[i][j] -= halfValue * 2;
                                if(guardPosition[i][j])                        //是否被己方棋子保护
                                    chessValuePosition[i][j] += halfValue;         //被保护再加上nHalfvalue
                            }
                        }
                        else        //当前红棋被威胁，轮到黑棋走
                        {
                            if(fromType == R_KING)          //是否是红帅
                                return 18888;           //返回失败极值
                            chessValuePosition[i][j] -= halfValue * 10;         //减去10倍的nHalfvalue,表示威胁程度高
                            if(guardPosition[i][j])         //如果被保护
                                chessValuePosition[i][j] += halfValue * 9;      //被保护再加上9倍的nHalfvalue
                        }
                        //被威胁的棋子加上威胁差，防止一个兵威胁一个被保护的车，而估值函数没有反映此类问题
                        chessValuePosition[i][j] -= threadPosition[i][j];
                    }
                    else
                    {
                        //没受威胁
                        if(threadPosition[i][j])
                            chessValuePosition[i][j] += 5;            //受保护，加一点分
                    }
                }
                else
                {
                    //如果是黑棋
                    if(threadPosition[i][j])
                    {
                        //受威胁
                        if(!turn)
                        {
                            //轮到黑棋走
                            if(fromType == B_KING)                      //如果是黑将
                                chessValuePosition[i][j]-=20;                 //棋子价值降低20
                            else
                            {
                                //棋子价值降低2倍halfvalue
                                chessValuePosition[i][j] -= halfValue * 2;
                                if(guardPosition[i][j])//如果受保护
                                    chessValuePosition[i][j]  += halfValue;       //棋子价值增加nHalfvalue
                            }
                        }
                        else
                        {
                            //轮到红棋走
                            if(fromType == B_KING)      //是黑将
                                return 18888;       //返回失败极值
                            chessValuePosition[i][j] -= halfValue * 10;         //棋子价值减少10倍nHalfvalue
                            if(guardPosition[i][j])//受保护
                                chessValuePosition[i][j] += halfValue*9;        //被保护再加上9倍nHalfvalue
                        }
                        //被威胁的棋子再加上威胁差,防止一个兵威胁一个被保护的车,而估值函数没有反映此类的问题
                        chessValuePosition[i][j] -= threadPosition[i][j];
                    }
                    else
                    {
                        //不受威胁
                        if(guardPosition[i][j])
                            chessValuePosition[i][j] += 5;              //受保护，加一点分
                    }
                }
            }
        }

    int redValue = 0;
    int blackValue = 0;
    for(i=0;i<10;i++)
        for(j=0;j<9;j++)
        {
            fromId = board[i][j];
            fromType = idToType(fromId);
            if(fromId != -1)
            {
                if(IsRed(fromId))
                    redValue += chessValuePosition[i][j];  //把红棋的值加总
                else
                    blackValue += chessValuePosition[i][j];//把红棋的值加总
            }
        }

    if(turn)
        return redValue-blackValue;         //如果轮到红棋走返回估值
    else
        return blackValue-redValue;             //如果轮到黑棋走返回负估值



}


int ChessEvaluation::getRelatePiece(const int board[10][9],QPoint& from)
{
    this->relatedPosCount=0;
    int fromType;
    QPoint to;
    int i = from.y();
    int j = from.x();
    fromType = idToType(board[i][j]);           //取棋子类型

    switch(fromType)
    {
    case R_KING://红帅
    case B_KING://黑将
        //循环检查九宫之内哪些位置可到达/保护，包括将帅是否碰面的情况
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++)
                if(canMoveToDst(board,from,to))         //能否到达
                        AddPoint(to.rx(),to.ry());      //可达到/保护的位置加入数组

        for(to.ry() = 7;to.ry() < 10;to.ry()++)
            for(to.rx() = 3;to.rx() < 6;to.rx()++)
                if(canMoveToDst(board,from,to))         //能否到达
                        AddPoint(to.rx(),to.ry());      //可达到/保护的位置加入数组

        break;

    case R_BISHOP://红士
        //循环检查九宫之内哪些位置可到达/保护
        for(to.ry() = 7;to.ry() < 10;to.ry()++)
            for(to.rx()=3;to.rx()<6;to.rx()++)
                if(canMoveToDst(board,from,to))         //能否到达
                        AddPoint(to.rx(),to.ry());      //可达到/保护的位置加入数组

        break;

    case B_BISHOP://黑士
        //循环检查九宫之内哪些位置可到达/保护
        for(to.ry()=0;to.ry()<3;to.ry()++)
            for(to.rx() = 3;to.rx() < 6;to.rx()++)
                if(canMoveToDst(board,from,to))         //能否到达
                        AddPoint(to.rx(),to.ry());      //可达到/保护的位置加入数组
        break;

    case R_ELEPHANT://红相
    case B_ELEPHANT://黑象
        //右下
        to.rx() = j+2;
        to.ry() = i+2;
        if(canMoveToDst(board,from,to))         //能否到达
                AddPoint(to.rx(),to.ry());      //可达到/保护的位置加入数组

        //右上
        to.rx()=j+2;
        to.ry()=i-2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        //左下
        to.rx()=j-2;
        to.ry()=i+2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        //左上
        to.rx()=j-2;
        to.ry()=i-2;
        if(canMoveToDst(board,from,to))
                AddPoint(to.rx(),to.ry());

        break;

        case R_HORSE://红马
        case B_HORSE://黑马
            //检查右下方能否到达/保护
            to.rx()=j+2;
            to.ry()=i+1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查右上方能否到达/保护
            to.rx()=j+2;
            to.ry()=i-1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查左下方能否到达/保护
            to.rx()=j-2;
            to.ry()=i+1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查左上方能否到达/保护
            to.rx()=j-2;
            to.ry()=i-1;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查右下方能否到达/保护
            to.rx()=j+1;
            to.ry()=i+2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查右上方能否到达/保护
            to.rx()=j+1;
            to.ry()=i-2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查左下方能否到达/保护
            to.rx()=j-1;
            to.ry()=i+2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            //检查左上方能否到达/保护
            to.rx()=j-1;
            to.ry()=i-2;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());

            break;

        case R_CAR://红车
        case B_CAR://黑车
            //检查向右能否到达/保护
            to.rx()=j+1;
            to.ry()=i;
            while(to.rx()<9)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()++;
            }

            //检查向左能否到达/保护
            to.rx()=j-1;
            to.ry()=i;
            while(to.rx()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()--;
            }

            //检查向下能否到达/保护
            to.rx()=j;
            to.ry()=i+1;
            while(to.ry()<10)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()++;
            }

            //检查向上能否到达/保护
            to.rx()=j;
            to.ry()=i-1;
            while(to.ry()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()--;
            }

            break;

        case R_PAWN://红兵
            //观看向前是否到底
            to.ry()=i-1;
            to.rx()=j;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());
            if(i<5)
            {
                //如已过河
                to.ry()=i;
                to.rx()=j+1;//向右
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()=j-1;//向左
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
            }

            break;

        case B_PAWN://黑卒
            //观看向前是否到底
            to.ry()=i+1;
            to.rx()=j;
            if(canMoveToDst(board,from,to))
                    AddPoint(to.rx(),to.ry());
            if(i>4)
            {
                //如已过河
                to.ry()=i;
                to.rx()=j+1;//向右
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()=j-1;//向左
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
            }

            break;

        case B_CANON://黑炮
        case R_CANON://红炮
            //检查向右能否到达/保护的位置
            to.rx()=j+1;
            to.ry()=i;
            while(to.rx()<9)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()++;//继续向右
            }

            //检查向左能否到达/保护的位置
            to.rx()=j-1;
            to.ry()=i;
            while(to.rx()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.rx()--;//继续向左
            }

            //检查向下能否到达/保护的位置
            to.rx()=j;
            to.ry()=i+1;
            while(to.ry()<10)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()++;//继续向下
            }

            //检查向上能否到达/保护的位置
            to.rx()=j;
            to.ry()=i-1;
            while(to.ry()>=0)
            {
                if(canMoveToDst(board,from,to))
                        AddPoint(to.rx(),to.ry());
                to.ry()--;//继续向上
            }

            break;

        default:
            break;
        }
    return relatedPosCount;
}

int ChessEvaluation::getPAWNValue(const int board[10][9],QPoint& from)
{
    if(idToType(board[from.y()][from.x()]) == R_PAWN)
        return R_PAWN_ADD[from.y()][from.x()];
    else if(idToType(board[from.y()][from.x()]) == B_PAWN)
        return B_PAWN_ADD[from.y()][from.x()];
    return 0;
}

void ChessEvaluation::AddPoint(int x,int y)
{
    relatePos[relatedPosCount].setX(x);
    relatePos[relatedPosCount].setY(y);
    relatedPosCount++;
}


