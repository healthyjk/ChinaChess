#include "chessgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QSound>
#include <string>
#include <QPixmap>
#include <QDebug>

//主游戏类中使用到的常量
QPoint initPos(BOARD_LEFT_MARGIN,BOARD_TOP_MARGIN);     //棋盘第一个格点（0,0）所在的位置
std::string chesssManNames[14]={"将","车","马","炮","士","象","卒","帅","车","马","炮","仕","相","兵"};   //双方棋子名称
std::string redStepXName[9] = {"九","八","七","六","五","四","三","二","一"};            //红方步法X坐标
std::string redStepYName[9] = {"一","二","三","四","五","六","七","八","九"};            //红方步法Y坐标
QString chuMens[9] = {"xiangyu","fanzeng","xiangzhuang","yuziqi","yingbu","jibu","zhonglimei","longqie","yuji"};
QString hanMens[9] = {"liubang","hanxin","xiaohe","zhangliang","fankuai","chenping","caocan","pengyue","lvzhi"};
std::string chuMenIntro[9] = {"项羽：西楚霸王项羽,武勇古今无双，赞曰：力拔山兮气盖世",
                              "范增：西楚霸王项羽的谋士，被尊为“亚夫”。“年七十，素居家，好奇计”",
                              "项庄：西楚霸王项羽的堂弟，作为项羽麾下的武将一直追随项羽南征北战",
                              "虞子期：霸王项羽麾下五大将之一，项羽兵败垓下后，一直追随战死到最后",
                              "英布：秦末汉初名将，因受秦律被黔，初属项梁，后为项羽帐下五大将之一",
                              "季布：西汉官吏，项羽麾下五大将之一，好打抱不平，以信守承诺著称",
                              "钟离昧：项羽麾下一员武艺超群的上将，多次在与刘邦正面对峙时给刘邦以沉重打击",
                              "龙且：项羽麾下五大将之一，官拜西楚国大司马",
                              "虞姬:楚汉之争时期“西楚霸王”项羽的爱姬名虞,在四面楚歌的困境下一直陪伴在项羽身边,最终自尽"};
std::string hanMenIntro[9] = {"刘邦：汉高祖刘邦，谥号高皇帝。刘邦约法三章：“杀人者死，伤人及盗抵罪”。",
                              "韩信：汉初三杰之一。刘邦评价之：“战必胜，攻必取，吾不如韩信”。",
                              "萧何：汉初三杰之一。《史记》载：“萧相国何者，以文无害，为沛主吏缘。”",
                              "张良：汉初三杰之一。刘邦评价曰：“运筹帷幄之中，决胜千里之外，吾不如子房”。",
                              "樊哙：西汉大将军，左丞相。为汉高祖刘邦第一心腹，仅次于项羽的第二猛将",
                              "陈平：西汉王朝的开国功臣之一。“反间计”，“离间计”均出自其手。",
                              "曹参：西汉王朝的开国功臣之一。跟随刘邦在沛县起兵反秦，身经百战",
                              "彭越：西汉王朝的开国功臣之一。秦末聚兵起义，后率军归顺刘邦，拜魏相国，建成侯",
                              "吕雉：汉高皇后，中国历史上第一位皇后，第一个临朝称制的女子，掌握汉朝政权六十年"};

//电脑线程共享变量
ChessStep lastStep;  //电脑的上一步思考结果

//主游戏类构造函数
ChessGame::ChessGame(QWidget *parent) :
    QWidget(parent),timeRed(0,0,0),timeBlack(0,0,0)
{
    this->setGeometry(0,0,1060,700);
    this->setAutoFillBackground(true);
    QPixmap pixmap("./images/bg.png");
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));     //设置窗口背景
    this->setPalette(palette);

    initInterface();        //初始化界面
    initChessBoard();       //初始化棋盘

    timer=new QTimer(this);    //初始化定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(addTime()));

    //初始化AI引擎
    this->moveGenerator = new ChessMoveGenerator();
    this->moveGenerator->moveListCount = 0;
    this->evaluator = new ChessEvaluation();
    this->engine = new NegaScout_TT_HH_Engine();
    engine->setMoveGenerator(moveGenerator);
    engine->setEvaluator(evaluator);

    //对战方式
    chessMode = -1;
    initParameter();        //初始化棋盘
    configuration();        //初始化设置
}

