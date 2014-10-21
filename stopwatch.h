#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QtGui>
#include <QWidget>
#include <QLCDNumber>

class StopWatch : public QWidget
{
    Q_OBJECT
public:
    explicit StopWatch(QWidget *parent = 0);
    void dispTime();        //��ʾʱ�亯��
    void resetTime();       //����ʱ�亯��
    void startTime();       //��ʼ��ʱ����
    void stopTime();        //ֹͣʱ�亯��

public slots:
    void addTime();         //ÿ1000msʱ���1s
private:
    QTimer *timer;
    QTime time;
    QLCDNumber *lcdNumber;
};

#endif // STOPWATCH_H
