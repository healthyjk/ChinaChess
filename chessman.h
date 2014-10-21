#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QWidget>
#include <QPainter>
#include "chessdefine.h"


class ChessMan : public QWidget
{
    Q_OBJECT
public:
    explicit ChessMan(int type,QPoint pos,int id,bool killed = false,
                      bool selected = false,QWidget *parent = 0);

//以下变量在主类中使用频繁，固定义为public型
    int type;            // 棋子的类型
    bool killed;         //棋子是否被吃的标志
    QPoint position;     //棋子的位置
    bool selected;       //棋子被选择的标志
    int chessManId;      //棋子Id （0-31）
    QPixmap pixmap;      //取棋子图片的子类

protected:
    void paintEvent(QPaintEvent *);     //绘制棋子函数

};

#endif // CHESSMAN_H
