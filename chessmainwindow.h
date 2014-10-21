#ifndef CHESSMAINWINDOW_H
#define CHESSMAINWINDOW_H

#include <QMainWindow>
#include "chessgame.h"

class QMenu;
class QAction;
class QPalette;
class QLineEdit;
class QPushButton;
class QLabel;

class ChessMainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit ChessMainWindow(QWidget *parent = 0);
    ~ChessMainWindow();
    void createMenus();

public slots:
    void configuration();//选项菜单操作
    void saveChess();   //保存棋局菜单
    void newMMGame();   //新的双人对战
    void newMPGame();   //新的人机对战
    void loadChess();   //载入棋盘
    void help();        //帮助菜单操作
    
private:
    ChessGame *chessGame;
    QMenu *gameMenu;
    QMenu *helpMenu;
    QAction *newDoubleAct;
    QAction *newManComputerAct;
    QAction *configAct;
    QAction *saveAct;
    QAction *loadAct;
    QAction *exitAct;
    QAction *versionAct;

};

#endif // CHESSMAINWINDOW_H
