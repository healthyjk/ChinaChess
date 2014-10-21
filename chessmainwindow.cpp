#include "chessmainwindow.h"
#include <QtGui>

//���캯��
ChessMainWindow::ChessMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(QSize(1060,700));
    this->setFixedSize(1060,700);       //�̶����ڴ�С����������
    setWindowTitle(tr("�й�����"));
    createMenus();                      //�����˵�
    chessGame = new ChessGame(this);
    this->setCentralWidget(chessGame);  //��������Ϸ��Ϊ���봰�ڲ���
}

//��������
ChessMainWindow::~ChessMainWindow()
{
}

//�����˵�
void ChessMainWindow::createMenus()
{
    gameMenu = menuBar()->addMenu(tr("��Ϸ"));

    newDoubleAct = new QAction(tr("˫�˶�ս"), this);
    gameMenu->addAction(newDoubleAct);
    connect(newDoubleAct, SIGNAL(triggered()),this, SLOT(newMMGame()));

    newManComputerAct = new QAction(tr("�˻���ս"), this);
    gameMenu->addAction(newManComputerAct);
    connect(newManComputerAct, SIGNAL(triggered()),this, SLOT(newMPGame()));

    gameMenu->addSeparator();       //��ӷָ���

    configAct = new QAction(tr("ѡ��..."), this);
    gameMenu->addAction(configAct);
    connect(configAct, SIGNAL(triggered()),this, SLOT(configuration()));

    gameMenu->addSeparator();

    saveAct = new QAction(tr("&��������"), this);
    gameMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),
            this, SLOT(saveChess()));

    loadAct = new QAction(tr("&��������"), this);
    gameMenu->addAction(loadAct);
    connect(loadAct, SIGNAL(triggered()),
            this, SLOT(loadChess()));


    exitAct = new QAction(tr("&�˳�"), this);
    gameMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),
            this, SLOT(close()));

    gameMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("����"));

    versionAct = new QAction(tr("����"), this);
    helpMenu->addAction(versionAct);
    connect(versionAct, SIGNAL(triggered()),
            this, SLOT(help()));
}

//�µ�˫�˶�ս
void ChessMainWindow::newMMGame()
{
    chessGame->newMMGame();
}

//�µ��˻���ս
void ChessMainWindow::newMPGame()
{
    chessGame->newMPGame();
}

//ѡ��˵�����
void ChessMainWindow::configuration()
{
    chessGame->configuration();
}

//������ֲ˵�
void ChessMainWindow::saveChess()
{
    QString fileName =      // ��һ���ļ��Ի���,��ȡ�����ļ�����
            QFileDialog::
            getSaveFileName(this, tr("Save File"),
                            "chess1",
                            tr("files (*.*)"));
    chessGame->saveChess(fileName);
}

//��������
void ChessMainWindow::loadChess()
{
    QString fileName =
            QFileDialog::getOpenFileName
            (this, tr("Load File"),
             "chess1",
             tr("files (*.*)"));
    chessGame->loadChess(fileName);
}

//�����˵�����
void ChessMainWindow::help()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("help.pdf"));
}
