#include "chessmainwindow.h"
#include <QtGui>

//构造函数
ChessMainWindow::ChessMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(QSize(1060,700));
    this->setFixedSize(1060,700);       //固定窗口大小，不可拉伸
    setWindowTitle(tr("中国象棋"));
    createMenus();                      //创建菜单
    chessGame = new ChessGame(this);
    this->setCentralWidget(chessGame);  //设置主游戏类为中央窗口部件
}

//析构函数
ChessMainWindow::~ChessMainWindow()
{
}

//创建菜单
void ChessMainWindow::createMenus()
{
    gameMenu = menuBar()->addMenu(tr("游戏"));

    newDoubleAct = new QAction(tr("双人对战"), this);
    gameMenu->addAction(newDoubleAct);
    connect(newDoubleAct, SIGNAL(triggered()),this, SLOT(newMMGame()));

    newManComputerAct = new QAction(tr("人机对战"), this);
    gameMenu->addAction(newManComputerAct);
    connect(newManComputerAct, SIGNAL(triggered()),this, SLOT(newMPGame()));

    gameMenu->addSeparator();       //添加分隔符

    configAct = new QAction(tr("选项..."), this);
    gameMenu->addAction(configAct);
    connect(configAct, SIGNAL(triggered()),this, SLOT(configuration()));

    gameMenu->addSeparator();

    saveAct = new QAction(tr("&保存棋盘"), this);
    gameMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),
            this, SLOT(saveChess()));

    loadAct = new QAction(tr("&载入棋盘"), this);
    gameMenu->addAction(loadAct);
    connect(loadAct, SIGNAL(triggered()),
            this, SLOT(loadChess()));


    exitAct = new QAction(tr("&退出"), this);
    gameMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),
            this, SLOT(close()));

    gameMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("帮助"));

    versionAct = new QAction(tr("关于"), this);
    helpMenu->addAction(versionAct);
    connect(versionAct, SIGNAL(triggered()),
            this, SLOT(help()));
}

//新的双人对战
void ChessMainWindow::newMMGame()
{
    chessGame->newMMGame();
}

//新的人机对战
void ChessMainWindow::newMPGame()
{
    chessGame->newMPGame();
}

//选项菜单操作
void ChessMainWindow::configuration()
{
    chessGame->configuration();
}

//保存棋局菜单
void ChessMainWindow::saveChess()
{
    QString fileName =      // 打开一个文件对话框,获取保存文件名字
            QFileDialog::
            getSaveFileName(this, tr("Save File"),
                            "chess1",
                            tr("files (*.*)"));
    chessGame->saveChess(fileName);
}

//载入棋盘
void ChessMainWindow::loadChess()
{
    QString fileName =
            QFileDialog::getOpenFileName
            (this, tr("Load File"),
             "chess1",
             tr("files (*.*)"));
    chessGame->loadChess(fileName);
}

//帮助菜单操作
void ChessMainWindow::help()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("help.pdf"));
}
