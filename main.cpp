#include <QtGui/QApplication>
#include <QTextCodec>
#include <QFont>
#include "chessmainwindow.h"
#include "mysplashscreen.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // QApplication::addLibraryPath("./plugins");
    //ʹ������֧��������ʾ�ʹ���
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    //���Ӧ�ó���ͼ��
    QApplication::addLibraryPath("./images");
    a.setWindowIcon(QIcon("./images/mychess.ico"));


    //    QTextCodec *gbk = QTextCodec::codecForName("gb18030");
    //    QTextCodec::setCodecForTr(gbk);
    //    QTextCodec::setCodecForLocale(gbk);
    //    QTextCodec::setCodecForCStrings(gbk);
    //���Ӧ�ó�����������
    //    QSplashScreen *splash = new QSplashScreen;
    //    splash->setPixmap(QPixmap("./images/start.png"));
    //    splash->show();
    //    //�öԻ����ӳ�һ��ʱ����ʾ
    //    for(int i=0;i<2000;i++){
    //        splash->repaint();
    //    }
    MySplashScreen *splash = new MySplashScreen(QPixmap("./images/start.png"));

    splash->show(); // ��ʾ
    splash->setGeometry(128,72,800,576);
    splash->move ((QApplication::desktop()->width() - splash->width())/2,
                  (QApplication::desktop()->height() - splash->height())/2);
    splash->setProgress(30);// ��ʾ%30
    for(int i=0;i<60;i++){
        splash->repaint();
    }
    splash->setProgress(60);
    for(int i=0;i<60;i++){
        splash->repaint();
    }
    splash->setProgress(90);
    for(int i=0;i<60;i++){
        splash->repaint();
    }
    //����������
    ChessMainWindow w;
    w.show();
    //�������ƶ�����Ļ������
    w.move ((QApplication::desktop()->width() - w.width())/2,0);//(QApplication::desktop()->height() - w.height())/2);
    splash->finish(&w); // ��ʧ
    delete splash;
    return a.exec();
}
