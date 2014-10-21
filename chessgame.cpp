#include "chessgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QSound>
#include <string>
#include <QPixmap>
#include <QDebug>

//����Ϸ����ʹ�õ��ĳ���
QPoint initPos(BOARD_LEFT_MARGIN,BOARD_TOP_MARGIN);     //���̵�һ����㣨0,0�����ڵ�λ��
std::string chesssManNames[14]={"��","��","��","��","ʿ","��","��","˧","��","��","��","��","��","��"};   //˫����������
std::string redStepXName[9] = {"��","��","��","��","��","��","��","��","һ"};            //�췽����X����
std::string redStepYName[9] = {"һ","��","��","��","��","��","��","��","��"};            //�췽����Y����
QString chuMens[9] = {"xiangyu","fanzeng","xiangzhuang","yuziqi","yingbu","jibu","zhonglimei","longqie","yuji"};
QString hanMens[9] = {"liubang","hanxin","xiaohe","zhangliang","fankuai","chenping","caocan","pengyue","lvzhi"};
std::string chuMenIntro[9] = {"����������������,���¹Ž���˫����Ի������ɽ��������",
                              "�������������������ıʿ������Ϊ���Ƿ򡱡�������ʮ���ؾӼң�����ơ�",
                              "��ׯ����������������õܣ���Ϊ�������µ��佫һֱ׷������������ս",
                              "�����ڣ����������������֮һ������������º�һֱ׷��ս�������",
                              "Ӣ������ĩ�����������������ɱ�ǭ��������������Ϊ�����������֮һ",
                              "���������������������������֮һ���ô򱧲�ƽ�������س�ŵ����",
                              "����������������һԱ���ճ�Ⱥ���Ͻ���������������������ʱ�������Գ��ش��",
                              "���ң������������֮һ���ٰ���������˾��",
                              "�ݼ�:����֮��ʱ�ڡ���������������İ�������,����������������һֱ������������,�����Ծ�"};
std::string hanMenIntro[9] = {"�������������ֺŸ߻ʵۡ�����Լ�����£���ɱ�����������˼��������",
                              "���ţ���������֮һ����������֮����ս��ʤ������ȡ���᲻�纫�š���",
                              "���Σ���������֮һ����ʷ�ǡ��أ�����������ߣ������޺���Ϊ������Ե����",
                              "��������������֮һ����������Ի�����˳���֮�У���ʤǧ��֮�⣬�᲻���ӷ�����",
                              "���ࣺ�����󽫾�����ة�ࡣΪ�����������һ�ĸ�������������ĵڶ��ͽ�",
                              "��ƽ�����������Ŀ�������֮һ��������ơ��������ơ����������֡�",
                              "�ܲΣ����������Ŀ�������֮һ����������������������أ�����ս",
                              "��Խ�����������Ŀ�������֮һ����ĩ�۱����壬���ʾ���˳�����κ��������ɺ�",
                              "���������߻ʺ��й���ʷ�ϵ�һλ�ʺ󣬵�һ���ٳ����Ƶ�Ů�ӣ����պ�����Ȩ��ʮ��"};

//�����̹߳������
ChessStep lastStep;  //���Ե���һ��˼�����

//����Ϸ�๹�캯��
ChessGame::ChessGame(QWidget *parent) :
    QWidget(parent),timeRed(0,0,0),timeBlack(0,0,0)
{
    this->setGeometry(0,0,1060,700);
    this->setAutoFillBackground(true);
    QPixmap pixmap("./images/bg.png");
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));     //���ô��ڱ���
    this->setPalette(palette);

    initInterface();        //��ʼ������
    initChessBoard();       //��ʼ������

    timer=new QTimer(this);    //��ʼ����ʱ��
    connect(timer,SIGNAL(timeout()),this,SLOT(addTime()));

    //��ʼ��AI����
    this->moveGenerator = new ChessMoveGenerator();
    this->moveGenerator->moveListCount = 0;
    this->evaluator = new ChessEvaluation();
    this->engine = new NegaScout_TT_HH_Engine();
    engine->setMoveGenerator(moveGenerator);
    engine->setEvaluator(evaluator);

    //��ս��ʽ
    chessMode = -1;
    initParameter();        //��ʼ������
    configuration();        //��ʼ������
}

