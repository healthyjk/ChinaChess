#include "searchengine.h"

SearchEngine::SearchEngine()
{
}

SearchEngine::~SearchEngine()
{
    delete chessMoveGenerator;
    delete chessEvaluation;
}

//void setThinkProgress(CProgress* p){progress = p;}

//�ж��Ƿ��ѷ�ʤ��
int SearchEngine::isGameOver(const int board[10][9],int depth)
{
    int i,j,chessManType;
    bool redAlive = false;
    bool blackAlive = false;

    //���췽�Ź��Ƿ���˧
    for(i=7;i<10;i++)
        for(j=3;j<6;j++)
        {
            chessManType = idToType(board[i][j]);
            if(chessManType == B_KING)
                blackAlive = true;
            if(chessManType == R_KING)
                redAlive = true;
        }

    //���ڷ��Ź��Ƿ��н�
    for(i=0;i<3;i++)
        for(j=3;j<6;j++)
        {
            chessManType = idToType(board[i][j]);
            if(chessManType == B_KING)
                blackAlive = true;
            if(chessManType == R_KING)
                redAlive = true;
        }
    QPoint R_KING_POS,B_KING_POS;
    for(int i = 0;i < 10;i++)
        for(int j = 3;j < 6;j++)
            if(board[i][j] == B_KING_ID){
                B_KING_POS.setX(j);
                B_KING_POS.setY(i);
            }
            else if(board[i][j] == R_KING_ID){
                R_KING_POS.setX(j);
                R_KING_POS.setY(i);
            }


    i = (this->maxDepth - depth + 1) % 2;//ȡ��ǰ��ż��־,������Ϊ���Է�,ż����Ϊ�û���
    //�췽����

    if(!redAlive){
        if(i)
            return 19990 + depth; //�����㷵�ؼ���ֵ
        else
            return -19990 - depth;//ż���㷵�ؼ�Сֵ
    }

    //�ڷ�����
    if(!blackAlive){
        if(i)
            return -19990 - depth;//�����㷵�ؼ�Сֵ
        else
            return 19990 + depth; //ż���㷵�ؼ���ֵ
    }

    return 0;//��˧���ڣ�����0

}

 //����ĳһ�߷���������֮�������
int SearchEngine::stepMove(ChessStep* move)
{
    int chessManId;
    chessManId = currentBoard[move->to.y()][move->to.x()];          //ȡĿ��λ������
    //�������ƶ���Ŀ��λ��
    currentBoard[move->to.y()][move->to.x()] = currentBoard[move->from.y()][move->from.x()];
    currentBoard[move->from.y()][move->from.x()] = -1;//��ԭλ�����
    return chessManId;                  //���ر��Ե�������
}

 //�ָ�Ϊ�߹�֮ǰ������
void SearchEngine::undoStepMove(ChessStep* move,int chessManId)
{
    currentBoard[move->from.y()][move->from.x()] = currentBoard[move->to.y()][move->to.x()];
    currentBoard[move->to.y()][move->to.x()] = chessManId;//��ԭλ�����
}