//初始化界面
void ChessGame::initInterface()
{
    user1 = new QLabel(this);       //用户1介绍
    user1->setGeometry(QRect(20,180,150,65));
    user1->setFrameShape(QFrame::Box);
    user1->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(0, 0, 0);");
    user1->setWordWrap(true);
    user1->setAlignment(Qt::AlignTop);

    user2 = new QLabel(this);       //用户2介绍
    user2->setFrameShape(QFrame::Box);
    user2->setGeometry(QRect(20,570,150,65));
    user2->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(255, 0, 0);");
    user2->setWordWrap(true);
    user2->setAlignment(Qt::AlignTop);

    moveHistory = new QTextBrowser(this);       //步法历史文本框
    moveHistory->setGeometry(QRect(830,75,270,250));
    moveHistory->raise();
    moveHistory->setFontFamily("微软雅黑");
    moveHistory->setFontPointSize(12);
    QPalette mH = moveHistory->palette();
    mH.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    moveHistory->setPalette(mH);

    undoButton = new QPushButton(this);         //悔棋按钮
    connect(undoButton,SIGNAL(clicked()),this,SLOT(back()));
    undoButton->setGeometry(QRect(850,400,150,50));
    undoButton->setStyleSheet("background-image:url(./images/undo.png);width:150px;height:50px;");

    modeChangeButton = new QPushButton(this);       //模式切换按钮
    modeChangeButton->setGeometry(QRect(850,470,150,50));
    connect(modeChangeButton,SIGNAL(clicked()),this,SLOT(modeChange()));
    modeChangeButton->setStyleSheet("background-image:url(./images/qiehuan.png);width:150px;height:50px;");

    surrenderButton = new QPushButton(this);        //认输按钮
    surrenderButton->setGeometry(QRect(850,540,150,50));
    connect(surrenderButton,SIGNAL(clicked()),this,SLOT(surrender()));
    surrenderButton->setStyleSheet("background-image:url(./images/surrender.png);width:150px;height:50px;");

    tryAgainButton = new QPushButton(this);     //重新开始按钮
    tryAgainButton->setGeometry(QRect(850,610,150,50));
    connect(tryAgainButton,SIGNAL(clicked()),this,SLOT(newGame()));
    tryAgainButton->setStyleSheet("background-image:url(./images/newgame.png);width:150px;height:50px;");

    lcdNumberBlack = new QLCDNumber(this);      //时间显示液晶
    lcdNumberRed = new QLCDNumber(this);
    lcdNumberBlack->setGeometry(80,290,80,35);
    lcdNumberRed->setGeometry(80,340,80,35);

}

//初始化棋盘
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
    memcpy(chessBoard, initBoard, sizeof(chessBoard));      //拷贝到chessBoard，用于初始化棋盘
    QPoint pos;
    for(int i = 0,k = 0;i < 10;i++)
        for(int j = 0;j < 9;j++)
        {
            if(chessBoard[i][j] != NOCHESS)
            {
                pos.setX(j);
                pos.setY(i);
                chessMan[k] = new ChessMan(chessBoard[i][j],pos,k,false,false,this);    //顺序构造32颗棋子
                chessBoard[i][j] = k;                       //更新棋盘数组为棋子ＩＤ
                chessMan[k++]->show();
            }
            else
                chessBoard[i][j] = -1;
        }
}

//初始化游戏参数
void ChessGame::initParameter()
{
    selecteid = -1;
    turn = 1;
    stepCount = 0;
    flag = false;
    lastMoveF.setX(-1);     //上一步为空
    lastMoveF.setY(-1);
    lastMoveT.setX(-1);
    lastMoveT.setY(-1);
    this->update();
    compThingking = false;  //电脑不在思考
    chumen = rand()%9;
    hanmen = rand()%9;

}

//选项设置
void ChessGame::configuration()
{
    configDialog = new ConfigDialog(this);
    configDialog->move ((QApplication::desktop()->width() - configDialog->width())/2,
                          (QApplication::desktop()->height() - configDialog->height())/2);
    connect(configDialog,SIGNAL(getAllConfig(int,int,bool,bool)),this,SLOT(setCustomConfig(int,int,bool,bool)));
    connect(configDialog,SIGNAL(getDefaultConfig()),this,SLOT(setDefaultConfig()));
    configDialog->exec();
}

