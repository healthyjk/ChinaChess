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
    void configuration();//ѡ��˵�����
    void saveChess();   //������ֲ˵�
    void newMMGame();   //�µ�˫�˶�ս
    void newMPGame();   //�µ��˻���ս
    void loadChess();   //��������
    void help();        //�����˵�����
    
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
