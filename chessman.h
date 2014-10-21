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

//���±�����������ʹ��Ƶ�����̶���Ϊpublic��
    int type;            // ���ӵ�����
    bool killed;         //�����Ƿ񱻳Եı�־
    QPoint position;     //���ӵ�λ��
    bool selected;       //���ӱ�ѡ��ı�־
    int chessManId;      //����Id ��0-31��
    QPixmap pixmap;      //ȡ����ͼƬ������

protected:
    void paintEvent(QPaintEvent *);     //�������Ӻ���

};

#endif // CHESSMAN_H
