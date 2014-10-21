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



//�������������߳���
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
    NegaScout_TT_HH_Engine *engine;     //��������
    ChessStep bestMove;                 //��Ѳ���
    int compLevel;                      //���Եȼ�
};


//�������ֲ����߳���
class groundMusic: public QThread
{
    Q_OBJECT
public:
    groundMusic();
    ~groundMusic();
    void stop();        //ֹͣ��������
public slots:
    void MusicOn();     //�ظ����ű������ֲۺ���
protected:
    void run();
private:
    Phonon::MediaObject *mediaObject;       //��ý����ʵ����
};

//����Ϸ��
class ChessGame : public QWidget
{
    Q_OBJECT
public:
    explicit ChessGame(QWidget *parent = 0);
    enum ChessState{DRAW,RED_WIN,BLACK_WIN,LAST};   //��Ϸ״̬���ֱ�Ϊ�����壬����ʤ������ʤ�����ڽ�����
    void initChessBoard();                          //��ʼ������
    void initInterface();                           //��ʼ������
    void initParameter();                           //��ʼ����Ϸ����(��սģʽ�����Եȼ�,music)
    void playBgMusic();                             //���ű�������
    ChessState isGameOver();                        //�ж���Ϸ�Ƿ����
    bool moveChessMan(QPoint& from,QPoint& to);     //�ƶ����Ӻ���
    void printChessStep(int chessManId,QPoint& from,QPoint& to);    //��ӡ����׼�岽
    void recordSteps(QPoint &from, QPoint &to);                     //��¼���岽��
    void stopGame(ChessState state);                                //ֹͣ��Ϸ
    void computerThink();                           //���Կ����߳�˼������
    void dispTime();                                //��ʾʱ��
    void resetTime();                               //����ʱ��
    void startTime();                               //��ʼ��ʱ
    void stopTime();                                //ֹͣ��ʱ
    void saveChess(QString fileName);               //���浱ǰ���
    void loadChess(QString fileName);               //������ʷ��������
    void newMMGame();                               //�µ�˫�˶�ս
    void newMPGame();                               //�µ��˻���ս
    void backone();                                 //����һ������

public slots:
    void setCustomConfig(const int chessMode,const int compuLevel,
                         const bool bgMusicSwitch,const bool atMusicSwitch);        //���û��Զ���ѡ�����ò�������
    void setDefaultConfig();                                                        //��Ĭ��ѡ�����ò���
    void configuration();                                                           //ѡ�����õ����˵�����
    void computerRun(const int x1,const int y1,const int x2,const int y2);         //��������巽��
    void newGame();                                                                 //����Ϸ����
    void back();                                                                    //���尴ť�¼�
    void modeChange();                                                              //ģʽ�л���ť�¼�
    void surrender();                                                               //���䰴ť�¼�
    void addTime();                                                                 //��ʱʱ��ÿ���1����

protected:
    void mousePressEvent(QMouseEvent *);//����������¼�
    void paintEvent(QPaintEvent *);     //����������ط���

private:
    QLabel *user1;                      //�û�1�������
    QLabel *user2;                      //�û�2�������
    QTextBrowser *moveHistory;          //��ʾ��ʷ�����ı���
    QPushButton *undoButton;            //���尴ť
    QPushButton *modeChangeButton;      //ģʽ�л���ť
    QPushButton *surrenderButton;       //���䰴ť
    QPushButton *tryAgainButton;        //���¿�ʼ��Ϸ��ť

    QLCDNumber *lcdNumberBlack;         //�ڷ�ʱ����ʾҺ����
    QLCDNumber *lcdNumberRed;           //�췽ʱ����ʾҺ����
    QTimer *timer;                      //��Ϸ��ʱ��
    QTime timeBlack;                    //�ڷ�����ʱ��
    QTime timeRed;                      //�췽����ʱ��

    ConfigDialog *configDialog;         //���öԻ���

    NegaScout_TT_HH_Engine* engine;     //��������
    ChessMoveGenerator* moveGenerator;  //�߷�������
    ChessEvaluation* evaluator;         //����������

    int chessBoard[10][9];                          //ʹ��10x9�Ķ�ά�������洢����,�����д������id
    ChessMan *chessMan[32];                         //�洢32������
    int selecteid;                                  //�Ѿ�ѡ�����ӵ�id
    int turn;                                       //��һ����������ķ���0��1��

    int chessMode;          //��Ϸģʽ
    int compLevel;          //���Եȼ�
    bool bgMusicSwitch;     //�������ֿ���
    bool atMusicSwitch;     //���嶯����Ч����
    int chumen,hanmen;      //˫�������������

    QPoint lastMoveF,lastMoveT;                   //��һ���������յ�
    ComputerThread* computerThread[500];          //���������߳�
    ChessStep computerMove;                       //�������Ӳ���
    QList<UnDoChessStep> steps;                   //�洢ÿ����Ϸ���еĲ���

    int num;                                    //�̼߳���
    bool flag;                                  //�ƶ����Ӻ�ı�־
    bool compThingking;                         //�����Ƿ�������˼��
    int stepCount;                              //���߲�������
};

#endif // CHESSGAME_H
