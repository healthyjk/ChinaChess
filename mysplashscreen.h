#ifndef __MYSPLASHSCREEN_H
#define __MYSPLASHSCREEN_H

#include <QSplashScreen>
#include <QPixmap>
#include <QProgressBar>

class MySplashScreen: public QSplashScreen
{
        Q_OBJECT
public:
        MySplashScreen(const QPixmap& pixmap);
        ~MySplashScreen();
        void setProgress(int value);    //外部改变进度
private slots:
        void progressChanged(int);      //进度改变后重绘函数
private:
        QProgressBar *ProgressBar;
};

#endif // __MYSPLASHSCREEN_H