//��ʼ������
void ChessGame::initInterface()
{
    user1 = new QLabel(this);       //�û�1����
    user1->setGeometry(QRect(20,180,150,65));
    user1->setFrameShape(QFrame::Box);
    user1->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(0, 0, 0);");
    user1->setWordWrap(true);
    user1->setAlignment(Qt::AlignTop);

    user2 = new QLabel(this);       //�û�2����
    user2->setFrameShape(QFrame::Box);
    user2->setGeometry(QRect(20,570,150,65));
    user2->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(255, 0, 0);");
    user2->setWordWrap(true);
    user2->setAlignment(Qt::AlignTop);

    moveHistory = new QTextBrowser(this);       //������ʷ�ı���
    moveHistory->setGeometry(QRect(830,75,270,250));
    moveHistory->raise();
    moveHistory->setFontFamily("΢���ź�");
    moveHistory->setFontPointSize(12);
    QPalette mH = moveHistory->palette();
    mH.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    moveHistory->setPalette(mH);

    undoButton = new QPushButton(this);         //���尴ť
    connect(undoButton,SIGNAL(clicked()),this,SLOT(back()));
    undoButton->setGeometry(QRect(850,400,150,50));
    undoButton->setStyleSheet("background-image:url(./images/undo.png);width:150px;height:50px;");

    modeChangeButton = new QPushButton(this);       //ģʽ�л���ť
    modeChangeButton->setGeometry(QRect(850,470,150,50));
    connect(modeChangeButton,SIGNAL(clicked()),this,SLOT(modeChange()));
    modeChangeButton->setStyleSheet("background-image:url(./images/qiehuan.png);width:150px;height:50px;");

    surrenderButton = new QPushButton(this);        //���䰴ť
    surrenderButton->setGeometry(QRect(850,540,150,50));
    connect(surrenderButton,SIGNAL(clicked()),this,SLOT(surrender()));
    surrenderButton->setStyleSheet("background-image:url(./images/surrender.png);width:150px;height:50px;");

    tryAgainButton = new QPushButton(this);     //���¿�ʼ��ť
    tryAgainButton->setGeometry(QRect(850,610,150,50));
    connect(tryAgainButton,SIGNAL(clicked()),this,SLOT(newGame()));
    tryAgainButton->setStyleSheet("background-image:url(./images/newgame.png);width:150px;height:50px;");

    lcdNumberBlack = new QLCDNumber(this);      //ʱ����ʾҺ��
    lcdNumberRed = new QLCDNumber(this);
    lcdNumberBlack->setGeometry(80,290,80,35);
    lcdNumberRed->setGeometry(80,340,80,35);

}

