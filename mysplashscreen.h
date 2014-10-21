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
        void setProgress(int value);    //�ⲿ�ı����
private slots:
        void progressChanged(int);      //���ȸı���ػ溯��
private:
        QProgressBar *ProgressBar;
};

#endif // __MYSPLASHSCREEN_H
