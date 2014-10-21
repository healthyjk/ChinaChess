#include "stopwatch.h"
#include <QtGui>

StopWatch::StopWatch(QWidget *parent):time(0,0,0)
{
    setFixedSize(90,50);
    lcdNumber = new QLCDNumber(this);
    dispTime();
    timer=new QTimer(this);
}

//显示时间函数
void StopWatch::dispTime()
{
    QString text=time.toString("mm:ss");
    lcdNumber->display(text);
}

//重置时间函数
void StopWatch::resetTime()
{
    time.setHMS(0,0,0);
    dispTime();
    timer->stop();
}

//开始计时函数
void StopWatch::startTime()
{
    timer->start(1000);
}

//停止时间函数
void StopWatch::stopTime()
{
    timer->stop();
}

//每1000ms时间加1s
void StopWatch::addTime()
{
    time=time.addSecs(1);
    dispTime();
}



