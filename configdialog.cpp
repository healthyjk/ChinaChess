#include "configdialog.h"
#include "chessdefine.h"
#include <QtGui>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("选项设置");
    typeGroup = new QGroupBox(tr("对战模式选择"));
    mmButton = new QRadioButton(this);
    mmLabel = new QLabel(this);
    mmLabel->setText(tr("双人对战"));
    mmLabel->setAlignment(Qt::AlignLeft);
    cmButton = new QRadioButton(this);
    cmButton->setChecked(true);
    cmLabel = new QLabel(this);
    cmLabel->setText(tr("人机对战"));
    cmLabel->setAlignment(Qt::AlignLeft);
    group1 = new QButtonGroup(this);
    group1->addButton(mmButton);
    group1->addButton(cmButton);

    compLevelGroup = new QGroupBox(this);
    compLevelGroup->setTitle(tr("电脑等级选择"));
    compLevelLabel = new QLabel(this);
    compLevelLabel->setText(tr("电脑难度"));

    compLevelCombo = new QComboBox(this);
    compLevelCombo->addItem(tr("简单：初入沙场"));
    compLevelCombo->addItem(tr("中级：身经百战"));
    compLevelCombo->addItem(tr("高级：战无不胜"));

    musicGroup = new QGroupBox(this);
    musicGroup->setTitle(tr("音乐"));
    bgMusicLabel = new QLabel(this);
    bgMusicLabel->setText(tr("背景音乐"));
    bgMusicButton = new QCheckBox(this);
    atMusicLabel = new QLabel(this);
    atMusicLabel->setText(tr("下棋音效"));
    atMusicButton = new QCheckBox(this);

    submit = new QPushButton(this);
    submit->setText(tr("确定"));
    connect(submit,SIGNAL(clicked()),this,SLOT(confirmConfig()));
    cancel = new QPushButton(this);
    cancel->setText(tr("取消"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(closeConfig()));

    //以下为界面布局
    QHBoxLayout *typeMMLayout = new QHBoxLayout;
    typeMMLayout->addWidget(mmLabel);
    typeMMLayout->addWidget(mmButton);

    QHBoxLayout *typeCMLayout = new QHBoxLayout;
    typeCMLayout->addWidget(cmLabel);
    typeCMLayout->addWidget(cmButton);

    QVBoxLayout *typeLayout = new QVBoxLayout;
    typeLayout->addLayout(typeMMLayout);
    typeLayout->addLayout(typeCMLayout);
    typeGroup->setLayout(typeLayout);

    QHBoxLayout *compLevelLayout = new QHBoxLayout;
    compLevelLayout->addWidget(compLevelLabel);
    compLevelLayout->addWidget(compLevelCombo);
    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addLayout(compLevelLayout);
    compLevelGroup->setLayout(compLayout);

    QHBoxLayout *bgMusicLayout = new QHBoxLayout;
    bgMusicLayout->addWidget(bgMusicLabel);
    bgMusicLayout->addWidget(bgMusicButton);
    QHBoxLayout *atMusicLayout = new QHBoxLayout;
    atMusicLayout->addWidget(atMusicLabel);
    atMusicLayout->addWidget(atMusicButton);

    QVBoxLayout *musicLayout = new QVBoxLayout;
    musicLayout->addLayout(bgMusicLayout);
    musicLayout->addLayout(atMusicLayout);
    musicGroup->setLayout(musicLayout);

    QHBoxLayout *submitLayout = new QHBoxLayout;
    submitLayout->addWidget(submit);
    submitLayout->addWidget(cancel);

    QVBoxLayout *submitMainLayout = new QVBoxLayout;
    submitMainLayout->addLayout(submitLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(typeGroup);
    mainLayout->addWidget(compLevelGroup);
    mainLayout->addWidget(musicGroup);
    mainLayout->addLayout(submitMainLayout);
    this->setLayout(mainLayout);
}

//用户点击确定按钮事件
void ConfigDialog::confirmConfig()
{
    if(this->mmButton->isChecked())
        chessMode = MAN_MANCHESS;
    else
        chessMode = MAN_PCCHESS;
    if(compLevelCombo->currentIndex()==0)
        compLevel = 4;
    else if(compLevelCombo->currentIndex()==1)
        compLevel = 5;
    else
        compLevel = 6;
    if(bgMusicButton->isChecked())
        bgMusicSwitch = true;
    else
        bgMusicSwitch = false;
    if(atMusicButton->isChecked())
        atMusicSwitch = true;
    else
        atMusicSwitch = false;
    emit getAllConfig(chessMode,compLevel,bgMusicSwitch,atMusicSwitch);
    this->accept();         //关闭窗口
}

//用户点击取消按钮事件
void ConfigDialog::closeConfig()
{
    emit getDefaultConfig();    //使用默认设置
    this->reject();              //关闭窗口
}
