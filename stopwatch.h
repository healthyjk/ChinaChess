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
    void dispTime();        //显示时间函数
    void resetTime();       //重置时间函数
    void startTime();       //开始计时函数
    void stopTime();        //停止时间函数

public slots:
    void addTime();         //每1000ms时间加1s
private:
    QTimer *timer;
    QTime time;
    QLCDNumber *lcdNumber;
};

#endif // STOPWATCH_H
