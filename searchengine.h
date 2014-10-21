#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <chessdefine.h>
#include <chessevaluation.h>
#include <chessmovegenerator.h>
#include <string.h>

//��������Ļ���
class SearchEngine
{
public:
    SearchEngine();
    virtual ~SearchEngine();

public:
    virtual void searchOneGoodMove(const int board[10][9]) = 0;     //��������岽
    int isGameOver(const int board[10][9],int depth);
    //����ĳһ�߷���������֮�������
    int stepMove(ChessStep* move);
    //�ָ�Ϊ�߹�֮ǰ������
    void undoStepMove(ChessStep* move,int chessManId);
    ChessStep getBestMove(){return bestMove;}          //�õ�����߷�
    void setSearchDepth(int depth){this->searchDepth = depth;}//�趨����������
    void setEvaluator(ChessEvaluation* chessEvaluation){this->chessEvaluation=chessEvaluation;}  //�趨��ֵ����
    void setMoveGenerator(ChessMoveGenerator* chessMoveGenerator){this->chessMoveGenerator =chessMoveGenerator;}//�趨�߷�������
    ChessMoveGenerator* chessMoveGenerator;          //�߷�������

protected:
    int currentBoard[10][9];                 //����ʱ���ڼ�¼��ǰ�ڵ�����״̬������
    ChessStep bestMove;                      //��¼����߷�
    UnDoChessStep undoChessStep;
    ChessEvaluation* chessEvaluation;        //��ֵ����
    int searchDepth;                         //����������
    int maxDepth;                            //��ǰ����������������
};

#endif
