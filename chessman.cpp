#include "chessman.h"

//棋子图片名称数组
QString picNames[16] = {"B_KING.png","B_CAR.png","B_HORSE.png","B_CANON.png","B_BISHOP.png",
                        "B_ELEPHANT.png","B_PAWN.png","R_KING.png","R_CAR.png","R_HORSE.png",
                        "R_CANON.png","R_BISHOP.png","R_ELEPHANT.png","R_PAWN.png"};

ChessMan::ChessMan(int type,QPoint position,int chessManId, bool killed,bool selected, QWidget *parent) :
    QWidget(parent)
{
    this->type = type;
    this->position = position;
    this->killed = killed;
    this->selected = selected;
    this->chessManId = chessManId;
    //根基棋子坐标设置棋子显示位置
    this->setGeometry(QRect(this->position.rx()* BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_WIDTH / 2,
                            this->position.ry()*BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT / 2,CMAN_WIDTH,CMAN_WIDTH));
}

void ChessMan::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(!this->killed)       //如果棋子没被杀死
    {
        if(selected){       //如果棋子被选中，画上绿色边框
            this->pixmap.load("./images/selected.png");
            painter.drawPixmap(0,0,50,50,pixmap);
        }
        this->pixmap.load(QString("./images/")+ picNames[this->type - 1]);
        painter.drawPixmap(0,0,50,50,pixmap);
    }
}

