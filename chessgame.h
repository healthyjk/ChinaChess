#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QWidget>
#include <QString>
#include <QLCDNumber>
#include <phonon/mediaobject.h>
#include <phonon/AudioOutput>

#include "chessdefine.h"
#include "chessman.h"
#include "chessmovegenerator.h"
#include "chessevaluation.h"
#include "searchengine.h"
#include "transpositiontable.h"
#include "historyheuristic.h"
#include "negascout_tt_hh_engine.h"
#include "stopwatch.h"
#include "configdialog.h"



//电脑搜索步法线程类
class ComputerThread : public QThread
{
    Q_OBJECT
public:
    ComputerThread(int chessBoard[10][9],NegaScout_TT_HH_Engine *engine, int compLevel);
    ~ComputerThread();
    void run();
signals:
    void computerMoveSignal(const int x1,const int y1,const int x2,const int y2);
private:
    int board[10][9];
    NegaScout_TT_HH_Engine *engine;     //搜索引擎
    ChessStep bestMove;                 //最佳步法
    int compLevel;                      //电脑等级
};


//背景音乐播放线程类
class groundMusic: public QThread
{
    Q_OBJECT
public:
    groundMusic();
    ~groundMusic();
    void stop();        //停止播放音乐
public slots:
    void MusicOn();     //重复播放背景音乐槽函数
protected:
    void run();
private:
    Phonon::MediaObject *mediaObject;       //多媒体类实例化
};

//主游戏类
class ChessGame : public QWidget
{
    Q_OBJECT
public:
    explicit ChessGame(QWidget *parent = 0);
    enum ChessState{DRAW,RED_WIN,BLACK_WIN,LAST};   //游戏状态，分别为：和棋，红子胜，黑子胜，还在进行中
    void initChessBoard();                          //初始化棋盘
    void initInterface();                           //初始化界面
    void initParameter();                           //初始化游戏参数(对战模式，电脑等级,music)
    void playBgMusic();                             //播放背景音乐
    ChessState isGameOver();                        //判断游戏是否结束
    bool moveChessMan(QPoint& from,QPoint& to);     //移动棋子函数
    void printChessStep(int chessManId,QPoint& from,QPoint& to);    //打印出标准棋步
    void recordSteps(QPoint &from, QPoint &to);                     //记录下棋步法
    void stopGame(ChessState state);                                //停止游戏
    void computerThink();                           //电脑开启线程思考函数
    void dispTime();                                //显示时间
    void resetTime();                               //重置时间
    void startTime();                               //开始计时
    void stopTime();                                //停止计时
    void saveChess(QString fileName);               //保存当前棋局
    void loadChess(QString fileName);               //载入历史保存的棋局
    void newMMGame();                               //新的双人对战
    void newMPGame();                               //新的人机对战
    void backone();                                 //悔棋一步函数

public slots:
    void setCustomConfig(const int chessMode,const int compuLevel,
                         const bool bgMusicSwitch,const bool atMusicSwitch);        //按用户自定义选项设置参数函数
    void setDefaultConfig();                                                        //按默认选项设置参数
    void configuration();                                                           //选项设置弹出菜单函数
    void computerRun(const int x1,const int y1,const int x2,const int y2);         //计算机下棋方法
    void newGame();                                                                 //新游戏函数
    void back();                                                                    //悔棋按钮事件
    void modeChange();                                                              //模式切换按钮事件
    void surrender();                                                               //认输按钮事件
    void addTime();                                                                 //计时时间每秒加1函数

protected:
    void mousePressEvent(QMouseEvent *);//监听鼠标点击事件
    void paintEvent(QPaintEvent *);     //界面绘制重载方法

private:
    QLabel *user1;                      //用户1人物介绍
    QLabel *user2;                      //用户2人物介绍
    QTextBrowser *moveHistory;          //显示历史步法文本框
    QPushButton *undoButton;            //悔棋按钮
    QPushButton *modeChangeButton;      //模式切换按钮
    QPushButton *surrenderButton;       //认输按钮
    QPushButton *tryAgainButton;        //重新开始游戏按钮

    QLCDNumber *lcdNumberBlack;         //黑方时间显示液晶屏
    QLCDNumber *lcdNumberRed;           //红方时间显示液晶屏
    QTimer *timer;                      //游戏计时器
    QTime timeBlack;                    //黑方所用时间
    QTime timeRed;                      //红方所用时间

    ConfigDialog *configDialog;         //设置对话框

    NegaScout_TT_HH_Engine* engine;     //搜索引擎
    ChessMoveGenerator* moveGenerator;  //走法产生器
    ChessEvaluation* evaluator;         //局面评估类

    int chessBoard[10][9];                          //使用10x9的二维数组来存储棋盘,棋盘中存放棋子id
    ChessMan *chessMan[32];                         //存储32颗棋子
    int selecteid;                                  //已经选中棋子的id
    int turn;                                       //下一步下棋的是哪方，0黑1红

    int chessMode;          //游戏模式
    int compLevel;          //电脑等级
    bool bgMusicSwitch;     //背景音乐开关
    bool atMusicSwitch;     //下棋动作音效开关
    int chumen,hanmen;      //双方人物的随机编号

    QPoint lastMoveF,lastMoveT;                   //上一步的起点和终点
    ComputerThread* computerThread[500];          //电脑搜索线程
    ChessStep computerMove;                       //电脑走子步法
    QList<UnDoChessStep> steps;                   //存储每局游戏所有的步法

    int num;                                    //线程计数
    bool flag;                                  //移动棋子后的标志
    bool compThingking;                         //电脑是否在正在思考
    int stepCount;                              //所走步法总数
};

#endif // CHESSGAME_H
