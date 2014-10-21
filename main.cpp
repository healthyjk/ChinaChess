#include <QtGui/QApplication>
#include <QTextCodec>
#include <QFont>
#include "chessmainwindow.h"
#include "mysplashscreen.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // QApplication::addLibraryPath("./plugins");
    //使程序能支持中文显示和处理
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    //添加应用程序图标
    QApplication::addLibraryPath("./images");
    a.setWindowIcon(QIcon("./images/mychess.ico"));


    //    QTextCodec *gbk = QTextCodec::codecForName("gb18030");
    //    QTextCodec::setCodecForTr(gbk);
    //    QTextCodec::setCodecForLocale(gbk);
    //    QTextCodec::setCodecForCStrings(gbk);
    //添加应用程序启动界面
    //    QSplashScreen *splash = new QSplashScreen;
    //    splash->setPixmap(QPixmap("./images/start.png"));
    //    splash->show();
    //    //让对话框延迟一段时间显示
    //    for(int i=0;i<2000;i++){
    //        splash->repaint();
    //    }
    MySplashScreen *splash = new MySplashScreen(QPixmap("./images/start.png"));

    splash->show(); // 显示
    splash->setGeometry(128,72,800,576);
    splash->move ((QApplication::desktop()->width() - splash->width())/2,
                  (QApplication::desktop()->height() - splash->height())/2);
    splash->setProgress(30);// 显示%30
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
    //象棋主界面
    ChessMainWindow w;
    w.show();
    //将窗口移动到屏幕的中央
    w.move ((QApplication::desktop()->width() - w.width())/2,0);//(QApplication::desktop()->height() - w.height())/2);
    splash->finish(&w); // 消失
    delete splash;
    return a.exec();
}