//用户自定义参数设置
void ChessGame::setCustomConfig(const int chessMode,const int compuLevel,const bool bgMusicSwitch,const bool atMusicSwitch)
{
    this->compLevel = compuLevel;
    this->bgMusicSwitch = bgMusicSwitch;
    this->atMusicSwitch = atMusicSwitch;
    engine->setSearchDepth(compLevel);
    if(this->chessMode != chessMode){       //如果用户改变了模式，就重新开始游戏，否则不重新开始
        this->chessMode = chessMode;
        this->newGame();
    }
    playBgMusic();      //播放背景音乐
    startTime();        //启动定时器
}

//默认参数设置
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

void ChessGame::playBgMusic()       //播放背景音乐
{
    if(bgMusicSwitch){
        groundMusic groundsound;
        groundsound.start();
        groundsound.wait();
    }
}

//界面绘制事件重载
void ChessGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;

    pixmap.load("./images/chess_board.png");
    painter.drawPixmap(200,10,605,658,pixmap);      //画出棋盘

    pixmap.load(QString("./images/")+chuMens[chumen]+QString(".jpg"));
    painter.drawPixmap(20,30,150,150,pixmap);       //随机取得人物图片后显示图片
    user1->setText(QString::fromLocal8Bit(chuMenIntro[chumen].c_str()));
    if(chessMode == MAN_PCCHESS)
        pixmap.load("./images/computer.jpg");       //如果是人机模式，画出电脑图片
    else
        pixmap.load("./images/B_KING.png");         //如果是双人模式，画出黑方“将”图片
    painter.drawPixmap(20,20,40,40,pixmap);         //画出红方“帅图片”

    pixmap.load(QString("./images/")+hanMens[hanmen]+QString(".jpg"));
    painter.drawPixmap(20,420,150,150,pixmap);
    user2->setText(QString::fromLocal8Bit(hanMenIntro[hanmen].c_str()));
    pixmap.load("./images/R_KING.png");
    painter.drawPixmap(20,420,40,40,pixmap);

    if(lastMoveF.x() != -1 && lastMoveF.y() != -1 && lastMoveT.x() != -1 && lastMoveT.y() != -1)    //画出上一步运动轨迹
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

