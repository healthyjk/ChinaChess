#include "mysplashscreen.h"

//构造函数
MySplashScreen::MySplashScreen(const QPixmap& pixmap) : QSplashScreen(pixmap)
{
    ProgressBar = new QProgressBar(this);        // 父类为MySplashScreen
    ProgressBar->setGeometry(0,576-32,1024,32);  // 显示位置
    ProgressBar->setRange(0, 100);               // 显示进度范围
    ProgressBar->setValue(0);                    // 进度初值
    connect(ProgressBar, SIGNAL(valueChanged(int)), this, SLOT(progressChanged(int))); //值改变时，立刻repaint
}

//析构函数
MySplashScreen::~MySplashScreen()
{
}

//设置进度函数
void MySplashScreen::setProgress(int value)
{
        ProgressBar->setValue(value);
}

//进度改变后重绘函数
void MySplashScreen::progressChanged(int)
{
    repaint();
}