//��ʼ������
void ChessGame::initChessBoard()
{
    const int initBoard[10][9] = {
        {B_CAR,B_HORSE,B_ELEPHANT,B_BISHOP,B_KING,B_BISHOP,B_ELEPHANT,B_HORSE,B_CAR},
        {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
        {NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS},
        {B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN},
        {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
        {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
        {R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN},
        {NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS},
        {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
        {R_CAR,R_HORSE,R_ELEPHANT,R_BISHOP,R_KING,R_BISHOP,R_ELEPHANT,R_HORSE,R_CAR}
    };
    memcpy(chessBoard, initBoard, sizeof(chessBoard));      //������chessBoard�����ڳ�ʼ������
    QPoint pos;
    for(int i = 0,k = 0;i < 10;i++)
        for(int j = 0;j < 9;j++)
        {
            if(chessBoard[i][j] != NOCHESS)
            {
                pos.setX(j);
                pos.setY(i);
                chessMan[k] = new ChessMan(chessBoard[i][j],pos,k,false,false,this);    //˳����32������
                chessBoard[i][j] = k;                       //������������Ϊ���ӣɣ�
                chessMan[k++]->show();
            }
            else
                chessBoard[i][j] = -1;
        }
}

//��ʼ����Ϸ����
void ChessGame::initParameter()
{
    selecteid = -1;
    turn = 1;
    stepCount = 0;
    flag = false;
    lastMoveF.setX(-1);     //��һ��Ϊ��
    lastMoveF.setY(-1);
    lastMoveT.setX(-1);
    lastMoveT.setY(-1);
    this->update();
    compThingking = false;  //���Բ���˼��
    chumen = rand()%9;
    hanmen = rand()%9;

}

//ѡ������
void ChessGame::configuration()
{
    configDialog = new ConfigDialog(this);
    configDialog->move ((QApplication::desktop()->width() - configDialog->width())/2,
                          (QApplication::desktop()->height() - configDialog->height())/2);
    connect(configDialog,SIGNAL(getAllConfig(int,int,bool,bool)),this,SLOT(setCustomConfig(int,int,bool,bool)));
    connect(configDialog,SIGNAL(getDefaultConfig()),this,SLOT(setDefaultConfig()));
    configDialog->exec();
}

//�û��Զ����������
void ChessGame::setCustomConfig(const int chessMode,const int compuLevel,const bool bgMusicSwitch,const bool atMusicSwitch)
{
    this->compLevel = compuLevel;
    this->bgMusicSwitch = bgMusicSwitch;
    this->atMusicSwitch = atMusicSwitch;
    engine->setSearchDepth(compLevel);
    if(this->chessMode != chessMode){       //����û��ı���ģʽ�������¿�ʼ��Ϸ���������¿�ʼ
        this->chessMode = chessMode;
        this->newGame();
    }
    playBgMusic();      //���ű�������
    startTime();        //������ʱ��
}

//Ĭ�ϲ�������
void ChessGame::setDefaultConfig()
{
    if(chessMode == -1)
    {
        this->chessMode = MAN_PCCHESS;
        this->compLevel = 5;
        this->bgMusicSwitch = true;
        this->atMusicSwitch = true;
        engine->setSearchDepth(compLevel);
        playBgMusic();
        startTime();
    }
}

void ChessGame::playBgMusic()       //���ű�������
{
    if(bgMusicSwitch){
        groundMusic groundsound;
        groundsound.start();
        groundsound.wait();
    }
}

//��������¼�����
void ChessGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;

    pixmap.load("./images/chess_board.png");
    painter.drawPixmap(200,10,605,658,pixmap);      //��������

    pixmap.load(QString("./images/")+chuMens[chumen]+QString(".jpg"));
    painter.drawPixmap(20,30,150,150,pixmap);       //���ȡ������ͼƬ����ʾͼƬ
    user1->setText(QString::fromLocal8Bit(chuMenIntro[chumen].c_str()));
    if(chessMode == MAN_PCCHESS)
        pixmap.load("./images/computer.jpg");       //������˻�ģʽ����������ͼƬ
    else
        pixmap.load("./images/B_KING.png");         //�����˫��ģʽ�������ڷ�������ͼƬ
    painter.drawPixmap(20,20,40,40,pixmap);         //�����췽��˧ͼƬ��

    pixmap.load(QString("./images/")+hanMens[hanmen]+QString(".jpg"));
    painter.drawPixmap(20,420,150,150,pixmap);
    user2->setText(QString::fromLocal8Bit(hanMenIntro[hanmen].c_str()));
    pixmap.load("./images/R_KING.png");
    painter.drawPixmap(20,420,40,40,pixmap);

    if(lastMoveF.x() != -1 && lastMoveF.y() != -1 && lastMoveT.x() != -1 && lastMoveT.y() != -1)    //������һ���˶��켣
    {
        pixmap.load("./images/selected.png");
        painter.drawPixmap(lastMoveT.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_WIDTH/2,
                           lastMoveT.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                           CMAN_WIDTH,CMAN_HEIGHT,pixmap);
        pixmap.load("./images/selected_last.png");
        painter.drawPixmap(lastMoveF.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_WIDTH/2,
                           lastMoveF.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                           CMAN_WIDTH,CMAN_HEIGHT,pixmap);
    }
}

//����¼���������
void ChessGame::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = (event->pos() - initPos) / BOARD_GAP;             //�������������λ��
    if(clickPos.x() < 0 || clickPos.x() > 8
            || clickPos.y() < 0 || clickPos.y() > 9)
        return;
    int chessManId = chessBoard[clickPos.y()][clickPos.x()];
    if(this->selecteid != -1) //����Ѿ�ѡ������
    {
        if(chessManId / 16 == this->turn && chessManId != -1)           //����Ǳ�������
        {
            if(chessManId != selecteid)
            {
                chessMan[this->selecteid]->selected = false;
                chessMan[this->selecteid]->update();
                this->selecteid = chessManId;
                chessMan[chessManId]->selected = true;
                chessMan[chessManId]->update();
                if(atMusicSwitch)
                    QSound::play("./sounds/click.wav");
            }
        }
        else                                                        //�ǿո���߶Է�����
        {
            if(selecteid / 16 == this->turn)
                flag = moveChessMan(chessMan[selecteid]->position,clickPos);
            if(flag && chessMode == MAN_PCCHESS)
            {
                computerThink();
            }
        }
    }
    else
    {
        if(chessManId != -1 && isGameOver() == LAST)
        {
            if(chessMan[chessManId]->type / 8 == this->turn && !compThingking)//������Ե������
            {
                lastMoveF.setX(-1);
                lastMoveF.setY(-1);
                lastMoveT.setX(-1);
                lastMoveT.setY(-1);
                this->update();
                this->selecteid = chessManId;
                chessMan[chessManId]->selected = true;
                chessMan[chessManId]->update();
                if(atMusicSwitch){
                    QSound atMusic("./sounds/CLICK.wav");
                    atMusic.play();
                }
            }
        }
    }
}

//���Կ����߳̽���˼������
void ChessGame::computerThink()
{
    compThingking = true;
    computerThread[num] = new ComputerThread(this->chessBoard,this->engine,compLevel);
    connect(computerThread[num],SIGNAL(computerMoveSignal(const int,const int,const int,const int)),
            this,SLOT(computerRun(const int,const int,const int,const int)));
    connect(computerThread[num],SIGNAL(terminated()),computerThread[num],SLOT(deleteLater()));//�߳̽���֮���Զ�ɾ���߳�
    computerThread[num]->start();       //������������
    num++;
    if(num==499)
        num = 0;
}

//�ƶ����ӵĺ���
bool ChessGame::moveChessMan(QPoint &from,QPoint &to)
{
    int fromId = chessBoard[from.y()][from.x()];
    int toId = chessBoard[to.y()][to.x()];
    flag = false;
    //�ƶ�����ǰ����������һ��״̬
    lastMoveF.setX(-1);
    lastMoveF.setY(-1);
    lastMoveT.setX(-1);
    lastMoveT.setY(-1);
    this->update();

    if(toId != -1)
    {
        if(canMoveToDst(chessBoard,from,to))
        {
            printChessStep(fromId,from,to);
            chessBoard[to.y()][to.x()]
                    = fromId;
            chessBoard[from.y()][from.x()]= -1;
            chessMan[fromId]->position.setX(to.x());
            chessMan[fromId]->position.setY(to.y());
            chessMan[toId]->killed = true;
            chessMan[toId]->hide();
            chessMan[fromId]->selected = false;
            chessMan[fromId]->setGeometry(to.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_HEIGHT/2,
                                          to.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                                          CMAN_WIDTH,CMAN_HEIGHT);
            this->selecteid = -1;

                if(atMusicSwitch)
                     QSound::play("./sounds/CAPTURE.wav");  //���ų�������
                if(this->turn)
                    turn = 0;
                else
                    turn = 1;
            flag = true;
            stopGame(isGameOver());
        }
    }
    else
    {
        if(canMoveToDst(chessBoard,from,to))      //�����������
        {
            printChessStep(fromId,from,to);

            chessBoard[to.y()][to.x()]          //��������
                    = fromId;
            chessBoard[from.y()][from.x()] = -1;
            chessMan[fromId]->position.setX(to.x());        //��������
            chessMan[fromId]->position.setY(to.y());
            chessMan[fromId]->selected = false;
            chessMan[fromId]->setGeometry(to.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_HEIGHT/2,
                                          to.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                                          CMAN_WIDTH,CMAN_HEIGHT);
            this->selecteid = -1;
            if(atMusicSwitch)                           //�����ƶ�����
                  QSound::play("./sounds/MOVE.wav");
            turn = turn == 0? 1 : 0;                             //�������巽

            flag = true;
            stopGame(isGameOver());
        }
    }
    if(flag){
        lastMoveF = from;
        lastMoveT = to;
        this->update();
    }
    return flag;
}

void ChessGame::stopGame(ChessState state)
{
    if(state == BLACK_WIN){
        stopTime();
        if(atMusicSwitch)
            QSound::play("./sounds/WIN.wav");
        int r = QMessageBox::question(NULL, "ʤ��", "<font color='black'>����Ӯ�ˣ�</font></br></br>����һ�֣�",
                              QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        if(r==QMessageBox::Yes){
            this->newGame();
            startTime();
        }
        flag = false;
    }
    else if(state == RED_WIN){
        stopTime();
        if(atMusicSwitch)
           QSound::play("./sounds/WIN.wav");
        int r = QMessageBox::question(NULL, "ʤ��", "<font color='red'>����Ӯ�ˣ�</font></br></br>����һ�֣�",
                              QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        if(r==QMessageBox::Yes){
            this->newGame();
            startTime();
        }
        flag = false;
    }
}

//����·��ת��Ϊ���岽����������
void ChessGame::printChessStep(int chessManId,QPoint& from,QPoint& to)
{
    recordSteps(from,to);
    stepCount++;
    QString step;
    QString chessManName = QString(QString::fromLocal8Bit(chesssManNames[idToType(chessManId) - 1].c_str()));
    QString stepFrom;
    QString stepTo;
    QString stepType;
    if(IsRed(chessManId))           //����Ǻ��ӣ�������Ϊ�ŵ�һ
    {
        stepFrom = QString::fromLocal8Bit(redStepXName[from.x()].c_str());
        if(from.y() == to.y())          //����Ǻ�����
        {

            stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
            stepType = QString("ƽ");
        }
        else if(from.x() == to.x())
        {
            if(from.y()>to.y())
            {
                stepTo = QString::fromLocal8Bit(redStepYName[from.y()-to.y()-1].c_str());
                stepType = QString("��");
            }
            else
            {
                stepTo = QString::fromLocal8Bit(redStepYName[to.y()-from.y()-1].c_str());
                stepType = QString("��");
            }
        }
        else
        {
            if(from.y()>to.y())
            {
                stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
                stepType = QString("��");
            }
            else
            {
                stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
                stepType = QString("��");
            }
        }
    }
    else      //����Ǻ��ӣ�������Ϊ1��9
    {
        stepFrom = QString::fromLocal8Bit(redStepXName[from.x()].c_str());
        stepFrom = QString::number(from.x()+1);
        if(from.y() == to.y())               //����Ǻ�����
        {
            stepTo = QString::number(to.x()+1);
            stepType = QString("ƽ");
        }
        else if(from.x() == to.x())
        {
            if(from.y()>to.y())
            {
                stepTo = QString::number(from.y() - to.y());
                stepType = QString("��");
            }
            else
            {
                stepTo = QString::number(to.y() - from.y());
                stepType = QString("��");
            }
        }
        else
        {
            if(from.y()>to.y())
            {
                stepTo = QString::number(to.x()+1);
                stepType = QString("��");
            }
            else
            {
                stepTo = QString::number(to.x()+1);
                stepType = QString("��");
            }
        }
    }
    step = chessManName + stepFrom + stepType + stepTo;
    if((stepCount % 2)){
        step = QString::number(stepCount/2 + 1) + QString(".") + step;
        moveHistory->append(step);
    }
    else{
        step = QString("         ") + step;
        moveHistory->insertPlainText(step);
    }
}

//��¼����
void ChessGame::recordSteps(QPoint& from, QPoint& to)
{
    UnDoChessStep one;
    int id = chessBoard[from.y()][from.x()];
    int killedId=chessBoard[to.y()][to.x()];
    one.killedChessManId = killedId;
    one.lastStep.from = from;
    one.lastStep.to = to;
    one.lastStep.movedId = id;
    steps.push_back(one);
}

void ChessGame::back()
{
    if(chessMode == MAN_PCCHESS)
    {
        backone();
        backone();
    }
    else
        backone();
}

//����һ��
void ChessGame::backone()
{
    if(!compThingking)
    {
        if(steps.size() == 0)
            return;
        if(this->selecteid != -1){
            chessMan[this->selecteid]->selected = 0;
            this->selecteid = -1;
        }
        //�������ǵ�˫����ս��Ҫ�󣬽������Ϊ1��
        UnDoChessStep one = steps[steps.size()-1];
        chessBoard[one.lastStep.from.y()][one.lastStep.from.x()] = one.lastStep.movedId;
        chessBoard[one.lastStep.to.y()][one.lastStep.to.x()] = one.killedChessManId;
        steps.pop_back();
        if(one.killedChessManId != -1)
        {
            chessMan[one.killedChessManId]->killed = false;
            chessMan[one.killedChessManId]->show();
        }
        chessMan[one.lastStep.movedId]->position = one.lastStep.from;
        chessMan[one.lastStep.movedId]->setGeometry(
                    one.lastStep.from.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_HEIGHT/2,
                    one.lastStep.from.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                    CMAN_WIDTH,CMAN_HEIGHT);
        turn = (turn == 0) ? 1 : 0;                             //�������巽
        lastMoveF = one.lastStep.from;
        lastMoveT = one.lastStep.to;
        this->update();
    }
}

//�ж���Ϸ�Ƿ����
ChessGame::ChessState ChessGame::isGameOver()
{
    if(chessMan[R_KING_ID]->killed)     //����췽˧����
        return BLACK_WIN;
    else if(chessMan[B_KING_ID]->killed) //����ڷ�������
        return RED_WIN;
    else if(chessMan[R_KING_ID]->position.x() == chessMan[B_KING_ID]->position.x()      //�����������
            && countChessMan(chessBoard,chessMan[R_KING_ID]->position,chessMan[B_KING_ID]->position)==0)
        return turn == 0? BLACK_WIN : RED_WIN;
    //�ݲ����������ж�
    return LAST;
}


//��ʼ�µĶԾ�
void ChessGame::newGame()
{
    for(int i = 0;i < 32;i++)
    {
        delete chessMan[i];
    }
    steps.clear();      //��մ洢�Ĳ���
    initChessBoard();
    initParameter();
    resetTime();
    moveHistory->append(QString("�µĶԾ�"));
    this->update();
}

void ChessGame::newMMGame()
{
    chessMode = MAN_MANCHESS;
    newGame();
}

void ChessGame::newMPGame()
{
    chessMode = MAN_PCCHESS;
    newGame();
}

void ChessGame::modeChange()
{
    if(chessMode == MAN_MANCHESS){  //�����ǰ��˫�˶�ս
        chessMode = MAN_PCCHESS;   //��Ϊ�˻���ս
        if(this->turn == 0)      //����Ǻ�����
            computerThink();     //������һ��
    }
    else
        chessMode = MAN_MANCHESS;
    this->update();
}

void ChessGame::surrender()
{
    if(turn)
        stopGame(BLACK_WIN);
    else
        stopGame(RED_WIN);
}

//����˼���̹߳��캯��
ComputerThread::ComputerThread(int chessBoard[10][9], NegaScout_TT_HH_Engine *engine,int compLevel)
{
    this->engine = engine;
    this->compLevel = compLevel;
    memcpy(board,chessBoard,360);
}

//����˼���߳���������
ComputerThread::~ComputerThread()
{
    delete engine;
}

//����˼���߳�
void ComputerThread::run()
{
    engine->searchOneGoodMove(this->board);
    bestMove = engine->getBestMove();       //�õ���ѵĲ���
    if(bestMove.from == lastStep.from && bestMove.to == lastStep.to)
    {
        bestMove = engine->chessMoveGenerator->moveList[compLevel][rand() %
                engine->chessMoveGenerator->moveListCount];
    }
    lastStep = bestMove;
    emit computerMoveSignal(bestMove.from.x(),bestMove.from.y(),bestMove.to.x(),bestMove.to.y()); //������ɷ����ź�
    this->terminate();      //��ֹ�߳�
    this->wait();           //�ȴ��߳�
}

//���������
void ChessGame::computerRun(const int x1,const int y1,const int x2,const int y2)
{
    QPoint from(x1,y1);
    QPoint to(x2,y2);
    moveChessMan(from,to);
    compThingking = false;
}

//��ʾʱ��
void ChessGame::dispTime()
{
    QString textR=timeRed.toString("mm:ss");        //��׼��ʱ���ʽ���췽��
    this->lcdNumberRed->display(textR);              //��ʾʱ��
    QString textB=timeBlack.toString("mm:ss");   //�ڷ�
    this->lcdNumberBlack->display(textB);
}

//ʱ�临λ
void ChessGame::resetTime()
{
    timeRed.setHMS(0,0,0);
    dispTime();
    timeBlack.setHMS(0,0,0);
}

//������ʱ��
void ChessGame::startTime()
{
    timer->start(1000);
    dispTime();
}

//ֹͣ��ʱ��
void ChessGame::stopTime()
{
    timer->stop();
}

//��ʱ���ۼ�ˢ��
void ChessGame::addTime()
{
    if(turn)
         timeRed=timeRed.addSecs(1);
    else
         timeBlack=timeBlack.addSecs(1);
    this->dispTime();
}

//�������
void ChessGame::saveChess(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);            //��ֻдģʽ���ļ�
    QDataStream data(&file);

    data << this->turn;                 //д�����ڷ�

    for(int i=0; i<32; i++)     //д����������
    {
        data << chessMan[i]->type;
        data << chessMan[i]->chessManId;
        data << chessMan[i]->killed;
        data << chessMan[i]->position;
    }

    for(int i=0; i<steps.size(); i++)   //д�����в���
    {
        data << steps[i].lastStep.movedId;
        data << steps[i].lastStep.from;
        data << steps[i].lastStep.to;
        data << steps[i].lastStep.movedId;
    }
}

//��������
void ChessGame::loadChess(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))            //��ֻ��ģʽ���ļ�
        return;
    QDataStream data(&file);

    data >> this->turn;     //�������巽

    for(int i=0; i<32; i++)      //����32������λ��
    {
        data >> chessMan[i]->type;
        data >> chessMan[i]->chessManId;
        data >> chessMan[i]->killed;
        data >> chessMan[i]->position;
    }

    while(1)
    {
        if(data.atEnd())
            break;
        UnDoChessStep step;
        data >> step.lastStep.movedId;              //���������岽
        data >> step.lastStep.from;
        data >> step.lastStep.to;
        data >> step.lastStep.movedId;
        steps.push_back(step);
    }
    for(int i = 0;i < 10;i++)                       //��ʽ������
        for(int j = 0;j < 9;j++)
            chessBoard[i][j] = -1;
    for(int i=0; i<32; i++)                       //������������
    {
        chessMan[i]->update();
        chessMan[i]->setGeometry(
                    chessMan[i]->position.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_HEIGHT/2,
                    chessMan[i]->position.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                    CMAN_WIDTH,CMAN_HEIGHT);
        chessBoard[chessMan[i]->position.y()][chessMan[i]->position.x()] = chessMan[i]->chessManId;
        if(chessMan[i]->killed)
            chessMan[i]->hide();
        else
            chessMan[i]->show();
    }
    this->selecteid = -1;
}


groundMusic::groundMusic()
{
    mediaObject=Phonon::createPlayer(                   //������ý����ʵ�������ű�������
                Phonon::MusicCategory,
                Phonon::MediaSource("./sounds/bgsound.wav"));
    connect(mediaObject,SIGNAL(aboutToFinish()),this,SLOT(MusicOn()));
    mediaObject->play();
}

groundMusic::~groundMusic()
{

}

void groundMusic::run()
{
    mediaObject->play();
}

void groundMusic::stop()
{
    mediaObject->stop();
    free(mediaObject);
}

void groundMusic::MusicOn()
{
    mediaObject->stop();
    mediaObject->setCurrentSource(Phonon::MediaSource(QString("./sounds/WIN.wav")));
    mediaObject->enqueue(Phonon::MediaSource(QString("./sounds/bgsound.wav")));
   // mediaObject->seek(mediaObject->totalTime());
    mediaObject->play();
}
