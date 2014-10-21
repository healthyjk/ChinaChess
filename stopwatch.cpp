#include "stopwatch.h"
#include <QtGui>

StopWatch::StopWatch(QWidget *parent):time(0,0,0)
{
    setFixedSize(90,50);
    lcdNumber = new QLCDNumber(this);
    dispTime();
    timer=new QTimer(this);
}

//��ʾʱ�亯��
void StopWatch::dispTime()
{
    QString text=time.toString("mm:ss");
    lcdNumber->display(text);
}

//����ʱ�亯��
void StopWatch::resetTime()
{
    time.setHMS(0,0,0);
    dispTime();
    timer->stop();
}

//��ʼ��ʱ����
void StopWatch::startTime()
{
    timer->start(1000);
}

//ֹͣʱ�亯��
void StopWatch::stopTime()
{
    timer->stop();
}

//ÿ1000msʱ���1s
void StopWatch::addTime()
{
    time=time.addSecs(1);
    dispTime();
}



