#include "mysplashscreen.h"

//���캯��
MySplashScreen::MySplashScreen(const QPixmap& pixmap) : QSplashScreen(pixmap)
{
    ProgressBar = new QProgressBar(this);        // ����ΪMySplashScreen
    ProgressBar->setGeometry(0,576-32,1024,32);  // ��ʾλ��
    ProgressBar->setRange(0, 100);               // ��ʾ���ȷ�Χ
    ProgressBar->setValue(0);                    // ���ȳ�ֵ
    connect(ProgressBar, SIGNAL(valueChanged(int)), this, SLOT(progressChanged(int))); //ֵ�ı�ʱ������repaint
}

//��������
MySplashScreen::~MySplashScreen()
{
}

//���ý��Ⱥ���
void MySplashScreen::setProgress(int value)
{
        ProgressBar->setValue(value);
}

//���ȸı���ػ溯��
void MySplashScreen::progressChanged(int)
{
    repaint();
}