//鼠标事件监听函数
void ChessGame::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = (event->pos() - initPos) / BOARD_GAP;             //棋盘上鼠标点击的位置
    if(clickPos.x() < 0 || clickPos.x() > 8
            || clickPos.y() < 0 || clickPos.y() > 9)
        return;
    int chessManId = chessBoard[clickPos.y()][clickPos.x()];
    if(this->selecteid != -1) //如果已经选择棋子
    {
        if(chessManId / 16 == this->turn && chessManId != -1)           //如果是本方棋子
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
        else                                                        //是空格或者对方棋子
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
            if(chessMan[chessManId]->type / 8 == this->turn && !compThingking)//如果可以点击棋子
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

//电脑开启线程进入思考函数
void ChessGame::computerThink()
{
    compThingking = true;
    computerThread[num] = new ComputerThread(this->chessBoard,this->engine,compLevel);
    connect(computerThread[num],SIGNAL(computerMoveSignal(const int,const int,const int,const int)),
            this,SLOT(computerRun(const int,const int,const int,const int)));
    connect(computerThread[num],SIGNAL(terminated()),computerThread[num],SLOT(deleteLater()));//线程结束之后自动删除线程
    computerThread[num]->start();       //启动电脑搜索
    num++;
    if(num==499)
        num = 0;
}

//移动棋子的函数
bool ChessGame::moveChessMan(QPoint &from,QPoint &to)
{
    int fromId = chessBoard[from.y()][from.x()];
    int toId = chessBoard[to.y()][to.x()];
    flag = false;
    //移动棋子前更新棋盘上一步状态
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
                     QSound::play("./sounds/CAPTURE.wav");  //播放吃子音乐
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
        if(canMoveToDst(chessBoard,from,to))      //如果可以走子
        {
            printChessStep(fromId,from,to);

            chessBoard[to.y()][to.x()]          //更新棋盘
                    = fromId;
            chessBoard[from.y()][from.x()] = -1;
            chessMan[fromId]->position.setX(to.x());        //更新棋子
            chessMan[fromId]->position.setY(to.y());
            chessMan[fromId]->selected = false;
            chessMan[fromId]->setGeometry(to.x() * BOARD_GAP + BOARD_LEFT_MARGIN - CMAN_HEIGHT/2,
                                          to.y() * BOARD_GAP + BOARD_TOP_MARGIN - CMAN_HEIGHT/2,
                                          CMAN_WIDTH,CMAN_HEIGHT);
            this->selecteid = -1;
            if(atMusicSwitch)                           //播放移动声音
                  QSound::play("./sounds/MOVE.wav");
            turn = turn == 0? 1 : 0;                             //更换下棋方

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
        int r = QMessageBox::question(NULL, "胜败", "<font color='black'>黑棋赢了！</font></br></br>再来一局？",
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
        int r = QMessageBox::question(NULL, "胜败", "<font color='red'>红棋赢了！</font></br></br>再来一局？",
                              QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        if(r==QMessageBox::Yes){
            this->newGame();
            startTime();
        }
        flag = false;
    }
}

//走子路径转化为象棋步法规则术语
void ChessGame::printChessStep(int chessManId,QPoint& from,QPoint& to)
{
    recordSteps(from,to);
    stepCount++;
    QString step;
    QString chessManName = QString(QString::fromLocal8Bit(chesssManNames[idToType(chessManId) - 1].c_str()));
    QString stepFrom;
    QString stepTo;
    QString stepType;
    if(IsRed(chessManId))           //如果是红子，从左到右为九到一
    {
        stepFrom = QString::fromLocal8Bit(redStepXName[from.x()].c_str());
        if(from.y() == to.y())          //如果是横着走
        {

            stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
            stepType = QString("平");
        }
        else if(from.x() == to.x())
        {
            if(from.y()>to.y())
            {
                stepTo = QString::fromLocal8Bit(redStepYName[from.y()-to.y()-1].c_str());
                stepType = QString("进");
            }
            else
            {
                stepTo = QString::fromLocal8Bit(redStepYName[to.y()-from.y()-1].c_str());
                stepType = QString("退");
            }
        }
        else
        {
            if(from.y()>to.y())
            {
                stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
                stepType = QString("进");
            }
            else
            {
                stepTo = QString::fromLocal8Bit(redStepXName[to.x()].c_str());
                stepType = QString("退");
            }
        }
    }
    else      //如果是黑子，从左到右为1到9
    {
        stepFrom = QString::fromLocal8Bit(redStepXName[from.x()].c_str());
        stepFrom = QString::number(from.x()+1);
        if(from.y() == to.y())               //如果是横着走
        {
            stepTo = QString::number(to.x()+1);
            stepType = QString("平");
        }
        else if(from.x() == to.x())
        {
            if(from.y()>to.y())
            {
                stepTo = QString::number(from.y() - to.y());
                stepType = QString("退");
            }
            else
            {
                stepTo = QString::number(to.y() - from.y());
                stepType = QString("进");
            }
        }
        else
        {
            if(from.y()>to.y())
            {
                stepTo = QString::number(to.x()+1);
                stepType = QString("退");
            }
            else
            {
                stepTo = QString::number(to.x()+1);
                stepType = QString("进");
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

//记录步法
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

//悔棋一步
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
        //次数考虑到双人作战的要求，将悔棋改为1步
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
        turn = (turn == 0) ? 1 : 0;                             //更换下棋方
        lastMoveF = one.lastStep.from;
        lastMoveT = one.lastStep.to;
        this->update();
    }
}

//判断游戏是否结束
ChessGame::ChessState ChessGame::isGameOver()
{
    if(chessMan[R_KING_ID]->killed)     //如果红方帅已死
        return BLACK_WIN;
    else if(chessMan[B_KING_ID]->killed) //如果黑方将已死
        return RED_WIN;
    else if(chessMan[R_KING_ID]->position.x() == chessMan[B_KING_ID]->position.x()      //如果将军会面
            && countChessMan(chessBoard,chessMan[R_KING_ID]->position,chessMan[B_KING_ID]->position)==0)
        return turn == 0? BLACK_WIN : RED_WIN;
    //暂不加入和棋的判断
    return LAST;
}


//开始新的对局
void ChessGame::newGame()
{
    for(int i = 0;i < 32;i++)
    {
        delete chessMan[i];
    }
    steps.clear();      //清空存储的步法
    initChessBoard();
    initParameter();
    resetTime();
    moveHistory->append(QString("新的对局"));
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
    if(chessMode == MAN_MANCHESS){  //如果当前是双人对战
        chessMode = MAN_PCCHESS;   //改为人机对战
        if(this->turn == 0)      //如果是黑子走
            computerThink();     //电脑走一步
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

//电脑思考线程构造函数
ComputerThread::ComputerThread(int chessBoard[10][9], NegaScout_TT_HH_Engine *engine,int compLevel)
{
    this->engine = engine;
    this->compLevel = compLevel;
    memcpy(board,chessBoard,360);
}

//电脑思考线程析构函数
ComputerThread::~ComputerThread()
{
    delete engine;
}

//电脑思考线程
void ComputerThread::run()
{
    engine->searchOneGoodMove(this->board);
    bestMove = engine->getBestMove();       //得到最佳的步法
    if(bestMove.from == lastStep.from && bestMove.to == lastStep.to)
    {
        bestMove = engine->chessMoveGenerator->moveList[compLevel][rand() %
                engine->chessMoveGenerator->moveListCount];
    }
    lastStep = bestMove;
    emit computerMoveSignal(bestMove.from.x(),bestMove.from.y(),bestMove.to.x(),bestMove.to.y()); //搜索完成发出信号
    this->terminate();      //终止线程
    this->wait();           //等待线程
}

//计算机走子
void ChessGame::computerRun(const int x1,const int y1,const int x2,const int y2)
{
    QPoint from(x1,y1);
    QPoint to(x2,y2);
    moveChessMan(from,to);
    compThingking = false;
}

//显示时间
void ChessGame::dispTime()
{
    QString textR=timeRed.toString("mm:ss");        //标准化时间格式（红方）
    this->lcdNumberRed->display(textR);              //显示时间
    QString textB=timeBlack.toString("mm:ss");   //黑方
    this->lcdNumberBlack->display(textB);
}

//时间复位
void ChessGame::resetTime()
{
    timeRed.setHMS(0,0,0);
    dispTime();
    timeBlack.setHMS(0,0,0);
}

//启动定时器
void ChessGame::startTime()
{
    timer->start(1000);
    dispTime();
}

//停止定时器
void ChessGame::stopTime()
{
    timer->stop();
}

//定时器累加刷新
void ChessGame::addTime()
{
    if(turn)
         timeRed=timeRed.addSecs(1);
    else
         timeBlack=timeBlack.addSecs(1);
    this->dispTime();
}

//保存棋局
void ChessGame::saveChess(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);            //以只写模式打开文件
    QDataStream data(&file);

    data << this->turn;                 //写入下期防

    for(int i=0; i<32; i++)     //写入所有棋子
    {
        data << chessMan[i]->type;
        data << chessMan[i]->chessManId;
        data << chessMan[i]->killed;
        data << chessMan[i]->position;
    }

    for(int i=0; i<steps.size(); i++)   //写入所有步法
    {
        data << steps[i].lastStep.movedId;
        data << steps[i].lastStep.from;
        data << steps[i].lastStep.to;
        data << steps[i].lastStep.movedId;
    }
}

//载入棋盘
void ChessGame::loadChess(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))            //以只读模式打开文件
        return;
    QDataStream data(&file);

    data >> this->turn;     //载入下棋方

    for(int i=0; i<32; i++)      //载入32颗棋子位置
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
        data >> step.lastStep.movedId;              //载入所有棋步
        data >> step.lastStep.from;
        data >> step.lastStep.to;
        data >> step.lastStep.movedId;
        steps.push_back(step);
    }
    for(int i = 0;i < 10;i++)                       //格式化棋盘
        for(int j = 0;j < 9;j++)
            chessBoard[i][j] = -1;
    for(int i=0; i<32; i++)                       //更新所有棋子
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
    mediaObject=Phonon::createPlayer(                   //创建多媒体类实例，播放背景音乐
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
